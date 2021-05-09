# 简介
mimiLib是一个跨平台的面向对象c语言库
# 包含:
## 1.字符串处理（mimistr) 

## 2.内存管理 (mimimemory) 

## 3.虚拟硬件 (mimivm)

支持串口和 gui 

## 4.命令行交互层 (mimishell) 

## 5.数据结构 (mimidata) 

## 6.异步回调机制 (mimiapp) 

## 7.单元测试 (mimitest) 

## 8.系统兼容层 (mimiport)

支持在stm32和linux上运行

## 9.代码片段 (mimiconfig/vscode)

加速构建统一的面向对象代码

## 使用方法：

``` shell
    # build the container
    cd docker
    sh build.sh

    # attach to the container
    docker attach mimilib

    # make the project
    cd mimilib
    sh make.sh

    # update and contribute to the project
    sh update.sh
```

