

#include "mbed.h"
#include "stm32f7xx.h"
#include <STM32F7DMA.hpp>
#include <STM32F7HAL.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <touchgfx/lcd/LCD.hpp>

uint16_t* STM32F7HAL::getTFTFrameBuffer() const
{
    return (uint16_t*)LTDC_Layer1->CFBAR;
}

void STM32F7HAL::setTFTFrameBuffer(uint16_t* adr)
{
    ;
    LTDC_Layer1->CFBAR = (uint32_t)adr;

    /* Reload immediate */
    LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
}

uint16_t STM32F7HAL::getTFTCurrentLine()
{
    // This function only requires an implementation if single buffering
    // on LTDC display is being used (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).

    // The CPSR register (bits 15:0) specify current line of TFT controller.
    uint16_t curr = (uint16_t)(LTDC->CPSR & 0xffff);
    uint16_t backPorchY = (uint16_t)(LTDC->BPCR & 0x7FF) + 1;

    // The semantics of the getTFTCurrentLine() function is to return a value
    // in the range of 0-totalheight. If we are still in back porch area, return 0.
    if (curr < backPorchY) {
        return 0;
    } else {
        return curr - backPorchY;
    }
}

void STM32F7HAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
    HAL::flushFrameBuffer(rect);

    /* To make sure LTDC sees correct data: */
    SCB_CleanInvalidateDCache();
}

void STM32F7HAL::configureInterrupts()
{
    //    printf("STM32F7HAL::configureInterrupts\r\n");
    NVIC_SetPriority(DMA2D_IRQn, 9);
    NVIC_SetPriority(LTDC_IRQn, 9);
    NVIC_SetVector(DMA2D_IRQn, (int32_t)DMA2D_IRQHandler);
    NVIC_SetVector(LTDC_IRQn, (int32_t)LTDC_IRQHandler);
}

static uint16_t lcd_int_active_line;
static uint16_t lcd_int_porch_line;

/* Enable LCD line interrupt, when entering video (active) area */
void STM32F7HAL::enableLCDControllerInterrupt()
{
    lcd_int_active_line = (LTDC->BPCR & 0x7FF) - 1;
    lcd_int_porch_line = (LTDC->AWCR & 0x7FF) - 1;
    LTDC->LIPCR = lcd_int_active_line;
    LTDC->IER = 1;
}

void STM32F7HAL::disableInterrupts()
{
    NVIC_DisableIRQ(LTDC_IRQn);
    NVIC_DisableIRQ(DMA2D_IRQn);
}

void STM32F7HAL::enableInterrupts()
{
    NVIC_EnableIRQ(LTDC_IRQn);
    NVIC_EnableIRQ(DMA2D_IRQn);
}

void __attribute__((interrupt("IRQ"))) STM32F7HAL::LTDC_IRQHandler(void)
{
    if (LTDC->ISR & 1) {
        LTDC->ICR = 1;
        if (LTDC->LIPCR == lcd_int_active_line) {
            //            //entering active area
            LTDC->LIPCR = lcd_int_porch_line;
            HAL::getInstance()->vSync();
            OSWrappers::signalVSync();
            //            // Swap frame buffers immediately instead of waiting for the task to be scheduled in.
            //            // Note: task will also swap when it wakes up, but that operation is guarded and will not have
            //            // any effect if already swapped.
            HAL::getInstance()->swapFrameBuffers();
            GPIO::set(GPIO::VSYNC_FREQ);
        } else {
            //            //exiting active area
            LTDC->LIPCR = lcd_int_active_line;
            //            GPIO::clear(GPIO::VSYNC_FREQ);
            HAL::getInstance()->frontPorchEntered();
        }
    }
}

void __attribute__((interrupt("IRQ"))) STM32F7HAL::DMA2D_IRQHandler(void)
{
    if (DMA2D->ISR & 2) {
        DMA2D->IFCR = 2;
        //invalidate D-Cache after DMA transfer
        SCB_CleanInvalidateDCache();

        touchgfx::HAL::getInstance()->signalDMAInterrupt();
    }
}

extern "C" void __attribute__((interrupt("IRQ"))) LTDC_ER_IRQHandler()
{
    if (LTDC->ISR & 2) {
        LTDC->ICR = 2;
    }
}
