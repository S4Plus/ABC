   * [QPanda 学习笔记](#qpanda-学习笔记)
      * [<a href="https://qpanda-tutorial.readthedocs.io/zh/latest/Tutorial.html" rel="nofollow">下载、编译、安装和使用</a>](#下载编译安装和使用)
         * [下载](#下载)
         * [编译和安装](#编译和安装)
         * [使用](#使用)
            * [入门](#入门)
            * [高级](#高级)
         * [注意事项](#注意事项)
   * [QPanda API](#qpanda-api)
   * [QPanda测试用例](#qpanda测试用例)
   * [问题清单](#问题清单)

# QPanda 学习笔记

## 简介

**一种功能齐全，运行高效的量子软件开发工具包**

QPanda 2是由本源量子开发的开源量子计算框架，它可以用于构建、运行和优化量子算法。QPanda 2作为本源量子计算系列软件的基础库，为OriginIR、Qurator、量子计算服务提供核心部件。

## [下载、编译、安装和使用](https://qpanda-tutorial.readthedocs.io/zh/latest/Tutorial.html)

### 下载

1. 如果系统上已经安装了git，可以通过git获取QPanda2：

``` sh
git clone https://github.com/OriginQ/QPanda-2.git
```

如果没有，也可以选择到[QPanda2的Github仓库]( https://github.com/OriginQ/QPanda-2)中下载zip包。在该网页点击`Clone and Download`，再选择`Download ZIP`，完成QPanda-2的下载。

### 编译和安装

由于我使用的操作系统为Linux发行版Ubuntu，所以只介绍在Ubuntu上的编译和安装。

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

这里将安装目录设置在了`/usr/local`下。

### 使用

#### 入门

例程使用QPanda提供的用例。下面的例子可以在量子计算机中构建量子纠缠态($|00\rangle + |11\rangle$)，对其进行测量，重复制备1000次。在无噪声的模拟器上运行的结果是约有50%的概率使测量结果分别在00或11上。

```C++
USING_QPANDA

int main()
{
    // 初始化量子虚拟机
    init(QMachineType::CPU);

    // 申请量子比特以及经典寄存器
    auto q = qAllocMany(2);
    auto c = cAllocMany(2);

    // 构建量子程序
    QProg prog;
    prog << H(q[0])
        << CNOT(q[0],q[1])
        << MeasureAll(q, c);

    // 量子程序运行1000次，并返回测量结果
    auto results = runWithConfiguration(prog, c, 1000);

    // 打印量子态在量子程序多次运行结果中出现的次数
    for (auto &val: results)
    {
        std::cout << val.first << ", " << val.second << std::endl;
    }

    // 释放量子虚拟机
    finalize();

    return 0;
}
```

在Linux环境下，用户可以使用g++编译test.cpp，编译命令如下：

```sh
g++ test.cpp -std=c++14	-fopenmp -I{QPanda安装路径}/include/qpanda2 -I{QPanda安装路径}/include/qpanda2/ThirdParty -L{QPanda安装路径}/lib/ -lComponents -lQAlg -lQPanda2 -lTinyXML -lantlr4 -lGPUQGates -o test
```

#### 高级

​	除了QPanda官方文档中提供的例程，QPanda文件夹下`Application`文件夹和`Test`文件夹也包含了许多可以用来参考的例子。下面我结合几个例程介绍QPanda的其他功能。

**量子线路**

​	在例程中，量子线路的表示上只使用了最简单的`H`和`CNOT`门。但是在官方文档中关于[量子逻辑门的介绍](https://qpanda-tutorial.readthedocs.io/zh/latest/QGate.html)中可以看到除此之外还有其他的门。关于他们的使用，可以参考以下文件：

| 文件名                     | 文件位置  | 概述                                   |
| -------------------------- | --------- | -------------------------------------- |
| QProgTransform.test.cpp    | test/Core | 测试将量子线路转化成OriginIR的功能     |
| QCircuitOptimizer.test.cpp | test/Core | 测试对量子子线路进行等价替换优化的功能 |
| Oracle.cpp                 | test/Core | 往量子线路中添加黑盒                   |

​	上述文件由于自身的需要，大部分代码是关于量子线路的创建，其中包含量子逻辑门的调用，共轭转置$\dagger$的实现等。对于想要更进一步理解程序的执行原理的同学可以访问`include/Core/QuantumCircuit/QGate.h`文件，其中包含了量子门类`QGate`的API介绍以及量子逻辑门的定义。

**控制流**

​	在QPanda的官方文档中提到，可以使用`QIf`和`QWhile`控制程序的执行流。同时官网也分别给出了[`QIf`](https://qpanda-tutorial.readthedocs.io/zh/latest/QIf.html)和[`QWhile`](https://qpanda-tutorial.readthedocs.io/zh/latest/QWhile.html)的使用样例。对于想要阅读更多有关这两者使用代码的同学，可以看下面的文件：

| 文件名                          | 文件位置  | 概述                                                         |
| ------------------------------- | --------- | ------------------------------------------------------------ |
| JudgeTwoNodeIterIsSwappable.cpp | test/Core | 测试检查两段线路是否可以交换（对易性）的功能<br>其中涉及对QIf的使用 |
| ClassicalCondition.cpp          | test/Core | 测试线路对于经典态的支持<br>其中涉及对QWhile的使用           |

**量子算法**

​	对于量子算法感兴趣的同学，可以阅读`Application`文件夹下的代码，其中包含了目前已有的大部分量子算法的实现，例如BV算法，Groover搜索算法等。

### 注意事项

+ 如果产生类似`QPanda.h: No such file or Directory`的错误，说明`-I`选项的配置有可能产生了问题。`-I`选项用于添加搜索头文件的目录，这点可以通过阅读[Linux man手册](https://linux.die.net/man/1/g++)得知。需要注意的是，`-I`选项后面跟着的应该是绝对路径，否则同样会产生无法找到文件的错误。
+ 如果产生类似`/usr/bin/ld: cannnot find -lQAlg的错误，说明`-L`选项的配置有可能产生了问题。`-L`用于添加`-l`选项搜索的文件或者文件夹。
+ 其中`-lTinyXML`和`-lGPUQGates`的有无不对测试程序的运行产生影响

# QPanda API

​	请参考API文件夹下的[QPanda API笔记](./API/README.md)。

# QPanda测试用例

​	请参考[QPanda 测试用例笔记](./Example/README.md)

# 问题清单

1. `QCircuit`，`AbstractQuantumCicuit`和`OriginCircuit`的区别？
2. 由于`QObject`和`QNode`均是抽象基类，且在实现的虚函数上`QNode`要包含`QObject`。`QNode`继承自`QObject`否是冗余的？
3. `QGate.h`中的112行声明了`QGateNodeFactory`，在246行定义了`QGateNodeFactory`，是否冗余？

> 我认为上面的问题来自于QPanda升级到2.0更新了部分类结构(如节点(node)，门(gate)，线路(circuit))的基类，而又没有删除原本的基类所以导致了这些冗余的问题。



