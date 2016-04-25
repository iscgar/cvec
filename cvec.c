/**
 * @file    cvec.c
 * @author  Isaac Garzon
 * @since   24/02/2015
 *
 * @section DESCRIPTION
 *
 *  A generic vector implementation in ANSI C.
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015 Isaac Garzon
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE
 */

#include <stdlib.h>
#include <string.h>
#include "cvec.h"

#define VEC_DEFAULT_CAPACITY    5
#define VEC_IDX(idx, max)       (((idx) + (max)) % (max))
#define VEC_GET(vec, idx)       (&(vec)->_mem[(((vec)->start + (idx)) % (vec)->capacity) * (vec)->_t_size])
#define IS_VALID_VEC(vec) \
    ((vec) && ((vec)->_t_size) && \
     ((!(vec)->capacity && !(vec)->_mem) || \
      ((vec)->size <= (vec)->capacity && (vec)->start < (vec)->capacity)))

_impl_vec_def_struct(unsigned char, _IMPL_VEC_STRUCT_NAME);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static void _vec_rotate(_IMPL_VEC_STRUCT_NAME *vec_ptr, void *tmp)
{
    unsigned int count = 0;
    unsigned int offset = 0;
    unsigned int len = vec_ptr->capacity;
    unsigned int tsize = vec_ptr->_t_size;

    while (count < len)
    {
        unsigned int index = offset;
        unsigned int next = (vec_ptr->start + index) % len;

        memcpy(tmp, &vec_ptr->_mem[index * tsize], tsize);

        while (next != offset)
        {
            memcpy(&vec_ptr->_mem[index * tsize], &vec_ptr->_mem[next * tsize], tsize);
            ++count;

            index = next;
            next = (vec_ptr->start + index) % len;
        }

        memcpy(&vec_ptr->_mem[index * tsize], tmp, tsize);
        ++count;
        ++offset;
    }

    vec_ptr->start = 0;
}

static void _vec_align(_IMPL_VEC_STRUCT_NAME *vec_ptr, void *tmp)
{
    int circular_items = (vec_ptr->start + vec_ptr->size) - vec_ptr->capacity;

    /* Check if we actually need to align anything */
    if (circular_items > 0)
    {
        /* Check if we can align without full rotation */
        if (vec_ptr->capacity - vec_ptr->size < vec_ptr->size)
        {
            _vec_rotate(vec_ptr, tmp);
        }
        else
        {
            unsigned int offset = 0;
            unsigned int tsize = vec_ptr->_t_size;
            unsigned int items = vec_ptr->size;
            unsigned int last = vec_ptr->start;
            unsigned int end = items - vec_ptr->capacity + vec_ptr->start;

            /* Copy the circulated items just behind vec_ptr->start */
            while (end)
            {
                memcpy(&vec_ptr->_mem[--last * tsize], &vec_ptr->_mem[--end * tsize], tsize);
            }

            /* Copy items to the beginning of the buffer */
            while ((offset < items) && (vec_ptr->start < vec_ptr->capacity))
            {
                memcpy(&vec_ptr->_mem[offset++ * tsize], &vec_ptr->_mem[vec_ptr->start++ * tsize], tsize);
            }

            /* Copy the circulated items from before to their right place */
            while (offset < items)
            {
                memcpy(&vec_ptr->_mem[offset++ * tsize], &vec_ptr->_mem[last++ * tsize], tsize);
            }

            vec_ptr->start = 0;
        }
    }
}

static void _vec_swap(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int first, unsigned int second, void *tmp)
{
    memcpy(tmp, VEC_GET(vec_ptr, first), vec_ptr->_t_size);
    memcpy(VEC_GET(vec_ptr, first), VEC_GET(vec_ptr, second), vec_ptr->_t_size);
    memcpy(VEC_GET(vec_ptr, second), tmp, vec_ptr->_t_size);
}

static void _vec_sort(_IMPL_VEC_STRUCT_NAME *vec_ptr, _IMPL_VEC_CMPFN_NAME cmpfn, void *tmp)
{
    /* Check if we actually need to sort anything */
    if (vec_ptr->size)
    {
        /* Align the buffer so we can pass it to qsort */
        _vec_align(vec_ptr, tmp);
        qsort(VEC_GET(vec_ptr, 0), vec_ptr->size, vec_ptr->_t_size, cmpfn);
    }
}

