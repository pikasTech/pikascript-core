#include "test_timer.h"
#include "callback.h"
#include "timer.h"

struct test_data_t
{
    int isShow;
    int res[3];
    int i;
};

void _handler_test(callback_t *callback)
{
    mTimer_t *timer = callback->context;
    struct test_data_t *test_data = timer->context;
    int *i_p = &(test_data->i);
    int i = *i_p;

    int isShow = test_data->isShow;

    test_data->res[i] = timer->time_now;
    (*i_p)++;

    if (isShow)
    {
        printf("handling timer, the time now is %d\r\n", timer->time_now);
        printf("res %d = %d\r\n", i, test_data->res[i]);
    }
}

int TEST_timer(int isShow)
{
    int err = 0;

    struct test_data_t test_data =
        {
            .isShow = isShow,
            .res[0] = 0,
            .res[1] = 0,
            .res[2] = 0,
            .i = 0};

    mTimer_t *timer = Class_timer_init(_handler_test);
    timer->context = &test_data;

    timer->set(timer, 3000);
    timer->enable(timer);

    for (int ms = 0; ms < 10000; ms++)
    {
        timer->update(timer, ms);

        if (4000 == ms)
        {
            timer->set(timer, 1000);
            timer->enable(timer);
        }
        if (6000 == ms)
        {
            timer->set(timer, 2000);
            timer->enable(timer);
        }
        if (7000 == ms)
        {
            timer->set(timer, 2000);
            timer->enable(timer);
        }
    }

    if (3001 != test_data.res[0])
    {
        err = 1;
        goto exit;
    }

    if(5001 != test_data.res[1])
    {
        err = 2;
        goto exit;
    }

    if(9001 != test_data.res[2])
    {
        err = 3;
        goto exit;
    }

    goto exit;

exit:
    timer->dinit(timer);
    return err;
}