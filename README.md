tinyreactor
==========================

### 项目来源
读完muduo和Linux多线程服务端编程, 有所领悟, 想着自己实现一遍, 加深理解, 深入学习Reactor模式-Non-blocking IO + IO multiplexing
用C++11以上的线程组件来重新实现部分功能
有的部分逻辑大体与muduo类似.
最终要实现one loop per thread + thread pool

### 依赖问题
gtest和glog 
    
为了便于在macOS上用Clion远程部署到云环境Ubuntu18.04上.采用把依赖包统一装在~/.tinykvdb_deps这个路径下.
(本来想这么做的, 后来因为mac Clion这边头文件跳转问题放弃了, 给装在了/usr/local/目录下)
这部分目前是手动安装. 配置用的cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
这个依赖安装怎么融入整个项目, 而不是手动安装?TODO

### 开发计划    
先是单线程Reactor.
然后是多线程Reactor. 每个线程一个EventLoop. 实现多线程, 每个线程一个loop. 即one loop per thread
加上thread pool. 用C++11以上的标准实现. 
在这个过程中熟悉debug. 以及提高项目开发能力

### 开发环境
macOS + jetbrain Clion + vim插件
1. Linux环境. 

    >用Clion的远程开发功能. 远程连接上阿里云的Ubuntu18.04云服务器(提供Linux环境)

2. GUI debugger(虽然Clion的这个debugger gui还比较简单). 

    >debug虽然用gdb, lldb命令行方式能操作. 但是还是有点麻烦的...
    >所幸Clion远程开发也提供了一个简单的GUI debugger

3. IDE远程头文件跳转.

    >另外还看中一点是在于头文件跳转这里. 比如在包装epoll的那个.cpp文件, 如果想看<sys/epoll.h>的内容. 用这种远程开发方式也能实现.
    >(~/Library/Caches/CLion2019.3/.remote/云服务器ip/be8407c3-f337-4d50-9727-44b23799f0d0/usr/local/include)
