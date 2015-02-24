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
 *  The MIT License (MIT)
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

#define VEC_DEFAULT_CAPACITY	5
#define VEC_IDX(idx, max)		(((idx) + (max)) % (max))
#define VEC_GET(vec, idx) 		(&(vec)->_mem[(((vec)->start + (idx)) % (vec)->capacity) * (vec)->_t_size])
#define IS_VALID_VEC(vec) \
	((vec) && ((vec)->_t_size) && ((!(vec)->capacity && !(vec)->_mem) || ((vec)->size <= (vec)->capacity && (vec)->start < (vec)->capacity)))

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static int _vec_reserve(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint capacity)
{
	/* Check if we need to do anything */
	if (capacity > vec_ptr->capacity)
	{
		unsigned char *new_mem;
		int circular_items = (vec_ptr->start + vec_ptr->size) - vec_ptr->capacity;
		int use_malloc = circular_items > capacity - vec_ptr->capacity;

		/* Naive optimization: if we have items that circulated to the beginning of vec_ptr->_mem,
		 * check if we can copy them back to the end in a single memcpy call. If so, use realloc
		 * to minimize allocation overhead. Otherwise, the amount of copy needed already creates
		 * a huge overhead. Simply use malloc and copy */
		if (use_malloc)
		{
			new_mem = (unsigned char *)malloc(vec_ptr->_t_size * capacity);
		}
		else
		{
			new_mem = (unsigned char *)realloc(vec_ptr->_mem, vec_ptr->_t_size * capacity);
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
				uint bytes_at_end = (vec_ptr->capacity - (uint)circular_items) * vec_ptr->_t_size;

				/* We need to copy the items from vec_ptr->start until the end of the vec_ptr->_mem and
				 * then copy the rest of the items from the beginning of vec_ptr->_mem */
				memcpy(new_mem, VEC_GET(vec_ptr, 0), bytes_at_end);
				memcpy(new_mem + bytes_at_end, vec_ptr->_mem, (uint)circular_items * vec_ptr->_t_size);

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

static int _vec_insert(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx, const void *val)
{
	/* Don't allow adding elements in arbitrary place that is beyond vec_ptr->size */
	if (idx > vec_ptr->size)
	{
		return FALSE;
	}

	/* Check if we need to reserve more memory */
	if (vec_ptr->size >= vec_ptr->capacity)
	{
		uint addition = (vec_ptr->capacity ? vec_ptr->capacity : VEC_DEFAULT_CAPACITY);

		while (!_vec_reserve(vec_ptr, vec_ptr->capacity + addition))
		{
			/* Divide by two and try again unless reserve failed even for a single elemnt more */
			if (!(addition >>= 1))
			{
				return FALSE;
			}
		}
	}

    /* Perform all sort of crazy calculations only if we already have some elements and
     * we might need to move them around. Otherwise this is the first insertion, so simply
     * put it in idx 0 and be done with it */
    if (vec_ptr->size > 0)
    {
        /* Check if we can create free slot at idx with simple pointer regression */
    	if (idx == 0)
        {
            vec_ptr->start = VEC_IDX(vec_ptr->start - 1, vec_ptr->capacity);
        }
        else if (idx < vec_ptr->size)
        {
            uint free_idx = vec_ptr->size;

            /* Copy elements from the end of vec_ptr until idx one slot forward */
            while (free_idx > idx)
            {
                memcpy(VEC_GET(vec_ptr, free_idx), VEC_GET(vec_ptr, free_idx - 1), vec_ptr->_t_size);
                --free_idx;
            }
        }
    }

    /* Put val in the free slot */
    memcpy(VEC_GET(vec_ptr, idx), val, vec_ptr->_t_size);
    ++vec_ptr->size;

	return TRUE;
}

static int _vec_remove(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx, void *opt_out_val)
{
	/* Make sure that the index is reachable */
	if (idx >= vec_ptr->size)
	{
		return FALSE;
	}

	/* Check if we need to copy the value that is being removed */
	if (opt_out_val)
	{
		memcpy(opt_out_val, VEC_GET(vec_ptr, idx), vec_ptr->_t_size);
	}

    --vec_ptr->size;

	/* Check if we can finish with a simple start pointer advancement */
	if (idx == 0)
	{
		vec_ptr->start = VEC_IDX(vec_ptr->start + 1, vec_ptr->capacity);
	}
	else if (idx < vec_ptr->size)
	{
        uint free_idx = idx;

        /* Shift items from idx + 1 until the end of vec_ptr one slot back */
        while (free_idx < vec_ptr->size)
        {
            memcpy(VEC_GET(vec_ptr, free_idx), VEC_GET(vec_ptr, free_idx + 1), vec_ptr->_t_size);
            ++free_idx;
        }
	}

	return TRUE;
}

int _impl_vec_init(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint t_size)
{
	if ((!vec_ptr) || (!t_size))
	{
		return FALSE;
	}

	memset(vec_ptr, 0, sizeof(_IMPL_VEC_STRUCT_NAME));
	vec_ptr->_t_size = t_size;

	return TRUE;
}

int _impl_vec_reserve(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint capacity)
{
	if (!IS_VALID_VEC(vec_ptr))
	{
		return FALSE;
	}

	return _vec_reserve(vec_ptr, capacity);
}

uint _impl_vec_size(const _IMPL_VEC_STRUCT_NAME *vec_ptr)
{
	if (!IS_VALID_VEC(vec_ptr))
	{
		return 0;
	}

	return vec_ptr->size;
}

uint _impl_vec_capacity(const _IMPL_VEC_STRUCT_NAME *vec_ptr)
{
	if (!IS_VALID_VEC(vec_ptr))
	{
		return 0;
	}

	return vec_ptr->capacity;
}

void* _impl_vec_get(const _IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx)
{
	if (!IS_VALID_VEC(vec_ptr) || (idx >= vec_ptr->size))
	{
		return NULL;
	}

	return VEC_GET(vec_ptr, idx);
}

void* _impl_vec_first(const _IMPL_VEC_STRUCT_NAME *vec_ptr)
{
	if (!IS_VALID_VEC(vec_ptr) || !vec_ptr->size)
	{
		return NULL;
	}

	return VEC_GET(vec_ptr, 0);
}

void* _impl_vec_last(const _IMPL_VEC_STRUCT_NAME *vec_ptr)
{
	if (!IS_VALID_VEC(vec_ptr) || !vec_ptr->size)
	{
		return NULL;
	}

	return VEC_GET(vec_ptr, vec_ptr->size - 1);
}

int _impl_vec_assign(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx, const void *val)
{
	if (!IS_VALID_VEC(vec_ptr) || idx >= vec_ptr->size || !val)
	{
		return FALSE;
	}

	memcpy(VEC_GET(vec_ptr, idx), val, vec_ptr->_t_size);
	return TRUE;
}

int _impl_vec_push(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val)
{
	if (!IS_VALID_VEC(vec_ptr) || !val)
	{
		return FALSE;
	}

	return _vec_insert(vec_ptr, vec_ptr->size, val);
}

int _impl_vec_pop(_IMPL_VEC_STRUCT_NAME *vec_ptr, void *out)
{
	if (!IS_VALID_VEC(vec_ptr))
	{
		return FALSE;
	}

	return _vec_remove(vec_ptr, vec_ptr->size - 1, out);
}

int _impl_vec_shift(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val)
{
	if (!IS_VALID_VEC(vec_ptr) || !val)
	{
		return FALSE;
	}

	return _vec_insert(vec_ptr, 0, val);
}

int _impl_vec_unshift(_IMPL_VEC_STRUCT_NAME *vec_ptr, void *out)
{
	if (!IS_VALID_VEC(vec_ptr))
	{
		return FALSE;
	}

	return _vec_remove(vec_ptr, 0, out);
}

int _impl_vec_erase(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx, void *out)
{
	if (!IS_VALID_VEC(vec_ptr))
	{
		return FALSE;
	}

	return _vec_remove(vec_ptr, idx, out);
}

int _impl_vec_insert(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val, uint idx)
{
	if (!IS_VALID_VEC(vec_ptr) || !val)
	{
		return FALSE;
	}

	return _vec_insert(vec_ptr, idx, val);
}

void _impl_vec_clear(_IMPL_VEC_STRUCT_NAME *vec_ptr)
{
	if (IS_VALID_VEC(vec_ptr))
	{
		/* Save the type size before the memset call below */
		uint t_size = vec_ptr->_t_size;

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
