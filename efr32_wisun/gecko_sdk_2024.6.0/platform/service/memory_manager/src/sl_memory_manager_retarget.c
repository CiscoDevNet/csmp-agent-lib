/***************************************************************************//**
 * @file
 * @brief Memory Manager Driver's Retarget Implementation.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "sl_memory_manager.h"

#if defined(__GNUC__)
// Wrapping a system function with GCC works by using the linker option '--wrap=symbol'.
// Any undefined reference to "symbol" will be resolved to "__wrap_symbol".
// Any undefined reference to "__real_symbol" will be resolved to "symbol".
// In our case, "__real_symbol" is not really required as the retargeted standard
// C memory functions will call the corresponding Common Memory Manager (CMM)
// native function and not the standard functions again. So it should be seen:
// standard _malloc_r()  -> CMM sl_malloc()
// standard _free_r()    -> CMM sl_free()
// standard _calloc_r()  -> CMM sl_calloc()
// standard _realloc_r() -> CMM sl_realloc()
#define STD_LIB_WRAPPER_MALLOC  __wrap__malloc_r
#define STD_LIB_WRAPPER_FREE    __wrap__free_r
#define STD_LIB_WRAPPER_CALLOC  __wrap__calloc_r
#define STD_LIB_WRAPPER_REALLOC __wrap__realloc_r

// Reentrant parameter.
#define RARG      const struct _reent *reent,
#define VOID_RARG (void) reent

#elif defined(__IAR_SYSTEMS_ICC__)
// Wrapping a system function works with IAR by patching symbol definitions using $Super$$ and $Sub$$
// The $Super$$ special pattern identifies the original unpatched function used for calling
// the original function directly.
// The $Sub$$ special pattern identifies the new function that is called instead of the
// original function.
// In our case, $Super$$ is not really required as the retargeted standard
// C memory functions will call the corresponding Common Memory Manager (CMM)
// native function and not the standard functions again.
//
// NOTE: IAR is not consistent in the usage of $Sub$$ and the standard C memory functions names
//       to be wrapped. It was observed depending on the IAR version that the $Sub$$ usage can be:
//       - $Sub$$<function>
//       - $Sub$$__iar_dl<function> (e.g. v8.50.9)
//       - $Sub$$__basic_<function> (e.g. v9.40.1)
//       Thus the macros below are fine-tuned according to a given IAR version.
#if (__VER__ == 8050009)
#define STD_LIB_WRAPPER_MALLOC  $Sub$$__iar_dlmalloc
#define STD_LIB_WRAPPER_FREE    $Sub$$__iar_dlfree
#define STD_LIB_WRAPPER_CALLOC  $Sub$$__iar_dlcalloc
#define STD_LIB_WRAPPER_REALLOC $Sub$$__iar_dlrealloc
#elif (__VER__ == 9040001)
#define STD_LIB_WRAPPER_MALLOC  $Sub$$__basic_malloc
#define STD_LIB_WRAPPER_FREE    $Sub$$__basic_free
#define STD_LIB_WRAPPER_CALLOC  $Sub$$__basic_calloc
#define STD_LIB_WRAPPER_REALLOC $Sub$$__basic_realloc
#else
#error Unsupported IAR compiler version for standard C memory functions retarget
#endif

// Since IAR does not use reentrant functions, resolve reentrant parameter to nothing.
#define RARG
#define VOID_RARG

#else
#error Unsupported compiler for standard C memory functions retarget
#endif

#if defined(TEST_MEMORY_MANAGER_RETARGET_PRESENT)
uint32_t retarget_malloc_counter = 0;
uint32_t retarget_free_counter = 0;
uint32_t retarget_calloc_counter = 0;
uint32_t retarget_realloc_counter = 0;
#endif

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * malloc() wrapper. Allocates a memory block of at least requested size from
 * the heap.
 *
 * @param[in] size  Size of the block, in bytes.
 *
 * @return    Pointer to allocated block if successful. Null pointer if
 *            allocation failed.
 *
 * @note Requesting a block of 0 byte will return a null pointer.
 *
 * @note All allocated blocks using this function will be considered long-term
 *       allocations.
 ******************************************************************************/
void *STD_LIB_WRAPPER_MALLOC(RARG
                             size_t size)
{
  VOID_RARG;
  void *ptr;

  ptr = sl_malloc(size);
#if defined(TEST_MEMORY_MANAGER_RETARGET_PRESENT)
  retarget_malloc_counter++;
#endif

  return ptr;
}

/***************************************************************************//**
 * free() wrapper. Frees a previously allocated block back into the heap.
 *
 * @param[in] ptr   Pointer to memory block to be freed.
 *
 * @note Passing a null pointer does nothing.
 ******************************************************************************/
void STD_LIB_WRAPPER_FREE(RARG
                          void *ptr)
{
  VOID_RARG;
  sl_free(ptr);
#if defined(TEST_MEMORY_MANAGER_RETARGET_PRESENT)
  retarget_free_counter++;
#endif
}

/***************************************************************************//**
 * calloc() wrapper. Dynamically allocates a block of memory cleared to 0.
 *
 * @param[in] item_count  Number of elements to be allocated.
 * @param[in] size        Size of each elements, in bytes.
 *
 * @return Pointer to allocated block if successful. Null pointer if
 *         allocation failed.
 *
 * @note All allocated blocks using this function will be considered long-term
 *       allocations.
 ******************************************************************************/
void *STD_LIB_WRAPPER_CALLOC(RARG
                             size_t item_count,
                             size_t size)
{
  VOID_RARG;
  void *ptr;

  ptr = sl_calloc(item_count, size);
#if defined(TEST_MEMORY_MANAGER_RETARGET_PRESENT)
  retarget_calloc_counter++;
#endif

  return ptr;
}

/***************************************************************************//**
 * realloc() wrapper. Resize a previously allocated memory block.
 *
 * @param[in] ptr   Pointer to the allocation to resize. If NULL, behavior
 *                  is same as sl_malloc(), sl_memory_alloc().
 * @param[in] size  New size of the block, in bytes. If 0, behavior is same as
 *                  sl_free(), sl_memory_free().
 *
 * @return Pointer to newly allocated block, if successful. Null pointer if
 *         re-allocation failed.
 *
 * @note All re-allocated blocks using this function will be considered
 *       long-term allocations.
 *
 * @note 'ptr' NULL and 'size' of 0 bytes is an incorrect parameters
 *       combination. No reallocation will be done by the function as it is
 *       an error condition.
 *
 * @note If the new 'size' is the same as the old, the function changes nothing
 *       and returns the same provided address 'ptr'.
 ******************************************************************************/
void *STD_LIB_WRAPPER_REALLOC(RARG
                              void *ptr,
                              size_t size)
{
  VOID_RARG;
  void *r_ptr;

  r_ptr = sl_realloc(ptr, size);
#if defined(TEST_MEMORY_MANAGER_RETARGET_PRESENT)
  retarget_realloc_counter++;
#endif

  return r_ptr;
}
