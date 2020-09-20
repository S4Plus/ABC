# QPanda 学习笔记

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

## [Core](https://qpanda-tutorial.readthedocs.io/zh/latest/api/group_Core.html)

### QuantumCirucit

#### 简介

​	量子线路，也称量子逻辑电路是最常用的通用量子计算模型，表示在抽象概念下，对于量子比特进行操作的线路，是各种逻辑门的集合。最后常需要量子测量将结果读取出来。

​	在QPanda2中，QCircuit类是一个仅装载量子逻辑门的容器类型，它也是QNode中的一种，初始化一个QCircuit对象有以下两种方法：

```c++
QCircuit cir = QCircuit();
```

或

```c++
QCircuit cir = createEmptyCircuit();
```

你可以通过如下方式向QCircuit尾部填充结点：

```c++
QCircuit << QGate;
```

**接口总览**

```C++
class QGATE_SPACE::AbstractAngleParameter;
class QPanda::AbstractClassicalProg;
class QPanda::AbstractControlFlowNode;
class QPanda::AbstractQGateNode;
class QPanda::AbstractQuantumCircuit;
class QPanda::AbstractQuantumMeasure;
class QPanda::AbstractQuantumProgram;
class QPanda::AbstractQuantumReset;
class QPanda::CExpr;
class QPanda::CExprFactory;
class QPanda::CExprFactoryHelper;
class QPanda::ClassicalCondition;
class QPanda::ClassicalProg;
class QPanda::ClassicalProgFactory;
class QPanda::ClassicalProgRegisterAction;
class QPanda::HadamardQCircuit;
class QPanda::Item;
class QPanda::OriginCExpr;
class QPanda::OriginCircuit;
class QPanda::OriginClassicalProg;
class QPanda::OriginItem;
class QPanda::OriginProgram;
class QPanda::OriginQGate;
class QPanda::OriginQIf;
class QPanda::OriginQWhile;
class QPanda::OriginReset;
class QPanda::QCircuit;
class QPanda::QGate;

template <typename ... Targs>
class QGATE_SPACE::QGateFactory;

class QPanda::QGateNodeFactory;
class QPanda::QIfFactory;
class QPanda::QIfProg;
class QPanda::QIfRegisterAction;
class QPanda::QMeasure;
class QPanda::QNode;
class QPanda::QNodeDeepCopy;
class QPanda::QProg;
class QPanda::QReset;
class QPanda::QResetFactory;
class QPanda::QWhileFactory;
class QPanda::QWhileProg;
class QPanda::QuantumCircuitFactory;
class QGATE_SPACE::QuantumGate;
class QPanda::QuantumMeasureFactory;
class QPanda::QuantumProgramFactory;

// global functions

QIfProg QPanda::createIfProg(ClassicalCondition cc, QProg true_node);
QIfProg QPanda::createIfProg(ClassicalCondition cc, QProg true_node, QProg false_node);
QWhileProg QPanda::createWhileProg(ClassicalCondition cc, QProg true_node);
QCircuit QPanda::createEmptyCircuit();
HadamardQCircuit QPanda::createHadamardQCircuit(QVec& pQubitVector);
QGate QPanda::I(Qubit* qubit);
QGate QPanda::X(Qubit* qubit);
QGate QPanda::X1(Qubit* qubit);
QGate QPanda::RX(Qubit*, double angle);
QGate QPanda::U1(Qubit*, double angle);
QGate QPanda::U2(Qubit* qubit, double phi, double lambda);
QGate QPanda::U3(Qubit* qubit, double theta, double phi, double lambda);
QGate QPanda::Y(Qubit* qubit);
QGate QPanda::Y1(Qubit* qubit);
QGate QPanda::RY(Qubit*, double angle);
QGate QPanda::Z(Qubit* qubit);
QGate QPanda::Z1(Qubit* qubit);
QGate QPanda::RZ(Qubit*, double angle);
QGate QPanda::S(Qubit* qubit);
QGate QPanda::T(Qubit*);
QGate QPanda::H(Qubit* qubit);
QGate QPanda::CNOT(Qubit* control_qubit, Qubit* target_qubit);
QGate QPanda::CZ(Qubit* control_qubit, Qubit* target_qubit);
QGate QPanda::U4(double alpha, double beta, double gamma, double delta, Qubit*);
QGate QPanda::U4(QStat& matrix, Qubit*);
QGate QPanda::QDouble(QStat& matrix, Qubit* qubit1, Qubit* qubit2);
QGate QPanda::CU(double alpha, double beta, double gamma, double delta, Qubit*, Qubit*);
QGate QPanda::CU(QStat& matrix, Qubit*, Qubit*);
QGate QPanda::iSWAP(Qubit* targitBit_fisrt, Qubit* targitBit_second, double theta);
QGate QPanda::CR(Qubit* control_qubit, Qubit* targit_qubit, double theta);
QGate QPanda::SqiSWAP(Qubit* targitBit_fisrt, Qubit* targitBit_second);
QGate QPanda::SWAP(Qubit* targitBit_fisrt, Qubit* targitBit_second);

template <typename _Ty>
_Ty QPanda::deepCopy(_Ty& node);

QProg QPanda::createEmptyQProg();
QReset QPanda::Reset(Qubit*);
```

