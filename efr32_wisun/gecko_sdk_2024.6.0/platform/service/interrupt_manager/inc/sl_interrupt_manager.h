/***************************************************************************//**
 * @file
 * @brief Interrupt Management API to enable and configure interrupts.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_INTERRUPT_MANAGER_H
#define SL_INTERRUPT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_core.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief sl_interrupt_manager interrupt handler function.
typedef void(*sl_interrupt_manager_irq_handler_t)(void);

/***************************************************************************//**
 * @brief
 *   Initialize interrupt controller hardware and initialise vector table
 *   in RAM.
 ******************************************************************************/
void sl_interrupt_manager_init(void);

/***************************************************************************//**
 * @brief
 *   Reset the cpu core.
 ******************************************************************************/
void sl_interrupt_manager_reset_system(void);

/***************************************************************************//**
 * @brief
 *   Disable interrupts.
 ******************************************************************************/
void sl_interrupt_manager_disable_interrupts(void);

/***************************************************************************//**
 * @brief
 *   Enable interrupts.
 ******************************************************************************/
void sl_interrupt_manager_enable_interrupts(void);

/***************************************************************************//**
 * @brief
 *   Disable interrupt for an interrupt source.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 ******************************************************************************/
void sl_interrupt_manager_disable_irq(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Enable interrupt for an interrupt source.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 ******************************************************************************/
void sl_interrupt_manager_enable_irq(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Check if an interrupt is disabled.
 *
 * @param[in] irqn
 *   The IRQn_Type enumerator for the interrupt to check.
 *
 * @return
 *   True if the interrupt is disabled.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_disabled(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Check if a specific interrupt is blocked.
 *
 * @note
 *   The function return true if the IRQ is disabled.
 *
 * @param[in] irqn
 *   The IRQn_Type enumerator for the interrupt to check.
 *
 * @return
 *   True if the interrupt is disabled or blocked.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_blocked(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Set the interrupt handler of an interrupt source.
 *
 * @note
 *   This function depends on a RAM based interrupt vector table, i.e.
 *   SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM must be true. Or the device
 *   must be Series 3.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @param[in] handler
 *   The new interrupt handler for the interrupt source.
 *
 * @return
 *   The prior interrupt handler for the interrupt source.
 ******************************************************************************/
sl_status_t sl_interrupt_manager_set_irq_handler(int32_t irqn,
                                                 sl_interrupt_manager_irq_handler_t handler);

/***************************************************************************//**
 * @brief
 *   Get the interrupt preemption priority of an interrupt source.
 *
 * @note
 *   The number of priority levels is platform dependent.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @return
 *   The interrupt priority for the interrupt source.
 *   Value 0 denotes the highest priority.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_irq_priority(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Set the interrupt preemption priority of an interrupt source.
 *
 * @note
 *   The number of priority levels is platform dependent.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @param[in] priority
 *   The new interrupt priority for the interrupt source.
 *   Value 0 denotes the highest priority.
 ******************************************************************************/
void sl_interrupt_manager_set_irq_priority(int32_t irqn, uint32_t priority);

/***************************************************************************//**
 * @brief
 *   Pre-interrupt hook.
 *
 * @details
 *   This function is called before each interrupt service routine
 *   when SL_INTERRUPT_MANAGER_HOOKS is enabled.
 *
 * @note
 *   The function is weakly defined, and may be user-defined. By default, the
 *   pre-interrupt hook is empty.
 ******************************************************************************/
void sl_interrupt_manager_irq_enter_hook(void);

/***************************************************************************//**
 * @brief
 *   Register post-interrupt hook.
 *
 * @details
 *   This function is called after each interrupt service routine
 *   when SL_INTERRUPT_MANAGER_HOOKS is enabled.
 *
 * @note
 *   The function is weakly defined, and may be user-defined. By default, the
 *   post-interrupt hook is empty.
 ******************************************************************************/
void sl_interrupt_manager_irq_exit_hook(void);

/***************************************************************************//**
 * @brief
 *   Get the current ISR table.
 *
 * @details
 *   Depending on the configuration of the interrupt manager, this table of
 *   ISRs may be in RAM or in FLASH, and each ISR may or may not be wrapped by
 *   enter/exit hooks.
 *
 * @return
 *   The current ISR table.
 ******************************************************************************/
sl_interrupt_manager_irq_handler_t* sl_interrupt_manager_get_isr_table(void);

#ifdef __cplusplus
}
#endif

#endif /* SL_INTERRUPT_MANAGER_H */
