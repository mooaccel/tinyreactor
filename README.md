tinyreactor
==========================

### 项目来源
读了muduo和Linux多线程服务端编程, 有所领悟, 想着自己实现一遍, 重新造一下轮子, 加深理解, 深入学习Reactor模式-Non-blocking IO + IO multiplexing
用C++11以上的线程组件来重新实现部分功能
有的部分与muduo类似.
最终实现one loop per thread(主从Reactor), 用于完成计算密集任务的thread pool后期再考虑
目标是通过这个项目熟悉C++项目开发, 熟悉多线程, 熟悉网络编程, 提高代码品味

### 依赖问题
gtest和glog 
    
    装在/usr/local目录

### 开发计划    
先是单线程Reactor.
然后是多线程Reactor. 每个线程一个EventLoop. 实现多线程, 每个线程一个loop. 即one loop per thread
用C++11以上的标准实现. 
在这个过程中提高debug能力, 以及项目开发能力

### 开发环境
macOS + jetbrain Clion + vim插件
1. Linux环境. 

    >用Clion的远程开发功能. 远程连接上阿里云的Ubuntu18.04云服务器(提供Linux环境)

2. GUI debugger(虽然Clion的这个debugger gui还比较简单). 

    >debug虽然用gdb, lldb命令行方式能操作. 但是还是有点麻烦的...当前有些操作命令行方式更方便, 各有所长
    >所幸Clion远程开发也提供了一个简单的GUI debugger

3. IDE远程头文件跳转.

    >另外还看中一点是在于头文件跳转这里. 比如在包装epoll的那个.cpp文件, 如果想看<sys/epoll.h>的内容. 用这种远程开发方式也能实现.
    >(~/Library/Caches/CLion2019.3/.remote/云服务器ip/be8407c3-f337-4d50-9727-44b23799f0d0/usr/local/include)
