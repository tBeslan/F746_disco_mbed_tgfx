#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/HAL.hpp>

Thread* gui_thread;

void guiThread(void)
{
    touchgfx::hw_init();
    touchgfx::touchgfx_init();
    touchgfx::HAL::getInstance()->taskEntry();
}

int main()
{
    gui_thread = new Thread(osPriorityNormal, 8 * 1024, NULL, "GUI Thread");
    gui_thread->start(guiThread);

    while (1) {
        ThisThread::sleep_for(1s);
    }

    return 0;
}
