# QPanda 学习笔记

## [下载、编译、安装和使用](https://qpanda-tutorial.readthedocs.io/zh/latest/Tutorial.html)

### 下载

1. 如果系统上已经安装了git，可以通过git获取QPanda2：

``` sh
git clone https://github.com/OriginQ/QPanda-2.git
```

如果没有，也可以选择到[QPanda2的Github仓库]( https://github.com/OriginQ/QPanda-2)中下载zip包。在该网页点击`Clone and Download`，再选择`Download ZIP`，完成QPanda-2的下载。

### 编译和安装

由于我使用的操作系统为Linux发行版Ubuntu，所以只介绍在Ubuntu上的编译和安装。以下为QPanda的环境要求：

**环境要求**

+ gcc/clang/（其他c++编译器）：支持c++14标准以及openMP
+ CMake3.0以上
+ pkg-config工具(Linux,必须。Ubuntu下使用 sudo apt-get install pkg-config 安装,CentOS下需要下载源码编译)
+ UUID库（Linux，必须。Ubuntu下使用 sudo apt-get install uuid-dev 安装，CentOS下使用yum install libuuid-devel安装）
+ Python 3.5 以上 (pyQPanda，可选)
+ CURL (量子云连接功能，可选)
+ MPI (在集群上运行量子虚拟机，可选)
+ CUDA 9 以上 (在GPU上运行量子虚拟机，可选）

**编译**

1. 进入QPanda-2根目录
2. 输入以下命令：

```sh
mkdir -p build
cd build 
cmake -DCMKAE_INSTALLL_PREFIX=/usr/local -DFIND_CUDA=OFF -DUSE_CHEMIQ=OFF -DUSE_PYQPANDA=OFF ..
make
```

在上段代码中，将安装目录设置在了`/usr/local`下。

### 使用

接下来使用QPanda提供的示例程序进行演示，下面的例子可以在计算机上

在Linux环境下，用户可以使用g++编译test.cpp，编译命令如下：

```sh
g++ test.cpp -std=c++14	-fopenmp -I{QPanda安装路径}/include/qpanda2 -I{QPanda安装路径}/include/qpanda2/ThirdParty -L{QPanda安装路径}/lib/ -lComponents -lQAlg -lQPanda2 -lTinyXML -lantlr4 -lGPUQGates -o test
```

**注意事项**

+ 如果产生类似`QPanda.h: No such file or Directory`的错误，说明`-I`选项的配置有可能产生了问题。`-I`选项用于添加搜索头文件的目录，这点可以通过阅读[Linux man手册](https://linux.die.net/man/1/g++)得知。需要注意的是，`-I`选项后面跟着的应该是绝对路径，否则同样会产生无法找到文件的错误。
+ 如果产生类似`/usr/bin/ld: cannnot find -lQAlg的错误，说明`-L`选项的配置有可能产生了问题。`-L`用于添加`-l`选项搜索的文件或者文件夹。
+ 其中`-lTinyXML`和`-lGPUQGates`的有无不对测试程序的运行产生影响