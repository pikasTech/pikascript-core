# 简介
mimiscript是一个用于资源有限的mcu的脚本绑定工具，提供非侵入式python/typescript脚本绑定以及动态对象、对象树。

支持裸机运行，可运行于内存40Kb以上的mcu中，如stm32f103，esp32。

支持跨平台，可运行于linux环境。

仅使用C标准库，尽可能的结构清晰（尽我所能），几乎不使用宏。


## 使用方法：

测试例：
``` c
#include "sysObj.h"

/* 被绑定的方法 */
void add(MimiObj *obj, Args *args) 
{
    /* 
        obj 是对象指针，args是参数列表，
        所有被绑定的方法均使用此形参 
    */
    
    /* 从参数列表中取出参数 val1 */
    int val1 = args_getInt(args, "val1");
    
    /* 从参数列表中取出参数 val2 */
    int val2 = args_getInt(args, "val2");
    
    /* 实现方法的功能 */
    
    int res = val1 + val2;
    
    /* 将返回值传回参数列表 */
    method_returnInt(args, res);
}

/* 定义测试类的构造器 */
MimiObj *New_MimiObj_test(Args *args)
{
    /* 继承类 */
    MimiObj *self = New_MimiObj_sys(args);
    
    
    /* 
        为脚本绑定一个方法
        传入的第一个参数是对象指针
        传入的第二参数是被绑定方法的接口定义
        （此处使用typescript语法，简单的修改即可支持python格式）
        传入的第三个参数是被绑定方法的函数指针
    */
    class_defineMethod(self, "add(val1:int, val2:int):int", add); 


    /* 返回对象 */
    return self;
}

void main()
{
    /* 
        新建根对象，对象名为“sys” 
        传入对象名和构造器的函数指针
    */
    
    MimiObj *sys = newRootObj("sys", New_MimiObj_sys);

    /* 
        新建test对象，test对象挂载在sys对象下
    */
    obj_newObj(sys, "test", New_MimiObj_test);
    
    /*  
        运行单行脚本。
        因为test对象挂在在sys对象下，
        因此可以通过test.add调用test对象的方法
    */
    obj_run(sys, "res = test.add(val1 = 1, val2 = 2)");
    /*
        (也支持 "res = test.add(1, 2)"的调用方式)
    */
    
    /* 从对象中取出属性值 */
    int res = obj_getInt(sys, "res");
    
    /* 
        析构对象
        所有挂在在sys对象下的子对象都会被自动析构
        本例中挂载了test对象，因此在析构sys对象前，
        test对象会被自动析构
    */
    obj_deinit(sys);
    
    /* 打印返回值 res = 3*/
    printf("%d\r\n", res);    
}
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

