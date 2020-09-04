/**
  ******************************************************************************
  * File Name          : OSWrappers.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include "mbed.h"
#include "rtos.h"
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>

using namespace touchgfx;

static Semaphore frame_buffer_sem(1, 1);
static Semaphore vsync_sem(0, 1);

void OSWrappers::initialize()
{
}

void OSWrappers::takeFrameBufferSemaphore()
{
    frame_buffer_sem.try_acquire();
}
void OSWrappers::giveFrameBufferSemaphore()
{
    frame_buffer_sem.release();
}

void OSWrappers::tryTakeFrameBufferSemaphore()
{
    frame_buffer_sem.try_acquire_for(0s);
}

void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
    frame_buffer_sem.release();
}

void OSWrappers::signalVSync()
{
    vsync_sem.release();
}

void OSWrappers::waitForVSync()
{
    vsync_sem.try_acquire_for(0s); /*Clear any pending VSYNC first*/
    vsync_sem.try_acquire(); /*Then wait for the next signal*/
}

void OSWrappers::taskDelay(uint16_t ms)
{
    osDelay(ms);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
