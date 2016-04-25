/**
 * @file    cvec.h
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

#ifdef _MSC_VER
#   pragma once
#endif

#ifndef _GENERIC_CVEC_H_
#define _GENERIC_CVEC_H_

/****************************************************************************************
  Internal Macro Definitions
 ***************************************************************************************/
/**
 * @internal
 * Defines NULL in case it's not defined already
 */
#ifndef NULL
#    define NULL 0
#endif

/**
 * @internal
 * Defines FALSE in case it's not defined already
 */
#ifndef FALSE
#    define FALSE 0
#endif

/**
 * @internal
 * Defines TRUE in case it's not defined already
 */
#ifndef TRUE
#    define TRUE (!FALSE)
#endif

/**
 * @internal
 * Simple concatenation macros.
 */
#define _VEC_CAT(a, b) _X_VEC_CAT(a, b)
#define _X_VEC_CAT(a, b) a##b

/**
 * @internal
 * Defines the type name for the generic vec type
 */
#define _IMPL_VEC_STRUCT_NAME   _impl_vec_struct
#define _IMPL_VEC_CMPFN_NAME    _impl_vec_cmpfn

/**
 * @internal
 * Defines a <code>vec</code> struct of type <code>type</code>
 * with the name <code>name</code>
 */
#define _impl_vec_def_struct(type, name) \
typedef struct _VEC_CAT(_vectag, name) \
{ \
    unsigned int start; \
    unsigned int size; \
    unsigned int capacity; \
    unsigned int _t_size; \
    type        *_mem; \
} name

/**
 * @internal
 * Defines a comparer function pointer type for a <code>vec</code> of
 * type <code>type</code> with the name <code>name</code>
 */
#define _impl_vec_def_cmp(type, name) typedef int (*name)(const type *, const type *)

/****************************************************************************************
  Internal Type Definitions
 ***************************************************************************************/
/**
 * @internal
 * Declares the generic <code>vec</code> structure
 */
struct _VEC_CAT(_vectag, _IMPL_VEC_STRUCT_NAME);
typedef struct _VEC_CAT(_vectag, _IMPL_VEC_STRUCT_NAME) _IMPL_VEC_STRUCT_NAME;

/**
 * @internal
 * Defines the generic comparer function.
 */
_impl_vec_def_cmp(void, _IMPL_VEC_CMPFN_NAME);

/****************************************************************************************
  Internal Function Declarations
 ***************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @internal
 * @brief   Initializes a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] t_size    The size of the type in bytes
 *                      (the result of the <code>sizeof</code> operator).
 *
 * @return    TRUE if the initialization succeeded. FALSE otherwise.
 */
extern int (_impl_vec_init)(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int t_size);

/**
 * @internal
 * @brief   Reserves memory in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] capacity  The capacity to reserve.
 *
 * @note      Memory is only allocated if @p capacity exceeds the current
 *            capacity. Otherwise this function does nothing.
 *
 * @return    TRUE if the reservation succeeded. FALSE otherwise.
 */
extern int (_impl_vec_reserve)(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int capacity);

/**
 * @internal
 * @brief   Gets the count of elements stored in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 *
 * @return    The count of elements stored in the <code>vec</code>.
 */
extern unsigned int (_impl_vec_size)(const _IMPL_VEC_STRUCT_NAME *vec_ptr);

/**
 * @internal
 * @brief   Gets the capacity of a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 *
 * @return    The capacity of the <code>vec</code>.
 */
extern unsigned int (_impl_vec_capacity)(const _IMPL_VEC_STRUCT_NAME *vec_ptr);

/**
 * @internal
 * @brief   Gets an element from a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] idx       The index of the element.
 *
 * @return    Pointer to the element if @p vec_ptr and @p idx are valid.
 *            NULL otherwise.
 */
extern void* (_impl_vec_get)(const _IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx);

/**
 * @internal
 * @brief   Gets the last element in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 *
 * @return    Pointer to the last element if any. NULL otherwise.
 */
extern void* (_impl_vec_last)(const _IMPL_VEC_STRUCT_NAME *vec_ptr);

/**
 * @internal
 * @brief   Assigns a value to an element in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] idx       The index to assign the value to.
 * @param[in] val       Pointer to the value to assign.
 *
 * @note      @p idx is valid only if its value is between 0 and the
 *            <code>vec</code>'s size (inclusive).
 *
 * @return    TRUE if the assignment succeeded. FALSE otherwise.
 */
extern int (_impl_vec_assign)(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx, const void *val);

/**
 * @internal
 * @brief   Swaps two elements in a <code>vec</code>
 *
 * @param[in]  vec_ptr  Pointer to a generic <code>vec</code> structure.
 * @param[in]  first    The index of the first element.
 * @param[in]  second   The index of the second element.
 * @param[out] tmp      Pointer to a temporary value to use during the swap.
 *
 * @return     TRUE if the swap suceeded. FALSE otherwise.
 */
extern int (_impl_vec_swap)(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int first, unsigned int second, void *tmp);

/**
 * @internal
 * @brief   Sorts a <code>vec</code>
 *
 * @param[in]  vec_ptr  Pointer to a generic <code>vec</code> structure.
 * @param[in]  cmpfn    Pointer to comparer function.
 * @param[out] tmp      Pointer to a temporary value to use during the sort.
 *
 * @return     TRUE if the sort succeeded. FALSE otherwise.
 */
extern int (_impl_vec_sort)(_IMPL_VEC_STRUCT_NAME *vec_ptr, _IMPL_VEC_CMPFN_NAME cmpfn, void *tmp);

/**
 * @internal
 * @brief   Pushes a value to the end of a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] val       Pointer to the value to push.
 *
 * @return    TRUE if the push succeeded. FALSE otherwise.
 */
extern int (_impl_vec_push)(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val);

/**
 * @internal
 * @brief   Pushes an array of values to the end of a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] val       The array to push.
 * @param[in] len       The amount of elements to push.
 *
 * @return    TRUE if the push succeeded. FALSE otherwise.
 */
extern int (_impl_vec_pusharr)(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val, unsigned int len);

/**
 * @internal
 * @brief   Pops an element from the end of a <code>vec</code>
 *
 * @param[in]  vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[out] out       Optional pointer to store the popped element in.
 *
 * @return    TRUE if the pop succeeded. FALSE otherwise.
 */
extern int (_impl_vec_pop)(_IMPL_VEC_STRUCT_NAME *vec_ptr, void *out);

/**
 * @internal
 * @brief   Pops an array from the end of a <code>vec</code>
 *
 * @param[in]  vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in]  len       The amount of elements to pop.
 * @param[out] out       Optional pointer to store the popped elements in.
 *
 * @return    TRUE if the pop succeeded. FALSE otherwise.
 */
extern int (_impl_vec_poparr)(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int len, void *out);

/**
 * @internal
 * @brief   Inserts an value at a specific index in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] val       Pointer to the value to insert.
 * @param[in] idx       The index at which to insert the value.
 * @param[in] len       The amount of elements to insert.
 *
 * @return    TRUE if the insertion succeeded. FALSE otherwise.
 */
extern int (_impl_vec_insert)(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val, unsigned int idx, unsigned int len);

/**
 * @internal
 * @brief   Removes elements at a specific index from a <code>vec</code>
 *
 * @param[in]  vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in]  idx       The index of the element to remove.
 * @param[in]  len       The amount of elements to remove.
 * @param[out] out       Optional pointer to store the removed elements in.
 *
 * @return    TRUE if the removal succeeded. FALSE otherwise.
 */
extern int (_impl_vec_erase)(_IMPL_VEC_STRUCT_NAME *vec_ptr, unsigned int idx, unsigned int len, void *out);

/**
 * @internal
 * @brief   Clears a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 *
 * @note    This function must be called after there's no more use for
 *          the initialized <code>vec</code>.
 */
extern void (_impl_vec_clear)(_IMPL_VEC_STRUCT_NAME *vec_ptr);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/****************************************************************************************
  External Macro Definitions
 ***************************************************************************************/
#define vec_init(type) _VEC_CAT(_vec_init, type)
#define vec_size(type) _VEC_CAT(_vec_size, type)
#define vec_capacity(type) _VEC_CAT(_vec_capacity, type)
#define vec_empty(type) _VEC_CAT(_vec_empty, type)
#define vec_get(type) _VEC_CAT(_vec_get, type)
#define vec_first(type) _VEC_CAT(_vec_first, type)
#define vec_last(type) _VEC_CAT(_vec_last, type)
#define vec_swap(type) _VEC_CAT(_vec_swap, type)
#define vec_sort(type) _VEC_CAT(_vec_sort, type)
#define vec_pop(type) _VEC_CAT(_vec_pop, type)
#define vec_poparr(type) _VEC_CAT(_vec_poparr, type)
#define vec_unshift(type) _VEC_CAT(_vec_unshift, type)
#define vec_unshiftarr(type) _VEC_CAT(_vec_unshiftarr, type)
#define vec_insert(type) _VEC_CAT(_vec_insert, type)
#define vec_insertptr(type) _VEC_CAT(_vec_insertptr, type)
#define vec_insertarr(type) _VEC_CAT(_vec_insertarr, type)
#define vec_assignptr(type) _VEC_CAT(_vec_assignptr, type)
#define vec_assign(type) _VEC_CAT(_vec_assign, type)
#define vec_push(type) _VEC_CAT(_vec_push, type)
#define vec_pushptr(type) _VEC_CAT(_vec_pushptr, type)
#define vec_pusharr(type) _VEC_CAT(_vec_pusharr, type)
#define vec_shift(type) _VEC_CAT(_vec_shift, type)
#define vec_shiftptr(type) _VEC_CAT(_vec_shiftptr, type)
#define vec_shiftarr(type) _VEC_CAT(_vec_shiftarr, type)
#define vec_erase(type) _VEC_CAT(_vec_erase, type)
#define vec_clear(type) _VEC_CAT(_vec_clear, type)

/**
 * Gets the type name for <code>vec</code> of type <code>type</code>
 *
 * @param[in] type  The type that is stored in the vec.
 */
#define vec_type(type)      _VEC_CAT(_vec, type)

/**
 * Gets the type name of a comparer function pointer for
 * a <code>vec</code> of type <code>type</code>
 *
 * @param[in] type  The type that is stored in the vec.
 */
#define vec_cmp_type(type)  _VEC_CAT(_vec_cmp, vec_type(type))

/**
 * Defines a <code>vec</code> of type <code>type</code>
 * and its associated functionality
 */
