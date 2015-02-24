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

#ifndef _GENERIC_CVEC_H_
#define _GENERIC_CVEC_H_

/****************************************************************************************
  Macro Definitions
 ***************************************************************************************/
/**
 * @internal
 * Defines NULL in case it's not defined already
 */
#ifndef NULL
#	define NULL 0
#endif

/**
 * @internal
 * Defines FALSE in case it's not defined already
 */
#ifndef FALSE
#	define FALSE 0
#endif

/**
 * @internal
 * Defines TRUE in case it's not defined already
 */
#ifndef TRUE
#	define TRUE (!FALSE)
#endif

/**
 * @internal
 * Defines the unsigned int type used troughout the <code>vec</code> library
 */
typedef unsigned int uint;

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

/**
 * @internal
 * Defines a vec struct of type <code>type</code>
 * with the name <code>name</code>
 */
#define _impl_vec_def_struct(type, name) \
typedef struct \
{ \
	uint start; \
	uint size; \
	uint capacity; \
	uint _t_size; \
	type *_mem; \
} name

/****************************************************************************************
  Type Definitions
 ***************************************************************************************/
/**
 * @internal
 * Defines the generic vec structure
 */
_impl_vec_def_struct(unsigned char, _IMPL_VEC_STRUCT_NAME);

/****************************************************************************************
  Function Declarations
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
extern int (_impl_vec_init)(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint t_size);

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
extern int (_impl_vec_reserve)(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint capacity);

/**
 * @internal
 * @brief   Gets the count of elements stored in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 *
 * @return    The count of elements stored in the <code>vec</code>.
 */
extern uint (_impl_vec_size)(const _IMPL_VEC_STRUCT_NAME *vec_ptr);

/**
 * @internal
 * @brief   Gets the capacity of a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 *
 * @return    The capacity of the <code>vec</code>.
 */
extern uint (_impl_vec_capacity)(const _IMPL_VEC_STRUCT_NAME *vec_ptr);

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
extern void* (_impl_vec_get)(const _IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx);

/**
 * @internal
 * @brief   Gets the first element in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 *
 * @return    Pointer to the first element if any. NULL otherwise.
 */
extern void* (_impl_vec_first)(const _IMPL_VEC_STRUCT_NAME *vec_ptr);

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
extern int (_impl_vec_assign)(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx, const void *val);

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
 * @brief   Shifts a value to the beginning of a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] val       Pointer to the value to shift.
 *
 * @return    TRUE if the shift succeeded. FALSE otherwise.
 */
extern int (_impl_vec_shift)(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val);

/**
 * @internal
 * @brief   Unshifts an element from the beginning of a <code>vec</code>
 *
 * @param[in]  vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[out] out       Optional pointer to store the popped element in.
 *
 * @return    TRUE if the unshift succeeded. FALSE otherwise.
 */
extern int (_impl_vec_unshift)(_IMPL_VEC_STRUCT_NAME *vec_ptr, void *out);

/**
 * @internal
 * @brief   Inserts an value at a specific index in a <code>vec</code>
 *
 * @param[in] vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in] val       Pointer to the value to insert.
 * @param[in] idx       The index at which to insert the value.
 *
 * @return    TRUE if the insertion succeeded. FALSE otherwise.
 */
extern int (_impl_vec_insert)(_IMPL_VEC_STRUCT_NAME *vec_ptr, const void *val, uint idx);

/**
 * @internal
 * @brief   Removes an element at a specific index from a <code>vec</code>
 *
 * @param[in]  vec_ptr   Pointer to a generic <code>vec</code> structure.
 * @param[in]  idx       The index of the element to remove.
 * @param[out] out       Optional pointer to store the removed element in.
 *
 * @return    TRUE if the removal succeeded. FALSE otherwise.
 */
extern int (_impl_vec_erase)(_IMPL_VEC_STRUCT_NAME *vec_ptr, uint idx, void *out);

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

#endif /* !_GENERIC_CVEC_H_ */

/****************************************************************************************
  External Declarations
 ***************************************************************************************/
#ifdef vec_init
#   undef vec_init
#endif
#define vec_init(type) _VEC_CAT(_vec_init, type)

#ifdef vec_capacity
#   undef vec_capacity
#endif
#define vec_capacity(type) _VEC_CAT(_vec_capacity, type)

#ifdef vec_size
#   undef vec_size
#endif
#define vec_size(type) _VEC_CAT(_vec_size, type)

#ifdef vec_empty
#   undef vec_empty
#endif
#define vec_empty(type) _VEC_CAT(_vec_empty, type)

#ifdef vec_get
#   undef vec_get
#endif
#define vec_get(type) _VEC_CAT(_vec_get, type)

#ifdef vec_first
#   undef vec_first
#endif
#define vec_first(type) _VEC_CAT(_vec_first, type)

#ifdef vec_last
#   undef vec_last
#endif
#define vec_last(type) _VEC_CAT(_vec_last, type)

#ifdef vec_pop
#   undef vec_pop
#endif
#define vec_pop(type) _VEC_CAT(_vec_pop, type)

