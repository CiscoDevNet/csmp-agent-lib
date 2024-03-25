/***************************************************************************//**
 * @file
 * @brief Getters for Heap and stack
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
#include <stdint.h>
#include "em_device.h"
#include "sl_memory_manager_region.h"
#include "sl_memory_manager_region_config.h"
#include "sl_component_catalog.h"

#define IAR_HEAP_BLOCK_NAME      "MEMORY_MANAGER_HEAP"

/* Prevent's compilation errors when building in simulation */
#ifndef   __USED
  #define __USED
#endif

#if defined(__GNUC__)
/* Declare stack object used with gcc */
static char sl_stack[SL_STACK_SIZE] __attribute__ ((aligned(8), used, section(".stack")));

/*
 * Declare the base and limit of the full heap region used with gcc. To make
 * use of otherwise unused memory, the total heap region be larger than the
 * minimum heap allocation above.
 */
extern char __HeapBase[];
extern char __HeapLimit[];

#elif defined(__ICCARM__)
/* Declare stack object used with iar */
__root char sl_stack[SL_STACK_SIZE] @ ".stack";

  #pragma section=IAR_HEAP_BLOCK_NAME

#endif

sl_memory_region_t sl_memory_get_stack_region(void)
{
  sl_memory_region_t region;

  region.addr = &sl_stack;
  region.size = SL_STACK_SIZE;
  return region;
}

sl_memory_region_t sl_memory_get_heap_region(void)
{
  sl_memory_region_t region;

  /*
   * Report the actual heap region which may be larger then the minimum
   * allocation of SL_HEAP_SIZE bytes
   */
#if defined(__GNUC__)
  region.addr = __HeapBase;
  region.size = (size_t) ((uintptr_t) __HeapLimit - (uintptr_t) __HeapBase);

#elif defined(__ICCARM__)
  region.addr = __section_begin(IAR_HEAP_BLOCK_NAME);
  region.size = __section_size(IAR_HEAP_BLOCK_NAME);
#endif

  return region;
}

#if defined(__GNUC__)
__USED void * _sbrk(int incr)
{
#if defined(SL_CATALOG_MEMORY_MANAGER_PRESENT)
  (void)incr;

  /*
   * This means there is an issue with the setup of C standard library.
   */
  while (1) {
    // infinite loop;
  }
#else
  static char *heap_end = __HeapBase;
  char *prev_heap_end;

  if ((heap_end + incr) > __HeapLimit) {
    // Not enough heap
    return (void *) -1;
  }

  prev_heap_end = heap_end;
  heap_end += incr;

  return prev_heap_end;
#endif
}
#endif