static int _vec_reserve(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int capacity)
{
    /* Check if we need to do anything */
    if (capacity > vec_ptr->capacity)
    {
        unsigned char *new_mem;
        int circular_items = (vec_ptr->start + vec_ptr->size) - vec_ptr->capacity;
        int use_malloc = (circular_items > 0) && ((unsigned int)circular_items > capacity - vec_ptr->capacity);
        unsigned int size = capacity * vec_ptr->_t_size;

        /* Avoid integer overflow */
        if (size / vec_ptr->_t_size != capacity)
        {
            return FALSE;
        }

        /* Naive optimization: if we have items that circulated to the beginning of vec_ptr->_mem,
         * check if we can copy them back to the end in a single memcpy call. If so, use realloc
         * to minimize allocation overhead. Otherwise, the amount of copy needed already creates
         * a huge overhead. Simply use malloc and copy */
        if (use_malloc)
        {
            new_mem = (unsigned char *)malloc(size);
        }
        else
        {
            new_mem = (unsigned char *)realloc(vec_ptr->_mem, size);
        }

        /* Check if allocation succeeded */
        if (!new_mem)
        {
            return FALSE;
        }

        /* Check if there are items that circulated beyond the buffer end */
        if (circular_items > 0)
        {
            if (!use_malloc)
            {
                memcpy(new_mem, VEC_GET(vec_ptr, 0), circular_items * vec_ptr->_t_size);
            }
            else
            {
                unsigned int bytes_at_end = (vec_ptr->capacity - (unsigned int)circular_items) * vec_ptr->_t_size;

                /* We need to copy the items from vec_ptr->start until the end of the vec_ptr->_mem and
                 * then copy the rest of the items from the beginning of vec_ptr->_mem */
                memcpy(new_mem, VEC_GET(vec_ptr, 0), bytes_at_end);
                memcpy(new_mem + bytes_at_end, vec_ptr->_mem, (unsigned int)circular_items * vec_ptr->_t_size);

                /* We're now at the beginning of the memory block */
                vec_ptr->start = 0;

                /* Free the old memory */
                free(vec_ptr->_mem);
            }
        }

        /* Set the new values */
        vec_ptr->_mem = new_mem;
        vec_ptr->capacity = capacity;
    }

    return TRUE;
}

static int _vec_insert(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx, const unsigned char *val, unsigned int len)
{
    /* Don't allow adding elements in arbitrary place that is beyond vec_ptr->size
     * and avoid integer overflow */
    if ((idx > vec_ptr->size) || (vec_ptr->size + len < vec_ptr->size))
    {
        return FALSE;
    }

    /* Check if we actually need to do anything */
    if (len)
    {
        /* Check if we need to reserve more memory */
        if (vec_ptr->size + len > vec_ptr->capacity)
        {
            unsigned int addition = (vec_ptr->capacity ? vec_ptr->capacity : VEC_DEFAULT_CAPACITY);

            /* Make sure we have enough to store len elements */
            while (addition < len)
            {
                addition <<= 1;
            }

            /* Avoid integer overflow */
            if (vec_ptr->capacity + addition < vec_ptr->capacity)
            {
                addition = len;
            }

            /* Try to reserve place to fit at least len elements */
            while (!_vec_reserve(vec_ptr, vec_ptr->capacity + addition))
            {
                /* Divide by two and try again unless reserve failed even for exactly len elemnts more */
                if ((addition >>= 1) < len)
                {
                    return FALSE;
                }
            }
        }

        /* Perform all sorts of crazy calculations only if we already have some elements and
         * we might need to move them around. Otherwise this is the first insertion, so simply
         * put it in idx 0 and be done with it */
        if (vec_ptr->size > 0)
        {
            /* Check if we can create free slot at idx with simple pointer regression */
            if (idx == 0)
            {
                vec_ptr->start = VEC_IDX(vec_ptr->start - len, vec_ptr->capacity);
            }
            else if (idx < vec_ptr->size)
            {
                unsigned int free_idx = vec_ptr->size + len;
                unsigned int elements = len;

                /* Copy elements from the end of vec_ptr until idx len slots forward */
                do
                {
                    memcpy(VEC_GET(vec_ptr, free_idx), VEC_GET(vec_ptr, free_idx - len), vec_ptr->_t_size);
                    --free_idx, --elements;
                } while (elements);
            }
        }

		vec_ptr->size += len;

		/* Copy len elements from val into the free slots */
        do
        {
            memcpy(VEC_GET(vec_ptr, idx), val, vec_ptr->_t_size);
            ++idx;
            val += vec_ptr->_t_size;
        } while (--len);
    }

    return TRUE;
}

static int _vec_remove(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx, unsigned int len, void *opt_out_val)
{
    /* Make sure that the index is reachable */
    if ((len > vec_ptr->size) || (vec_ptr->size - len < idx))
    {
        return FALSE;
    }

    /* Check if we actually need to do anything */
    if (len)
    {
        /* Check if we need to copy the value that is being removed */
        if (opt_out_val)
        {
            unsigned int curr_idx = 0;
            unsigned char *out_ptr = (unsigned char *)opt_out_val;

            /* Copy len elements to the output variable */
            while (curr_idx < len)
            {
                memcpy(out_ptr, VEC_GET(vec_ptr, idx + curr_idx), vec_ptr->_t_size);
                ++curr_idx;
                out_ptr += vec_ptr->_t_size;
            }
        }

        vec_ptr->size -= len;

        /* Check if we need to shift elements around because of the removal */
        if (vec_ptr->size)
        {
            /* Check if we can finish with a simple start pointer advancement */
            if (idx == 0)
            {
                vec_ptr->start = VEC_IDX(vec_ptr->start + len, vec_ptr->capacity);
            }
            else if (idx < vec_ptr->size)
            {
                unsigned int free_idx = idx;
                unsigned int used_idx = idx + len;

                /* Shift items from used_idx until the end of vec_ptr len slots back */
                do
                {
                    memcpy(VEC_GET(vec_ptr, free_idx), VEC_GET(vec_ptr, used_idx), vec_ptr->_t_size);
                    ++free_idx, ++used_idx;
                } while (free_idx < vec_ptr->size);
            }
        }
    }

    return TRUE;
}

