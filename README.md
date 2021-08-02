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