接下来用文件区分对各个接口进行分类：

#### [QCircuit](https://github.com/OriginQ/QPanda-2/blob/master/Core/QuantumCircuit/QCircuit.cpp)

[AbstractQuantumCircuit](https://qpanda-tutorial.readthedocs.io/zh/latest/api/class_QPanda_AbstractQuantumCircuit.html#doxid-class-q-panda-1-1-abstract-quantum-circuit)

量子线路抽象基类

**virtual NodeIter getFirstNodeIter() = 0**

作为基类提供的虚方法。用于获得迭代器的首部，对应`first_node_iter`。相对地，其他的`getLastNodeIter()`，`getEndNodeIter()`和`getHeadNodeIter`分别对应不同的返回值。

**virtual bool isDagger() const = 0**

判断当前的量子线路是否是dagger

**virtual NodeIter deleteQNode(NodeIter &)**

对量子线路中的节点进行操作。`deleteQNode()`函数将`NodeIter`所指向位置的节点删除，而`insertQNode()`函数是往`NodeIter`指向位置插入新的节点。

**virtual void pushBackNode(std::shared_ptr<QNode> )**

往量子线路末端插入新的节点。该接口同时在`QCircuit`类中的`<<`操作符重载中被调用。

[QCircuit](https://qpanda-tutorial.readthedocs.io/zh/latest/api/class_QPanda_QCircuit.html#doxid-class-q-panda-1-1-q-circuit)

```C++
class QCircuit: public QPanda::AbstractQuantumCircuit
{
protected:  
 		std::shared_ptr<AbstractQuantumCircuit> m_pQuantumCircuit;
public:
    // construction
    QCircuit();
    QCircuit(const QCircuit&);
    QCircuit(QGate& gate);
    QCircuit(std::shared_ptr<AbstractQuantumCircuit> node);

    // methods

    std::shared_ptr<AbstractQuantumCircuit> getImplementationPtr();
    virtual void pushBackNode(std::shared_ptr<QNode>);

    template <typename T>
    QCircuit& operator << (T node);

    virtual QCircuit dagger();
    virtual QCircuit control(QVec);
    NodeType getNodeType() const;
    virtual bool isDagger() const;
    virtual bool getControlVector(QVec&);
    virtual NodeIter getFirstNodeIter();
    virtual NodeIter getLastNodeIter();
    virtual NodeIter getEndNodeIter();
    virtual NodeIter getHeadNodeIter();
    virtual NodeIter insertQNode(NodeIter&, std::shared_ptr<QNode>);
    virtual NodeIter deleteQNode(NodeIter&);
    virtual void setDagger(bool isDagger);
    virtual void setControl(QVec);
};
```

​	QCircuit类为量子线路的抽象基类。

**std::shared_ptr<AbstractQuantumCircuit> m_pQuantumCircuit**

为`protected`属性。根据类型看来应该是保存线路信息的变量。

**virtual bool getFirstNodeIter()**

这一系列的对于NodeIter的操作均为对保护属性`std::shared_ptr<AbstractQuantumCircuit> m_pQuantumCircuit`的操作。

**class HadamardQCircuit: public QCircuit**

QCircuit类的直接继承类。根据名字为仅由Hadamard门组成的线路。

[OriginCircuit](https://qpanda-tutorial.readthedocs.io/zh/latest/api/class_QPanda_OriginCircuit.html#doxid-class-q-panda-1-1-origin-circuit)

​	继承自`public QNode`和`public AbstractQuantumCircuit`的类型，`QCircuit`的实现类。从观察上来看这并非抽象基类而是真正可以使用的类。

[QuantumCircuitFactory](https://qpanda-tutorial.readthedocs.io/zh/latest/api/class_QPanda_QuantumCircuitFactory.html#details-class-q-panda-1-1-quantum-circuit-factory)

​	类AbstractQuantumCircuit的工厂。风咋混个了创建QuantumCircuit的函数。

```c++
class QuantumCircuitFactory
{
public:
    // methods
    void registClass(
        std::string name,
        CreateQCircuit method
        );
    AbstractQuantumCircuit* getQuantumCircuit(std::string&);
    static QuantumCircuitFactory& getInstance();
};
```



#### QNode

[QNode](https://qpanda-tutorial.readthedocs.io/zh/latest/api/class_QPanda_QNode.html#doxid-class-q-panda-1-1-q-node)

​	继承自`QObject`。量子节点抽象基类。其中`QObject`的定义如下：

```c++
class QObject
{
  // 得到当前结点类型
  virtual NodeType getNodeType() const = 0;
}
```

​	而QNode的定义为：

```c++
class QNode : public QObject
{
public:
    virtual NodeType getNodeType() const = 0;
    virtual ~QNode() {}
};
```

[Item](https://qpanda-tutorial.readthedocs.io/zh/latest/api/class_QPanda_Item.html)

​	项的抽象基类。从提供的接口`virtual Item * getNext()`和`virtual Item * getPre()`以及对应的set接口上看，其代表的结构类似于链表节点。

OriginItem

​	继承自`Item`类，作为原始Item实现类。除了实现抽象基类`Item`的接口之外，添加了私有属性以支持接口的操作。定义如下：

```c++
class  OriginItem : public Item
{
private:
    Item *m_pNext;			/**< next item pointer*/
    Item *m_pPre;				/**< previous item pointer*/
    std::shared_ptr<QNode> m_node;  /**< QNode shared pointer*/

public:
    OriginItem();
    ~OriginItem();
    Item * getNext()const;
    Item * getPre()const;
    std::shared_ptr<QNode> getNode() const;
    void setNext(Item * pItem);
    void setPre(Item * pItem);
    void setNode(std::shared_ptr<QNode> pNode);
};
```

​	在添加了属性变量m_pNext和m_pPre之后，该类更加接近链表节点这一数据类型了。

#### QGate

**[AbstractQGateNode](https://qpanda-tutorial.readthedocs.io/zh/latest/api/class_QPanda_AbstractQGateNode.html)**

​	量子门抽象基类。各个方法的作用如下：

| 函数名以及函数参数                      | 函数功能                       |
| --------------------------------------- | ------------------------------ |
| virtual  size_t getQubitVector(QVec& )  | 获得当前量子门内部QVec以及尺寸 |
| virtual Qubit * popBackQubit() = 0      | 消除末端的QVec元素             |
| virtual void PushBackQubit(Qubit *)     | 往末端插入QVec元素             |
| virtual size getTargetQubitNum() const  | 得到当前量子门中的目标比特个数 |
| virtual size getControlQubitNum() const | 得到当前量子门中的控制比特个数 |
| virtual QuantumGate * getQGate() const  | 得到量子门                     |
| virtual void setQGate()                 | 设置量子门                     |
| virtual bool isDagger()                 | 判断当前量子门是否是共轭转置   |
| virtual size_t getControlVector(QVec&)  | 从当前量子门节点中得到控制矢量 |
| virtual bool setDagger(bool)            | 对当前量子门设置dagger         |
| virtual bool setControl(QVec)           |                                |

**QGate**

```C++
class QGate : public AbstractQGateNode
```

​	QPanda2量子门基类。

+ 成员变量
	+ std::shared_ptr<AbstractQGateNode> m_qgate_node
+ 公有方法：

| 函数名以及函数参数     | 函数功能           |
| ---------------------- | ------------------ |
| NodeType getNodeType() | 得到当前节点的类型 |

​	在`QGate.h`中，量子门的实现的返回值类型为`QGate`，下面列举其中的一部分：

```c++
QGate I(Qubit* qubit);
QGate X(Qubit* qubit);
QGate U1(Qubit*, double angle);
QGate U2(Qubit*, double phi, double lambda);
QGate U3(Qubit*, double theta, double phi, double lambda);
QGate CU(double alpha, double beta, double gamma, double delta, Qubit * )
```

**OriginQGate**

​	QGate的实现类

```C++
class OriginQGate : public QNode, public AbstractQGateNode
```

+ 私有变量
	+ QVec m_qubit_vector
	+ QuantumGate *m_qgate
	+ NodeType m_node_type
	+ bool m_Is_dagger
	+ std::vector<Qubit*> m_control_qubit_vector
+ 公共方法均为父类的虚方法的实现

#### QProgram



#### 其它接口

**QCircuit createEmptyCircuit()**

所在位置：[Circuit.h](https://github.com/OriginQ/QPanda-2/blob/master/Core/QuantumCircuit/QCircuit.cpp)

QPanda2提供的用于创建空线路的基本接口。返回类型为`QCircuit`，即为所需的空线路。



### Utilities.Tools

#### QCircuitOptimizer

该文件中暂时没有在文档中找到相关记录。

**AbstractCirOptimizer**

​	文档中没有记录。从成员函数看来包括两个函数，`do_optimize`根据

```c++
class AbstractCirOptimizer {
  virtual void do_optimize(QProg src_prog, OptimizerSink &gates_sink, std::vector<QCircuit>& replace_to_cir_vec) = 0;
  virtual bool is_same_controled(pOptimizerNodeInfo first_node, pOptimizerNodeInfo second_node);
}
```

**OptimizerSubCir**

```C++
struct optimizerSubCir {
  QCircuit target_sub_cir;
  QCircuit replace_to_sub_cir;
  OptimizerSink m_sub_cir_gates_buffer;
}
```

​	根据结构的名字推断这应该是用于子线路替换的工具，即有可能是等价线路变换中需要用到的数据结构。可以通过查找`target_sub_cir`——推测是目标子线路的意思——来判断哪个函数用于替换子线路。

**FindSubCircuit**

```c++
class FindSubCircuit {
public:
  template <class T>
  	using MatchNode = seqNode<T>;
  template <class T>
    using MatchNodeVec = std::vector<MatchNode<T>>;
  using MatchNodeTable = std::vector<std::pair<pOptimizerNodeInfo, MatchNodeVec<pOptimizerNodeInfo>>>;
private:
  TopologSequence<pOptimizerNodeInfo>& m_topolog_sequence;
  std::vector<TopologSequence<pOptimizerNodeInfo>> m_sub_graph_vec; /* Multiple possible matching subgraphs, each of which is stored in the form of topological sequence */
	MatchNodeTable m_node_match_vector;
}
```

+ 公共变量和私有变量参考以上代码
+ 公有函数
	+ 构造函数`FindSubCircuit`和析构函数`~FindSubCircuit`
	+ 见下表：

| 函数定义                                                     | 函数功能                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| sub_cir_query(<br>TopologSequence<pOptimizerNodeInfo>& sub_sequence) | 查询子图并将查询结果保存在query_Result中                     |
| node_match(<br>const SeqNode<pOptimizerNodeInfo>& target_seq_node,<br>const SeqNode<pOptimizerNodeInfo>& graph_node) | 从函数定义看来，主要是比对<br/>`target_seq_node`和`graph_node`两者的<br>类型是否相等。 |
| match_layer(<br>SeqLayer<pOptimizerNodeInfo>& sub_seq_layerb,<br>const size_t match_layer,<br>std::vector<TopologSequence<pOptimizerNodeInfo>>&<br> sub_graph_vec) | 层匹配：匹配且合并子图中每一层的节点                         |
|                                                              |                                                              |
|                                                              |                                                              |

#### JsonConfigParam

[**JsonConfigParam**]

+ 变量

| 变量名         | 数据类型            | 访问范围 | 变量功能 |
| -------------- | ------------------- | -------- | -------- |
| m_doc          | rapidjson::Document | private  |          |
| m_json_content | std::string         | private  |          |

+ 函数接口

| 函数名                                                       | 函数功能                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| bool load_config(const std::string config_data = CONFIG_PATH) | 加载json配置数据<br>输入参数config_data可以是配置文件或者配置数据，用后缀名判断 |
| rapidjson::Document& get_root_element()                      | 返回m_doc变量                                                |
| static bool readAdjacentMatrix(<br>const rapidjson::Value& AdjacentMatrixElement,<br>int &qubit_num,<br>std::vector\<std::vector<double\>> &vec,<br>const std::string configFile="" ) | 从json配置文件中获得拓扑结构                                 |
|                                                              |                                                              |

+ 下面结合cpp文件具体介绍各个函数
+ `bool JsonConfigParam::load_config(const std::string file_name)`

# QPanda测试用例

**注意事项**

+ 放在`Test`文件夹下的程序使用了Google Test项目，未安装gtest并且想要利用它进行测试的同学可以到[链接](https://blog.csdn.net/csm201314/article/details/75136605)处下载。如果不想要使用gtest的，可以将对应的`TEST`宏替换成`main`函数，并做相应的修改。

## QCircuitOptimizer.test.cpp

程序的重点在于利用`QPanda::sub_cir_optimizer()`函数对于子线路进行优化。





# 问题清单

1. `QCircuit`，`AbstractQuantumCicuit`和`OriginCircuit`的区别？
2. 由于`QObject`和`QNode`均是抽象基类，且在实现的虚函数上`QNode`要包含`QObject`。`QNode`继承自`QObject`否是冗余的？
3. `QGate.h`中的112行声明了`QGateNodeFactory`，在246行定义了`QGateNodeFactory`，是否冗余？

> 我认为上面的问题来自于QPanda升级到2.0更新了部分类结构(如节点(node)，门(gate)，线路(circuit))的基类，而又没有删除原本的基类所以导致了这些冗余的问题。



