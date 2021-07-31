# 简介
mimiscript是一个用于资源有限的mcu的面向对象c语言脚本库，提供动态对象，对象树以及非侵入式的python/typescript脚本绑定。

支持裸机运行，可运行于内存40Kb以上的mcu中，如stm32f103,esp32等。

支持跨平台，可运行于linux环境。

仅使用C标准库，尽可能的结构清晰（水平有限），几乎不使用宏。


## 使用方法：

测试例：
``` c
#include "sysObj.h"

/* 被绑定的方法 */
void add(MimiObj *obj, Args *args)
{
    int val1 = args_getInt(args, "val1");
    int val2 = args_getInt(args, "val2");
    method_returnInt(args, val1 + val2);
}

/* 定义测试类 */
MimiObj *New_MimiObj_test(Args *args)
{
    /* 继承类 */
    MimiObj *self = New_MimiObj_sys(args);
    /* 定义方法 */
    class_defineMethod(self, "add(val1:int, val2:int):int", add);
    /* 返回对象 */
    return self;
}

/* 新建对象容器，对象名为“testObj” */
MimiObj *obj = newRootObj("testObj", New_MimiObj_Root);
/* 调用方法 */
obj_run(obj, "res = add(val1 = 1, val2 = 2)");
/* 从对象容器中取出返回值 */
int res = obj_getInt(obj, "res");
/* 输出返回值 res = 3*/
printf("%d\r\n", res);
/* 析构 */
obj_deinit(obj);
```

# 结构摘要：

## 1.数据结构 mimidata
datamemory：内存管理

datalink：非侵入式双向链表

dataArg：数据容器，支持int、float、string、object类型和自定义类型

dataArgs：基于双向链表的数据列表，常用作动态参数列表，并用来存储对象的属性和方法

dataSting，字符串处理

## 2.对象支持 (mimiObject) 
dataObject：对象构造、对象析构、对象树、创建属性、绑定属性、绑定方法、python接口绑定、python接口解析

## 3.命令行交互层 (mimishell) 
mimiSH：用于调用python接口

## 4.基于发布-订阅模型的事件机制（mimiEvnet）
支持同步事件回调

## 5.单元测试 (mimitest) 
几乎完全覆盖的单元测试

