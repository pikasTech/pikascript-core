#include "market_case1.h"
#include "VM_delay.h"
#include "VM_key.h"
#include "VM_memory.h"
#include "VM_socket.h"
#include "mimiPort_market.h"
#include "mimiSH_config.h"
#include "mimiSH_core.h"
#include "mimiStr.h"
#include "panel_market.h"
#include <stdlib.h>
#include <string.h>

static void gui_goosNum_refuresh(gui_t *gui);

extern sh_t *Pub_Sh;

static void deinit(market_case1_t *market_case1)
{
    DynMemPut(market_case1->mem);
    market_case1->panel->dinit(market_case1->panel);
}

static void gui_clear_line(gui_t *gui, int line)
{
    gui->showLine(gui, line, 0, "                     ");
}

goods_t *Class_goods_init_market(void *field_market, char *goods_name, int price)
{
    goods_t *goods = Class_goods_init();
    goods->feild = field_market;
    goods->setName(goods, goods_name);
    goods->price = price;
    return goods;
}

static void choseGoodsAndChange_callBack(void *context)
{
    // the the context
    panel_market_t *panel = (panel_market_t *)context;
    market_case1_t *market = (market_case1_t *)panel->context;
    gui_t *gui = panel->gui_main;
    int goods_ID = gui->option_pointer;
    goods_t *goods_main;
    switch (goods_ID)
    {
    case 0:
        goods_main = market->goods1;
        break;
    case 1:
        goods_main = market->goods2;
        break;
    case 2:
        goods_main = market->goods3;
        break;
    case 3:
        goods_main = market->goods4;
        break;
    case 4:
        goods_main = market->goods5;
    default:
        break;
    }
    market->goods_main = goods_main;

    char buff_set[256] = {0};
    char buff_buy[256] = {0};

    sprintf(buff_set, "Setting %s", goods_main->name);
    sprintf(buff_buy, "Buy %s", goods_main->name);

    panel->gui_goods_set->setTitle(panel->gui_goods_set, buff_set);
    panel->gui_goods_set->setTitle(panel->gui_goods_shopping, buff_buy);

    guiChange_callBack(context);
}

static void gui_BuyGoods_refresh(gui_t *gui)
{
    gui->clear(gui);
    gui->showLine(gui, 6, 0, "----------------------");
    gui->showLine(gui, 7, 0, "|ADD");
    gui->showLine(gui, 7, 0 + 5, "|   DEC ");
    gui->showLine(gui, 7, 0 + 5 + 5 + 5, "| OK |");

    gui->title_refresh(gui);
    gui_goosNum_refuresh(gui);
}

static void gui_setGoodsPrice_refresh(gui_t *gui)
{
    panel_market_t *panel = (panel_market_t *)gui->context;
    market_case1_t *market = (market_case1_t *)panel->context;
    gui->clear(gui);
    gui->showLine(gui, 6, 0, "----------------------");
    gui->showLine(gui, 7, 0, "|ADD");
    gui->showLine(gui, 7, 0 + 5, "|   DEC ");
    gui->showLine(gui, 7, 0 + 5 + 5 + 5, "| OK |");

    char buff[32] = {0};
    sprintf(buff, "Prece: %d CNY", market->goods_main->price);
    gui_clear_line(gui, 2);
    gui->showLine(gui, 2, 2, buff);

    gui->option_pointer_refrash(gui);
    gui->option_str_refrash(gui);
    gui->title_refresh(gui);
}
static int get_totle(market_case1_t *market)
{
    int totle = 0;
    int price = 0;
    int buy_number = 0;

    goods_t *goods;

    for (int i = 0; i < 5; i++)
    {
        switch (i)
        {
        case 0:
            goods = market->goods1;
            break;

        case 1:
            goods = market->goods2;
            break;
        case 2:
            goods = market->goods3;
            break;
        case 3:
            goods = market->goods4;
            break;
        case 4:
            goods = market->goods5;
            break;
        default:
            break;
        }
        price = goods->price;
        buy_number = goods->buy_number;
        totle += price * buy_number;
    }
    return totle;
}
static void gui_pay_refresh(gui_t *gui)
{
    panel_market_t *panel = (panel_market_t *)gui->context;
    market_case1_t *market = (market_case1_t *)panel->context;
    gui->clear(gui);
    gui->showLine(gui, 6, 0, "----------------------");
    gui->showLine(gui, 7, 0, "|Back");
    gui->showLine(gui, 7, 0 + 5, "|     ");
    gui->showLine(gui, 7, 0 + 5 + 5 + 5, "| OK |");

    market->totle = get_totle(market);

    char buff[32] = {0};
    sprintf(buff, "Need pay: %d CNY", market->totle);
    gui_clear_line(gui, 2);
    gui->showLine(gui, 2, 2, buff);

    gui->option_pointer_refrash(gui);
    gui->option_str_refrash(gui);
    gui->title_refresh(gui);
}