#ifdef vec_unshift
#   undef vec_unshift
#endif
#define vec_unshift(type) _VEC_CAT(_vec_unshift, type)

#ifdef vec_insert
#   undef vec_insert
#endif
#define vec_insert(type) _VEC_CAT(_vec_insert, type)

#ifdef vec_insertptr
#   undef vec_insertptr
#endif
#define vec_insertptr(type) _VEC_CAT(_vec_insertptr, type)

#ifdef vec_assignptr
#   undef vec_assignptr
#endif
#define vec_assignptr(type) _VEC_CAT(_vec_assignptr, type)

#ifdef vec_assign
#   undef vec_assign
#endif
#define vec_assign(type) _VEC_CAT(_vec_assign, type)

#ifdef vec_pushptr
#   undef vec_pushptr
#endif
#define vec_pushptr(type) _VEC_CAT(_vec_pushptr, type)

#ifdef vec_push
#   undef vec_push
#endif
#define vec_push(type) _VEC_CAT(_vec_push, type)

#ifdef vec_shiftptr
#   undef vec_shiftptr
#endif
#define vec_shiftptr(type) _VEC_CAT(_vec_shiftptr, type)

#ifdef vec_shift
#   undef vec_shift
#endif
#define vec_shift(type) _VEC_CAT(_vec_shift, type)

#ifdef vec_erase
#   undef vec_erase
#endif
#define vec_erase(type) _VEC_CAT(_vec_erase, type)

#ifdef vec_clear
#   undef vec_clear
#endif
#define vec_clear(type) _VEC_CAT(_vec_clear, type)

/**
 * Gets the type name for vec of type <code>type</code>
 *
 * @param[in] type  The type that is stored in the vec.
 */
#ifdef vec_type
#   undef vec_type
#endif
#define vec_type(type) _VEC_CAT(_vec, type)

/**
 * Defines a <code>vec</code> of type <code>type</code>
 * and its associated functionality
 */
#ifdef using_vec_type
#	undef using_vec_type
#endif
#define using_vec_type(type)	\
	_impl_vec_def_struct(type, vec_type(type)); \
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
	static int _VEC_CAT(_vec_reserve, type)(vec_type(type) *vec_ptr, uint capacity) { \
		return (_impl_vec_reserve)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, capacity); \
	} \
    /**
     * @brief   Gets the count of elements stored in a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @return    The count of elements stored in the <code>vec</code>.
     */ \
	static uint _VEC_CAT(_vec_size, type)(vec_type(type) *vec_ptr) { \
		return (_impl_vec_size)((_IMPL_VEC_STRUCT_NAME *)vec_ptr); \
	} \
    /**
     * @brief   Gets the capacity of a <code>vec</code>
     *
     * @param[in] vec_ptr   Pointer to a <code>vec</code> structure.
     *
     * @return    The capacity of the <code>vec</code>.
     */ \
	static uint _VEC_CAT(_vec_capacity, type)(vec_type(type) *vec_ptr) { \
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
	static type* _VEC_CAT(_vec_get, type)(vec_type(type) *vec_ptr, uint idx) { \
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
		return (type *)(_impl_vec_first)((_IMPL_VEC_STRUCT_NAME *)vec_ptr); \
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
	static int _VEC_CAT(_vec_assignptr, type)(vec_type(type) *vec_ptr, uint idx, const type *val) { \
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
	static int _VEC_CAT(_vec_assign, type)(vec_type(type) *vec_ptr, uint idx, const type val) { \
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
		return (_impl_vec_shift)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val); \
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
		return (_impl_vec_shift)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, &val); \
	} \
    /**
     * @brief   Unshifts an element from the beginning of a <code>vec</code>
     *
     * @param[in]  vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[out] out       Optional pointer to store the popped element in.
     *
     * @return    TRUE if the unshift succeeded. FALSE otherwise.
     */ \
	static int _VEC_CAT(_vec_unshift, type)(vec_type(type) *vec_ptr, type *out) { \
		return (_impl_vec_unshift)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, out); \
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
	static int _VEC_CAT(_vec_insertptr, type)(vec_type(type) *vec_ptr, const type *val, uint idx) { \
		return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, val, idx); \
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
	static int _VEC_CAT(_vec_insert, type)(vec_type(type) *vec_ptr, const type val, uint idx) { \
		return (_impl_vec_insert)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, &val, idx); \
	} \
    /**
     * @brief   Removes an element at a specific index from a <code>vec</code>
     *
     * @param[in]  vec_ptr   Pointer to a <code>vec</code> structure.
     * @param[in]  idx       The index of the element to remove.
     * @param[out] out       Optional pointer to store the removed element in.
     *
     * @return    TRUE if the removal succeeded. FALSE otherwise.
     */ \
    static int _VEC_CAT(_vec_erase, type)(vec_type(type) *vec_ptr, uint idx, type *out) { \
        return (_impl_vec_erase)((_IMPL_VEC_STRUCT_NAME *)vec_ptr, idx, out); \
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
	}
