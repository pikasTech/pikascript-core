#include "lamp.h"
#include "PORT_inductor.h"
#include "VM_device_blueTeeth.h"
#include "VM_device_light.h"
#include "VM_device_voltage.h"
#include "VM_device_zigbee.h"
#include "dataMemory.h"
#include "callback.h"
#include "inductor.h"
#include "light.h"
#include "mimiPort_lamp.h"
#include "mimiPort_market.h"
#include "timer.h"

static void deinit(lamp_t *lamp)
{
    DynMemPut(lamp->mem);
    lamp->panel->dinit(lamp->panel);
}

static void update(lamp_t *self, int systime)
{
    // return if lamp is not enable
    if (0 == self->enable)
    {
        return;
    }

    if (0 == systime % 500)
    {
        self->panel->update(self->panel);
    }
    if (0 == systime % 100)
    {
        self->inductor->update(self->inductor, systime);
    }
    if (0 == systime % 50)
    {
        self->key_0->update_port(self->key_0, systime);
        self->key_1->update_port(self->key_1, systime);
        self->key_2->update_port(self->key_2, systime);
    }
    if (0 == systime % 300)
    {
        self->key_0->update_callBack(self->key_0, systime);
        self->key_1->update_callBack(self->key_1, systime);
        self->key_2->update_callBack(self->key_2, systime);
    }

    if (0 == systime % 350)
    {
        self->voltage = self->vol->read_float(self->vol, NULL);
        self->current = self->voltage / 452 * 1000;
    }

    if (0 == systime % 380)
    {
        self->light_val = self->light_sensor->read_float(self->light_sensor, NULL);
        if (self->light_val < 30)
        {
            inductor_t *idt = self->inductor;
            // return if not enable
            if (!idt->enable)
            {
                return;
            }
            idt->open_callback(idt);
            idt->time_opened = systime;
            idt->time_remain = 1000;
        }
    }

    if (0 == systime % 1000)
    {
        device_t *blueTeeth = self->blueTeeth;

        blueTeeth->print(blueTeeth, "light:   %2f\r\n", self->light_val);
        blueTeeth->print(blueTeeth, "voltage: %2f V\r\n", self->voltage);
        blueTeeth->print(blueTeeth, "current: %2f mA\r\n\r\n", self->current);
    }

    if (0 == systime % 1000)
    {
        device_t *zigbee = self->zigbee;

        zigbee->print(zigbee, "light:   %2f\r\n", self->light_val);
        zigbee->print(zigbee, "voltage: %2f V\r\n", self->voltage);
        zigbee->print(zigbee, "current: %2f mA\r\n\r\n", self->current);
    }

    self->timer->update(self->timer, systime);
}

static void refresh_periodic_observer(VMgui_t *gui)
{
    panel_lamp_t *panel = (panel_lamp_t *)(gui->context);
    lamp_t *lamp = (lamp_t *)(panel->context);
    {
        char buff[32];
        sprintf(buff, "L: %1.4f", lamp->light_val);
        gui->_PORT_showLine(gui, 3, 1, buff);
    }
    {
        char buff[32];
        sprintf(buff, "V: %1.4f V", lamp->voltage);
        gui->_PORT_showLine(gui, 4, 1, buff);
    }
    {
        char buff[32];
        sprintf(buff, "I: %1.4f mA", lamp->current);
        gui->_PORT_showLine(gui, 5, 1, buff);
    }
}

static void closeLight_callBack(VMgui_t *gui)
{
    //rewrite in user code
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;
    lamp->light->close(lamp->light);
}

static void openLight_callBack(VMgui_t *gui)
{
    //rewrite in user code
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;
    lamp->light->open(lamp->light);
}

static void enableInductor_callBack(VMgui_t *gui)
{
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;
    lamp->inductor->enable = 1;
}

static void disableInductor_callBack(VMgui_t *gui)
{
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;
    lamp->inductor->enable = 0;
    lamp->light->close(lamp->light);
}

panel_lamp_t *Class_panel_init_lamp(panel_lamp_t *panel, void *context)
{
    panel->refresh_periodic_observer = refresh_periodic_observer;
    panel->openLight_callBack = openLight_callBack;
    panel->closeLight_callBack = closeLight_callBack;
    panel->enableInductor_callBack = enableInductor_callBack;
    panel->disableInudctor_callBack = disableInductor_callBack;
    panel->context = context;
    return panel;
}

static void open_callback(inductor_t *inductor)
{
    lamp_t *lamp = inductor->context;
    lamp->light->open(lamp->light);
}

static void close_callback(inductor_t *inductor)
{
    lamp_t *lamp = inductor->context;
    lamp->light->close(lamp->light);
}

void inductor_init_lamp(inductor_t *inductor, void *context)
{
    Class_inductor_init_PORT(inductor);
    inductor->open_callback = open_callback;
    inductor->close_callback = close_callback;
    inductor->context = context;
}

// callback need to be write in user code.
static void callback_key_0(vkey_t *key, void *arg)
{
    lamp_t *lamp = (lamp_t *)key->context;
    VMgui_t *gui = lamp->panel->gui_main;
    gui->back(gui);
}

// callback need to be write in user code.
static void callback_key_1(vkey_t *key, void *arg)
{
    lamp_t *lamp = (lamp_t *)key->context;
    VMgui_t *gui = lamp->panel->gui_main;
    gui->down(gui);
}

// callback need to be write in user code.
static void callback_key_2(vkey_t *key, void *arg)
{
    lamp_t *lamp = (lamp_t *)key->context;
    VMgui_t *gui = lamp->panel->gui_main;
    gui->enter(gui);
}

static vkey_t *VM_key_init_lamp(int KEY_id, void *context)
{
    vkey_t *key = VM_key_init_PORT(KEY_id);
    switch (KEY_id)
    {
    case 0:
        key->callback = callback_key_0;
        break;
    case 1:
        key->callback = callback_key_1;
        break;
    case 2:
        key->callback = callback_key_2;
        break;
    }
    key->context = context;
    return key;
}

static void _light_timer_handler(callback_t *callback)
{
    mTimer_t *timer = callback->context;
    lamp_t *lamp = timer->context;
    lamp->light->close(lamp->light);
}

static void enable(lamp_t *lamp)
{
    lamp->isEnable = 1;
}

static void gui_clear_line(VMgui_t *gui, int line)
{
    gui->_PORT_showLine(gui, line, 0, "                     ");
}

static void gui_setTime_refresh(VMgui_t *gui)
{
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;

    gui->clear(gui);
    gui->_PORT_showLine(gui, 6, 0, "----------------------");
    gui->_PORT_showLine(gui, 7, 0, "|ADD");
    gui->_PORT_showLine(gui, 7, 0 + 5, "|   DEC ");
    gui->_PORT_showLine(gui, 7, 0 + 5 + 5 + 5, "| OK |");

    char buff[32] = {0};
    sprintf(buff, "time: %d s", (lamp->timer->time_ms / 1000));
    gui_clear_line(gui, 2);
    gui->_PORT_showLine(gui, 2, 2, buff);

    gui->refresh_title(gui);
}

void timerSet_dec(VMgui_t *gui)
{
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;
    lamp->timer->set(lamp->timer, lamp->timer->time_ms + 1000);

    char buff[32] = {0};
    sprintf(buff, "time: %d s", (lamp->timer->time_ms / 1000));
    gui_clear_line(gui, 2);
    gui->_PORT_showLine(gui, 2, 2, buff);
}

void timerSet_add(VMgui_t *gui)
{
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;
    if (lamp->timer->time_ms <= 0)
    {
        return;
    }
    lamp->timer->set(lamp->timer, lamp->timer->time_ms - 1000);

    char buff[32] = {0};
    sprintf(buff, "time: %d s", (lamp->timer->time_ms / 1000));
    gui_clear_line(gui, 2);
    gui->_PORT_showLine(gui, 2, 2, buff);
}

static void timerSet_ok(VMgui_t *gui)
{
    gui->back_callBack(gui);
}

static void disable(lamp_t *lamp)
{
    lamp->isEnable = 0;
}

static void timer_start(VMgui_t *gui)
{
    panel_lamp_t *panel = gui->context;
    lamp_t *lamp = panel->context;
    lamp->light->open(lamp->light);
    lamp->timer->enable(lamp->timer);
}

lamp_t *Class_lamp_init(void)
{
    DMEM *mem = DynMemGet(sizeof(lamp_t));
    lamp_t *lamp = mem->addr;
    lamp->mem = mem;
    lamp->dinit = deinit;

    /* attribute */
    lamp->voltage = 0;
    lamp->current = 0;
    lamp->light_val = 0;

    /* operation */
    lamp->update = update;
    lamp->enable = enable;
    lamp->disable = disable;

    /* objcet */
    lamp->panel = NEW_Class_panel_lamp();
    lamp->panel = Class_panel_init_lamp(lamp->panel, lamp);
    lamp->panel = Class_panel_lamp_init(lamp->panel);

    lamp->panel->gui_timerSet->down = timerSet_add;
    lamp->panel->gui_timerSet->back = timerSet_dec;
    lamp->panel->gui_timerSet->enter = timerSet_ok;
    lamp->panel->gui_timerSet->refresh = gui_setTime_refresh;
    lamp->panel->gui_timing->_option_callBack[1] = timer_start;

    lamp->inductor = Class_inductor_NEW();
    inductor_init_lamp(lamp->inductor, lamp);

    lamp->key_0 = VM_key_init_lamp(0, lamp);
    lamp->key_1 = VM_key_init_lamp(1, lamp);
    lamp->key_2 = VM_key_init_lamp(2, lamp);

    lamp->light = Class_light_init_PORT();

    lamp->timer = Class_timer_init(_light_timer_handler);
    lamp->timer->context = lamp;

    lamp->zigbee = New_device_zigbee(NULL);
    lamp->vol = New_device_voltage(NULL);
    lamp->light_sensor = New_device_light(NULL);
    lamp->blueTeeth = New_device_blueTeeth(NULL);
    return lamp;
}