int _impl_vec_init(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int t_size)
{
    if ((!vec_ptr) || (!t_size))
    {
        return FALSE;
    }

    memset(vec_ptr, 0, sizeof(_IMPL_VEC_STRUCT_NAME));
    vec_ptr->_t_size = t_size;

    return TRUE;
}

int _impl_vec_reserve(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int capacity)
{
    if (!IS_VALID_VEC(vec_ptr))
    {
        return FALSE;
    }

    return _vec_reserve(vec_ptr, capacity);
}

unsigned int _impl_vec_size(const _IMPL_VEC_STRUCT_NAME *vec_ptr)
{
    if (!IS_VALID_VEC(vec_ptr))
    {
        return 0;
    }

    return vec_ptr->size;
}

unsigned int _impl_vec_capacity(const _IMPL_VEC_STRUCT_NAME *vec_ptr)
{
    if (!IS_VALID_VEC(vec_ptr))
    {
        return 0;
    }

    return vec_ptr->capacity;
}

void* _impl_vec_get(const _IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx)
{
    if (!IS_VALID_VEC(vec_ptr) || (idx >= vec_ptr->size))
    {
        return NULL;
    }

    return VEC_GET(vec_ptr, idx);
}

void* _impl_vec_last(const _IMPL_VEC_STRUCT_NAME *vec_ptr)
{
    if (!IS_VALID_VEC(vec_ptr) || !vec_ptr->size)
    {
        return NULL;
    }

    return VEC_GET(vec_ptr, vec_ptr->size - 1);
}

int _impl_vec_assign(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx, const void *val)
{
    if (!IS_VALID_VEC(vec_ptr) || idx >= vec_ptr->size || !val)
    {
        return FALSE;
    }

    memcpy(VEC_GET(vec_ptr, idx), val, vec_ptr->_t_size);
    return TRUE;
}

int _impl_vec_swap(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int first, unsigned int second, void *tmp)
{
    if (!IS_VALID_VEC(vec_ptr) || (first >= vec_ptr->size) || (second >= vec_ptr->size))
    {
        return FALSE;
    }

    /* Check if we actually need to do anything */
    if (first != second)
    {
        _vec_swap(vec_ptr, first, second, tmp);
    }

    return TRUE;
}

int _impl_vec_sort(_IMPL_VEC_STRUCT_NAME *vec_ptr, _IMPL_VEC_CMPFN_NAME cmpfn, void *tmp)
{
    if (!IS_VALID_VEC(vec_ptr) || !cmpfn || !tmp)
    {
        return FALSE;
    }

    _vec_sort(vec_ptr, cmpfn, tmp);
    return TRUE;
}

int _impl_vec_push(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val)
{
    if (!IS_VALID_VEC(vec_ptr) || !val)
    {
        return FALSE;
    }

    return _vec_insert(vec_ptr, vec_ptr->size, (const unsigned char *)val, 1);
}

int _impl_vec_pusharr(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val, unsigned int len)
{
    if (!IS_VALID_VEC(vec_ptr) || !val)
    {
        return FALSE;
    }

    return _vec_insert(vec_ptr, vec_ptr->size, (const unsigned char *)val, len);
}

int _impl_vec_pop(_IMPL_VEC_STRUCT_NAME *vec_ptr, void *out)
{
    if (!IS_VALID_VEC(vec_ptr))
    {
        return FALSE;
    }

    return _vec_remove(vec_ptr, vec_ptr->size - 1, 1, out);
}

int _impl_vec_poparr(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int len, void *out)
{
    if (!IS_VALID_VEC(vec_ptr))
    {
        return FALSE;
    }

    return _vec_remove(vec_ptr, vec_ptr->size - 1, len, out);
}

int _impl_vec_erase(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx, unsigned int len, void *out)
{
    if (!IS_VALID_VEC(vec_ptr))
    {
        return FALSE;
    }

    return _vec_remove(vec_ptr, idx, len, out);
}

int _impl_vec_insert(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val, unsigned int idx, unsigned int len)
{
    if (!IS_VALID_VEC(vec_ptr) || !val)
    {
        return FALSE;
    }

    return _vec_insert(vec_ptr, idx, (const unsigned char *)val, len);
}

void _impl_vec_clear(_IMPL_VEC_STRUCT_NAME *vec_ptr)
{
    if (IS_VALID_VEC(vec_ptr))
    {
        /* Save the type size before the memset call below */
        unsigned int t_size = vec_ptr->_t_size;

        if (vec_ptr->_mem)
        {
            free(vec_ptr->_mem);
        }

        /* Clear the struct and restore the type size to allow reuse without calling vec_init */
        memset(vec_ptr, 0, sizeof(_IMPL_VEC_STRUCT_NAME));
        vec_ptr->_t_size = t_size;
    }
}

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */
