/***************************************************************************//**
 * @file
 * @brief Memory Manager Driver API definition.
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

#ifndef SL_MEMORY_MANAGER_H_
#define SL_MEMORY_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "sl_status.h"
#include "sl_core.h"
#include "sl_memory_manager_region.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @addtogroup sl_malloc Memory Heap Allocator API
 * @brief Memory Heap Allocator Driver API.
 * @{
 *****************************************************************************/

/*******************************************************************************
 *********************************   DEFINES   *********************************
 ******************************************************************************/

/// Special value to indicate the default block alignment to the allocator.
/// 8 bytes is the minimum alignment to account for largest CPU data type
/// that can be used in some block allocation scenarios.
#define SL_MEMORY_BLOCK_ALIGN_DEFAULT     0xFFFFFFFFU

#define SL_MEMORY_BLOCK_ALIGN_8_BYTES     8U
#define SL_MEMORY_BLOCK_ALIGN_16_BYTES    16U
#define SL_MEMORY_BLOCK_ALIGN_32_BYTES    32U
#define SL_MEMORY_BLOCK_ALIGN_64_BYTES    64U
#define SL_MEMORY_BLOCK_ALIGN_128_BYTES   128U
#define SL_MEMORY_BLOCK_ALIGN_256_BYTES   256U
#define SL_MEMORY_BLOCK_ALIGN_512_BYTES   512U

/*******************************************************************************
 ******************************  DATA TYPES   **********************************
 ******************************************************************************/

/// @brief Block type.
typedef enum {
  BLOCK_TYPE_LONG_TERM = 0,   ///< Long-term block type.
  BLOCK_TYPE_SHORT_TERM = 1   ///< Short-term block type.
} sl_memory_block_type_t;

/// @brief General purpose heap information.
typedef struct {
  uint32_t base_addr;               ///< Heap base address.
  size_t used_size;                 ///< Used size (in bytes), independently of alignment.
  size_t free_size;                 ///< Free size (in bytes), independently of alignment.
  size_t total_size;                ///< Total heap size (in bytes).
  size_t free_block_count;          ///< Number of free blocks.
  size_t free_block_largest_size;   ///< Largest free block size (in bytes).
  size_t free_block_smallest_size;  ///< Smallest free block size (in bytes).
  size_t used_block_count;          ///< Number of used blocks.
  size_t used_block_largest_size;   ///< Largest used block size (in bytes).
  size_t used_block_smallest_size;  ///< Smallest used block size (in bytes).
} sl_memory_heap_info_t;

/// @brief Memory block reservation handle.
typedef struct {
  void *block_address;                 ///< Reserved block base address.
  uint32_t block_size;                 ///< Reserved block size (in bytes).
} sl_memory_reservation_t;

/// @brief Memory pool handle.
typedef struct {
#if !defined(SL_MEMORY_POOL_LIGHT)
  sl_memory_reservation_t *reservation; ///< Pointer to reservation handle.
#else
  void *block_address;                 ///< Reserved block base address.
#endif
  uint32_t *block_free;                 ///< Pointer to pool's free blocks list.
  size_t block_count;                   ///< Max quantity of blocks in the pool.
  size_t block_size;                    ///< Size of each block.
} sl_memory_pool_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * Initializes the memory manager.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_init(void);

/***************************************************************************//**
 * Reserves a memory block that will never need retention in EM2.
 *
 * @param[in]  size   Size of the block, in bytes.
 * @param[in]  align  Required alignment for the block, in bytes.
 * @param[out] block  Pointer to variable that will receive the start address of the
 *                    allocated block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Required alignment of memory block (in bytes) MUST be a power of 2
 *        and can range from 1 to 512 bytes.
 *        The define SL_MALLOC_BUF_ALIGN_DEFAULT can be specified to select
 *        the default alignment.
 ******************************************************************************/
sl_status_t sl_memory_reserve_no_retention(size_t size,
                                           size_t align,
                                           void **block);

/***************************************************************************//**
 * Allocates a memory block of at least requested size from the heap. Simple
 * version.
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
void *sl_malloc(size_t size);

/***************************************************************************//**
 * Dynamically allocates a block of memory.
 *
 * @param[in]  size   Size of the block, in bytes.
 * @param[in]  type   Type of block (long-term or short-term).
 *                      BLOCK_TYPE_LONG_TERM
 *                      BLOCK_TYPE_SHORT_TERM
 * @param[out] block  Pointer to variable that will receive the start address
 *                    of the allocated block.
 *
 * @return     SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_alloc(size_t size,
                            sl_memory_block_type_t type,
                            void **block);

/***************************************************************************//**
 * Dynamically allocates a block of memory. Advanced version that allows to
 * specify alignment.
 *
 * @param[in]  size     Size of the block, in bytes.
 * @param[in]  align    Required alignment for the block, in bytes.
 * @param[in]  type     Type of block (long-term or short term).
 *                        BLOCK_TYPE_LONG_TERM
 *                        BLOCK_TYPE_SHORT_TERM
 * @param[out] block    Pointer to variable that will receive the start address
 *                      of the allocated block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Required alignment of memory block (in bytes) MUST be a power of 2
 *        and can range from 1 to 512 bytes.
 *        The define SL_MALLOC_BUF_ALIGN_DEFAULT can be specified to select
 *        the default alignment.
 ******************************************************************************/
sl_status_t sl_memory_alloc_advanced(size_t size,
                                     size_t align,
                                     sl_memory_block_type_t type,
                                     void **block);

/***************************************************************************//**
 * Frees a previously allocated block back into the heap. Simple version.
 *
 * @param[in] ptr   Pointer to memory block to be freed.
 *
 * @note Passing a null pointer does nothing.
 ******************************************************************************/
void sl_free(void *ptr);

/***************************************************************************//**
 * Frees a dynamically allocated block of memory.
 *
 * @param[in] block   Pointer to the block that must be freed.
 *
 * @return    SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_free(void *block);

/***************************************************************************//**
 * Dynamically allocates a block of memory cleared to 0. Simple version.
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
void *sl_calloc(size_t item_count,
                size_t size);

/***************************************************************************//**
 * Dynamically allocates a block of memory cleared to 0.
 *
 * @param[in]  item_count   Number of elements to be allocated.
 * @param[in]  size         Size of each elements, in bytes.
 * @param[in]  type         Type of block (long-term or short-term).
 *                            BLOCK_TYPE_LONG_TERM
 *                            BLOCK_TYPE_SHORT_TERM
 * @param[out] block        Pointer to variable that will receive the start
 *                          address of the allocated block.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_calloc(size_t item_count,
                             size_t size,
                             sl_memory_block_type_t type,
                             void **block);

/***************************************************************************//**
 * Resize a previously allocated memory block. Simple version.
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
void *sl_realloc(void *ptr,
                 size_t size);

/***************************************************************************//**
 * Resize a previously allocated memory block.
 *
 * @param[in]  ptr   Pointer to the allocation to resize. If NULL, behavior
 *                   is same as sl_malloc(), sl_memory_alloc().
 * @param[in]  size  New size of the block, in bytes. If 0, behavior is same as
 *                   sl_free(), sl_memory_free().
 * @param[out] block Pointer to variable that will receive the start address of
 *                   the new allocated memory.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note All re-allocated blocks using this function will be considered
 *       long-term allocations.
 *
 * @note If the new 'size' is the same as the old, the function changes nothing
 *       and returns the same provided address 'ptr'.
 ******************************************************************************/
sl_status_t sl_memory_realloc(void *ptr,
                              size_t size,
                              void **block);

/***************************************************************************//**
 * Populates an sl_memory_heap_info_t structure with the current status of the heap.
 *
 * @param[in] heap_info Pointer to structure that will receive further heap
 *                      information data.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_get_heap_info(sl_memory_heap_info_t *heap_info);

/***************************************************************************//**
 * Dynamically reserves a block of memory.
 *
 * @param[in]  size    Size of the block, in bytes.
 * @param[in]  align   Required alignment for the block, in bytes.
 * @param[in] handle   Handle to the reserved block.
 * @param[out] block   Pointer to variable that will receive the start address
 *                     of the allocated block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Required alignment of memory block (in bytes) MUST be a power of 2
 *        and can range from 1 to 512 bytes.
 *        The define SL_MALLOC_BUF_ALIGN_DEFAULT can be specified to select
 *        the default alignment.
 ******************************************************************************/
sl_status_t sl_memory_reserve_block(size_t size,
                                    size_t align,
                                    sl_memory_reservation_t *handle,
                                    void **block);

/***************************************************************************//**
 * Frees a reserved block of memory.
 *
 * @param[in] handle  Handle to the reserved block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_release_block(sl_memory_reservation_t *handle);

/***************************************************************************//**
 * Dynamically allocates a block reservation handle.
 *
 * @param[out] handle  Handle.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_reservation_handle_alloc(sl_memory_reservation_t **handle);

/***************************************************************************//**
 * Frees a dynamically allocated block reservation handle.
 *
 * @param[in] handle  Handle.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_reservation_handle_free(sl_memory_reservation_t *handle);

/***************************************************************************//**
 * Gets the size of the memory reservation handle structure.
 *
 * @return  Memory reservation handle structure's size.
 ******************************************************************************/
uint32_t sl_memory_reservation_handle_get_size(void);

/***************************************************************************//**
 * Creates a memory pool.
 *
 * @param[in] block_size    Size of each block, in bytes.
 * @param[in] block_count   Number of blocks in the pool.
 * @param[in] pool_handle   Handle to the memory pool.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_create_pool(size_t block_size,
                                  uint32_t block_count,
                                  sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Deletes a memory pool.
 *
 * @param[in] pool_handle Handle to the memory pool.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note All pool allocations need to be freed by calling sl_memory_pool_free()
 *       on each block before calling sl_memory_delete_pool().
 *
 * @note The pool_handle provided is neither freed or invalidated. It can be
 *       reused in a new call to sl_memory_create_pool() to create another pool.
 ******************************************************************************/
sl_status_t sl_memory_delete_pool(sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Allocates a block from a memory pool.
 *
 * @param[in]  pool_handle Handle to the memory pool.
 * @param[out] block       Pointer to a variable that will receive the address
 *                         of the allocated block.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_alloc(sl_memory_pool_t *pool_handle,
                                 void **block);

/***************************************************************************//**
 * Frees a block from a memory pool.
 *
 * @param[in] pool_handle Handle to the memory pool.
 * @param[in] block       Pointer to the block to free.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_free(sl_memory_pool_t *pool_handle,
                                void *block);

/***************************************************************************//**
 * Dynamically allocates a memory pool handle.
 *
 * @param[out] handle  Handle.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_handle_alloc(sl_memory_pool_t **pool_handle);

/***************************************************************************//**
 * Frees a dynamically allocated memory pool handle.
 *
 * @param[in] handle  Handle.
 *
 * @return  SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_memory_pool_handle_free(sl_memory_pool_t *pool_handle);

/***************************************************************************//**
 * Gets the size of the memory reservation handle structure.
 *
 * @return  Memory reservation handle structure's size.
 ******************************************************************************/
uint32_t sl_memory_pool_handle_get_size(void);

/***************************************************************************//**
 * Gets the size of the heap.
 *
 * @return  Heap's size in bytes.
 ******************************************************************************/
size_t sl_memory_get_total_heap_size(void);

/***************************************************************************//**
 * Gets the current free heap size.
 *
 * @return  Free heap size in bytes.
 ******************************************************************************/
size_t sl_memory_get_free_heap_size(void);

/***************************************************************************//**
 * Gets the current used heap size.
 *
 * @return  Used heap size in bytes.
 ******************************************************************************/
size_t sl_memory_get_used_heap_size(void);

/***************************************************************************//**
 * Gets heap high watermark.
 *
 * @return  Highest heap usage in bytes recorded.
 ******************************************************************************/
size_t sl_memory_get_heap_high_watermark(void);

/***************************************************************************//**
 * Reset heap high watermark to the current heap used.
 ******************************************************************************/
void sl_memory_reset_heap_high_watermark(void);

/** @} (end addtogroup sl_memory_manager) */

#ifdef __cplusplus
}
#endif

#endif /* SL_MEMORY_MANAGER_H_ */