#define using_vec_type(type)    \
    _impl_vec_def_struct(type, vec_type(type)); \
    _impl_vec_def_cmp(type, vec_cmp_type(type)); \
    \
    /**
     * @brief   Initializes a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @return    TRUE if the initialization succeeded. FALSE otherwise.
     */ \
    static int (_VEC_CAT(_vec_init, type))(vec_type(type) *vec_ptr) { \
        return (_impl_vec_init)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, sizeof(type)); \
    } \
    /**
     * @brief   Reserves memory in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] capacity  The capacity to reserve.
     *
     * @note      Memory is only allocated if @p capacity exceeds the current
     *            capacity. Otherwise this function does nothing.
     *
     * @return    TRUE if the reservation succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_reserve, type)(vec_type(type) *vec_ptr, unsigned int capacity) { \
        return (_impl_vec_reserve)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, capacity); \
    } \
    /**
     * @brief   Gets the count of elements stored in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @return    The count of elements stored in the <code>vec</code>.
     */ \
    static unsigned int _VEC_CAT(_vec_size, type)(vec_type(type) *vec_ptr) { \
        return (_impl_vec_size)((_IMPL_VEC_STRUCT_NAME *)vec_ptr); \
    } \
    /**
     * @brief   Gets the capacity of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @return    The capacity of the <code>vec</code>.
     */ \
    static unsigned int _VEC_CAT(_vec_capacity, type)(vec_type(type) *vec_ptr) { \
        return (_impl_vec_capacity)((_IMPL_VEC_STRUCT_NAME *)vec_ptr); \
    } \
    /**
     * @brief   Checks if a <code>vec</code> is empty.
     *
     * @return  TRUE if the <code>vec</code> is empty. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_empty, type)(vec_type(type) *vec_ptr) { \
        return (_impl_vec_size)((_IMPL_VEC_STRUCT_NAME *)vec_ptr) == 0; \
    } \
    /**
     * @brief   Gets an element from a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
     * @param[in] idx       The index of the element.
     *
     * @return    Pointer to the element if @p vec_ptr and @p idx are valid.
     *            NULL otherwise.
     */ \
    static type* _VEC_CAT(_vec_get, type)(vec_type(type) *vec_ptr, unsigned int idx) { \
        return (type *)(_impl_vec_get)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, idx); \
    } \
    /**
     * @brief   Gets the first element in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @return    Pointer to the first element if any. NULL otherwise.
     */ \
    static type* _VEC_CAT(_vec_first, type)(vec_type(type) *vec_ptr) { \
        return (type *)(_impl_vec_get)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, 0); \
    } \
    /**
     * @brief   Gets the last element in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @return    Pointer to the last element if any. NULL otherwise.
     */ \
    static type* _VEC_CAT(_vec_last, type)(vec_type(type) *vec_ptr) { \
        return (type *)(_impl_vec_last)((_IMPL_VEC_STRUCT_NAME *)vec_ptr); \
    } \
    /**
     * @brief   Swaps two elements in a <code>vec</code>
     *
     * @param[in]  vec_ptr  Pointer to a <code>vec</code> structure.
     * @param[in]  first    The index of the first element.
     * @param[in]  second   The index of the second element.
     *
     * @return     TRUE if the swap suceeded. FALSE otherwise.
     */ \
    static type* _VEC_CAT(_vec_swap, type)(vec_type(type) *vec_ptr, unsigned int first, unsigned int second) { \
        unsigned char tmp[sizeof(type)]; \
        return (type *)(_impl_vec_swap)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, first, second, tmp); \
    } \
    /**
     * @brief   Sorts a <code>vec</code>
     *
     * @param[in]  vec_ptr  Pointer to a <code>vec</code> structure.
     * @param[in]  cmpfn    Pointer to comparer function for type <code>type</code>.
     *
     * @return    TRUE if the sort succeeded. FALSE otherwise.
     */ \
    static type* _VEC_CAT(_vec_sort, type)(vec_type(type) *vec_ptr, vec_cmp_type(type) cmpfn) { \
        unsigned char tmp[sizeof(type)]; \
        return (type *)(_impl_vec_sort)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, (_IMPL_VEC_CMPFN_NAME)cmpfn, tmp); \
    } \
    /**
     * @brief   Assigns a value passed by a pointer to an element in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] idx       The index to assign the value to.
     * @param[in] val       Pointer to the value to assign.
     *
     * @note      @p idx is valid only if its value is between 0 and the
     *            <code>vec</code>'s size (inclusive).
     *
     * @return    TRUE if the assignment succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_assignptr, type)(vec_type(type) *vec_ptr, unsigned int idx, const type *val) { \
        return (_impl_vec_assign)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, idx, val); \
    } \
    /**
     * @brief   Assigns a value to an element in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] idx       The index to assign the value to.
     * @param[in] val       The value to assign.
     *
     * @note      @p idx is valid only if its value is between 0 and the
     *            <code>vec</code>'s size (inclusive).
     *
     * @return    TRUE if the assignment succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_assign, type)(vec_type(type) *vec_ptr, unsigned int idx, const type val) { \
        return (_impl_vec_assign)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, idx, &val); \
    } \
    /**
     * @brief   Pushes a value passed by a pointer to the end of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       Pointer to the value to push.
     *
     * @return    TRUE if the push succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_pushptr, type)(vec_type(type) *vec_ptr, const type *val) { \
        return (_impl_vec_push)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val); \
    } \
    /**
     * @brief   Pushes an array of values to the end of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       The array to push.
     * @param[in] len       The amount of elements from the array to push.
     *
     * @return    TRUE if the push succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_pusharr, type)(vec_type(type) *vec_ptr, const type *val, unsigned int len) { \
        return (_impl_vec_pusharr)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val, len); \
    } \
    /**
     * @brief   Pushes a value to the end of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       The value to push.
     *
     * @return    TRUE if the push succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_push, type)(vec_type(type) *vec_ptr, const type val) { \
        return (_impl_vec_push)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, &val); \
    } \
    /**
     * @brief   Pops an array of elements from the end of a <code>vec</code>
     *
     * @param[in]  vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in]  len       The amount of elements to pop from the <code>vec</code>.
     * @param[out] out       Optional pointer to store the popped elements in.
     *
     * @return    TRUE if the pop succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_poparr, type)(vec_type(type) *vec_ptr, unsigned int len, type *out) { \
        return (_impl_vec_poparr)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, len, out); \
    } \
    /**
     * @brief   Pops an element from the end of a <code>vec</code>
     *
     * @param[in]  vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[out] out       Optional pointer to store the popped element in.
     *
     * @return    TRUE if the pop succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_pop, type)(vec_type(type) *vec_ptr, type *out) { \
        return (_impl_vec_pop)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, out); \
    } \
    /**
     * @brief   Shifts a value passed by a pointer to the beginning of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       Pointer to the value to shift.
     *
     * @return    TRUE if the shift succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_shiftptr, type)(vec_type(type) *vec_ptr, const type *val) { \
        return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val, 0, 1); \
    } \
    /**
     * @brief   Shifts an array of values to the beginning of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       The array to shift.
     * @param[in] len       The amount of elements to shift.
     *
     * @return    TRUE if the shift succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_shiftarr, type)(vec_type(type) *vec_ptr, const type *val, unsigned int len) { \
        return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val, 0, len); \
    } \
    /**
     * @brief   Shifts a value to the beginning of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       The value to shift.
     *
     * @return    TRUE if the shift succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_shift, type)(vec_type(type) *vec_ptr, const type val) { \
        return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, &val, 0, 1); \
    } \
    /**
     * @brief   Unshifts an array of elements from the beginning of a <code>vec</code>
     *
     * @param[in]  vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in]  len       The amount of elements to unshift.
     * @param[out] out       Optional pointer to store the unshifted elements in.
     *
     * @return    TRUE if the unshift succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_unshiftarr, type)(vec_type(type) *vec_ptr, unsigned int len, type *out) { \
        return (_impl_vec_erase)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, 0, len, out); \
    } \
    /**
     * @brief   Unshifts an element from the beginning of a <code>vec</code>
     *
     * @param[in]  vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[out] out       Optional pointer to store the unshifted element in.
     *
     * @return    TRUE if the unshift succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_unshift, type)(vec_type(type) *vec_ptr, type *out) { \
        return (_impl_vec_erase)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, 0, 1, out); \
    } \
    /**
     * @brief   Inserts an value passed by a pointer at a specific index in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       Pointer to the value to insert.
     * @param[in] idx       The index at which to insert the value.
     *
     * @return    TRUE if the insertion succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_insertptr, type)(vec_type(type) *vec_ptr, const type *val, unsigned int idx) { \
        return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val, idx, 1); \
    } \
    /**
     * @brief   Inserts an array of values at a specific index in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       The array to insert.
     * @param[in] idx       The index at which to insert the value.
     * @param[in] len       The amount of elements to insert.
     *
     * @return    TRUE if the insertion succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_insertarr, type)(vec_type(type) *vec_ptr, const type *val, unsigned int idx, unsigned int len) { \
        return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val, idx, len); \
    } \
    /**
     * @brief   Inserts an value at a specific index in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in] val       The value to insert.
     * @param[in] idx       The index at which to insert the value.
     *
     * @return    TRUE if the insertion succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_insert, type)(vec_type(type) *vec_ptr, const type val, unsigned int idx) { \
        return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, &val, idx, 1); \
    } \
    /**
     * @brief   Removes elements at a specific index from a <code>vec</code>
     *
     * @param[in]  vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in]  idx       The index of the element to remove.
     * @param[in]  len       The amount of elements to remove.
     * @param[out] out       Optional pointer to store the removed elements in.
     *
     * @return    TRUE if the removal succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_erase, type)(vec_type(type) *vec_ptr, unsigned int idx, unsigned int len, type *out) { \
        return (_impl_vec_erase)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, idx, len, out); \
    } \
    /**
     * @brief   Clears a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @note    This function must be called after there's no more use for
     *          the initialized <code>vec</code>.
     */ \
    static void _VEC_CAT(_vec_clear, type)(vec_type(type) *vec_ptr) { \
        (_impl_vec_clear)((_IMPL_VEC_STRUCT_NAME *)vec_ptr); \
    } \
    /*
     * Add a dummy typedef to require insertion of a semicolon after a using_vec_type declaration.
     */ \
    typedef vec_cmp_type(type) *_VEC_CAT(_p, vec_cmp_type(type))

#endif /* !_GENERIC_CVEC_H_ */