static void gui_add_goodsPriceset(gui_t *gui)
{
    panel_market_t *panel = (panel_market_t *)gui->context;
    market_case1_t *market = (market_case1_t *)panel->context;
    market->goods_main->price++;
    char buff[32] = {0};
    sprintf(buff, "Prece: %d CNY", market->goods_main->price);
    gui_clear_line(gui, 2);
    gui->showLine(gui, 2, 2, buff);
}

static void gui_dec_goodsPriceSet(gui_t *gui)
{
    panel_market_t *panel = (panel_market_t *)gui->context;
    market_case1_t *market = (market_case1_t *)panel->context;
    if (market->goods_main->price > 0)
    {
        market->goods_main->price--;
    }
    char buff[32] = {0};
    sprintf(buff, "Prece: %d CNY", market->goods_main->price);
    gui_clear_line(gui, 2);
    gui->showLine(gui, 2, 2, buff);
}

static void gui_ok_goodsPriceSet(gui_t *gui)
{
    gui->back_callBack(gui->context);
}

static void gui_goosNum_refuresh(gui_t *gui)
{
    panel_market_t *panel = (panel_market_t *)gui->context;
    market_case1_t *market = (market_case1_t *)panel->context;
    char buff_p[32] = {0};
    sprintf(buff_p, "Prece: %d CNY", market->goods_main->price);
    gui_clear_line(gui, 2);
    gui->showLine(gui, 2, 2, buff_p);

    char buff[32] = {0};
    sprintf(buff, "Buy number: %d", market->goods_main->buy_number);
    gui_clear_line(gui, 3);
    gui->showLine(gui, 3, 2, buff);

    char buff_ap[32] = {0};
    sprintf(buff_ap, "Subtotal: %d CNY", market->goods_main->buy_number * market->goods_main->price);
    gui_clear_line(gui, 4);
    gui->showLine(gui, 4, 2, buff_ap);
}

static void gui_add_goodsNum(gui_t *gui)
{
    panel_market_t *panel = (panel_market_t *)gui->context;
    market_case1_t *market = (market_case1_t *)panel->context;
    market->goods_main->buy_number++;
    gui_goosNum_refuresh(gui);
}

static void gui_dec_goodsNum(gui_t *gui)
{
    panel_market_t *panel = (panel_market_t *)gui->context;
    market_case1_t *market = (market_case1_t *)panel->context;
    if (market->goods_main->buy_number > 0)
    {
        market->goods_main->buy_number--;
    }
    gui_goosNum_refuresh(gui);
}

static void gui_ok_goodsNum(gui_t *gui)
{
    gui->back_callBack(gui->context);
}

static void clear_buy_num(market_case1_t *market)
{
    market->goods1->buy_number = 0;
    market->goods2->buy_number = 0;
    market->goods3->buy_number = 0;
    market->goods4->buy_number = 0;
    market->goods5->buy_number = 0;
    market->totle = get_totle(market);
}

static void gui_ok_pay(gui_t *gui)
{
    panel_market_t *panel = gui->context;
    market_case1_t *market = panel->context;
    iot_t *iot = market->iot;

    gui->back_callBack(gui->context);

    int goods_num = 0;
    goods_num += market->goods1->buy_number;
    goods_num += market->goods2->buy_number;
    goods_num += market->goods3->buy_number;
    goods_num += market->goods4->buy_number;
    goods_num += market->goods5->buy_number;

    for (int i = 0; i < goods_num; i++)
    {
        market->socket_0->open(market->socket_0);
        market->delay->delay(market->delay, 500);
        market->socket_0->close(market->socket_0);
        market->delay->delay(market->delay, 500);
    }

    // log odders;
    market->odders += goods_num;
    // log sales
    market->sales += market->totle;
    // upload sales
    iot->data_upload_int(iot, 0, market->sales);

    iot->delay(iot, 500);
    // upload odders;
    iot->data_upload_int(iot, 1, market->odders);

    clear_buy_num(market);
}

// callback need to be write in user code.
static void callback_key_0(vkey_t *key, void *arg)
{
    market_case1_t *market = (market_case1_t *)key->context;
    gui_t *gui = market->panel->gui_main;
    gui->back(gui);
}

// callback need to be write in user code.
static void callback_key_1(vkey_t *key, void *arg)
{
    market_case1_t *market = (market_case1_t *)key->context;
    gui_t *gui = market->panel->gui_main;
    gui->down(gui);
}

static void reconnect_callback(void *context)
{
    panel_market_t *panel = context;
    gui_t *gui = panel->gui_main;
    market_case1_t *market = panel->context;

    guiChange_callBack(panel);

    gui->showLine(gui, 2, 1, "Connecting to ");
    gui->showLine(gui, 3, 1, "iot server......");

    DynMemPut((DMEM *)(PubSh->cmd(PubSh, "iot connect")));
    iot_t *iot = market->iot;
    // upload sales
    iot->data_upload_int(iot, 0, 0);
    // upload odders;
    iot->delay(iot, 500);

    iot->data_upload_int(iot, 1, 0);

    gui->showLine(gui, 2, 1, "Reconnect finished!");
    gui->showLine(gui, 3, 1, "Please press [Back].");
}

// callback need to be write in user code.
static void callback_key_2(vkey_t *key, void *arg)
{
    market_case1_t *market = (market_case1_t *)key->context;
    gui_t *gui = market->panel->gui_main;
    gui->enter(gui);
}

static vkey_t *VM_key_init_market(int KEY_id, void *context)
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

static void update(market_case1_t *market, int systime_ms)
{
    if (0 == systime_ms % 3000)
    {
        market->panel->gui_main->refresh_periodic(market->panel->gui_main);
    }
    if (0 == systime_ms % 50)
    {
        market->key_0->update_port(market->key_0, systime_ms);
        market->key_1->update_port(market->key_1, systime_ms);
        market->key_2->update_port(market->key_2, systime_ms);
    }
    if (0 == systime_ms % 300)
    {
        market->key_0->update_callBack(market->key_0, systime_ms);
        market->key_1->update_callBack(market->key_1, systime_ms);
        market->key_2->update_callBack(market->key_2, systime_ms);
    }

    // if (0 == systime_ms % 5000)
    // {
    // 			int latitude = atoi(market->latitude);
    // 			int longitude = atoi(market->longitude);
    // 			iot_t *iot= market->iot;

    // 			if(Save_Data.isUsefull)
    // 			{
    // 				memcpy(market->latitude,Save_Data.latitude,latitude_Length);
    // 				memcpy(market->longitude,Save_Data.longitude,longitude_Length);
    // 			}

    // 			iot->data_upload_int(iot,2,latitude);
    // 			market->delay->delay(market->delay,300);
    // 			iot->data_upload_int(iot,3,longitude);
    // 			market->delay->delay(market->delay,300);
    // }
}

iot_t *VM_iot_Init_market(void *context)
{
    iot_t *iot = VM_iot_Init();
    iot->context = context;
    return iot;
}

panel_market_t *Class_panel_market_init_market(void *context)
{
    panel_market_t *panel = Class_panel_market_init();
    panel->context = context;

    for (int i = 0; i < 5; i++)
    {
        panel->gui_Set->option_callBack[i] = choseGoodsAndChange_callBack;
        panel->gui_shopping->option_callBack[i] = choseGoodsAndChange_callBack;
    }

    panel->gui_goods_set->back = gui_add_goodsPriceset;
    panel->gui_goods_set->down = gui_dec_goodsPriceSet;
    panel->gui_goods_set->enter = gui_ok_goodsPriceSet;
    panel->gui_goods_set->refresh = gui_setGoodsPrice_refresh;

    panel->gui_goods_shopping->back = gui_add_goodsNum;
    panel->gui_goods_shopping->down = gui_dec_goodsNum;
    panel->gui_goods_shopping->enter = gui_ok_goodsNum;
    panel->gui_goods_shopping->refresh = gui_BuyGoods_refresh;

    panel->gui_home->option_callBack[3] = reconnect_callback;

    panel->gui_Reconnect->next_gui[0] = panel->gui_Reconnect;

    panel->gui_Pay->refresh = gui_pay_refresh;
    panel->gui_Pay->enter = gui_ok_pay;

    return panel;
}

VM_socket_t *VM_socket_init_market(int socket_ID, void *context)
{
    VM_socket_t *socket = VM_socket_init_PORT(socket_ID);
    socket->isShowInfo = 1;
    socket->context = context;

    return socket;
}

struct VM_delay *VM_delay_init_market(void *context)
{
    struct VM_delay *delay = VM_delay_init_PORT();
    delay->context = context;
    return delay;
}

market_case1_t *
Class_market_case1_init(void)
{
    DMEM *mem = DynMemGet(sizeof(market_case1_t));
    market_case1_t *market = mem->addr;

    market->mem = mem;
    market->dinit = deinit;

    market->panel = Class_panel_market_init_market(market);

    market->goods1 = Class_goods_init_market(market, "goods 1", 5);
    market->goods2 = Class_goods_init_market(market, "goods 2", 3);
    market->goods3 = Class_goods_init_market(market, "goods 3", 20);
    market->goods4 = Class_goods_init_market(market, "goods 4", 2);
    market->goods5 = Class_goods_init_market(market, "goods 5", 60);
    market->goods_main = market->goods1;

    market->key_0 = VM_key_init_market(0, market);
    market->key_1 = VM_key_init_market(1, market);
    market->key_2 = VM_key_init_market(2, market);

    market->totle = 0;
    market->iot = VM_iot_Init_market(market);

    market->socket_0 = VM_socket_init_market(0, market);
    market->socket_1 = VM_socket_init_market(1, market);

    market->delay = VM_delay_init_market(market);

    market->update = update;
    market->sales = 0;
    market->odders = 0;

    // memcpy(market->latitude,"4352.83797",sizeof("4352.83797"));
    // memcpy(market->longitude,"12518.07578",sizeof("12518.07578"));

    return market;
}