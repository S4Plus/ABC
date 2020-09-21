# [Core](https://qpanda-tutorial.readthedocs.io/zh/latest/api/group_Core.html)

## QuantumCirucit

### 简介

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

### [QCircuit](https://github.com/OriginQ/QPanda-2/blob/master/Core/QuantumCircuit/QCircuit.cpp)

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

### QNode

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

### QGate

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

| 函数名以及函数参数              | 函数功能           |
| ------------------------------- | ------------------ |
| NodeType getNodeType()          | 得到当前节点的类型 |
| 基类AbstractQGateNode的函数接口 | 略                 |

​	在`QGate.h`中，量子门的实现的返回值类型为`QGate`，下面列举其中的一部分：

```c++
QGate QPanda::I(Qubit* qubit);
QGate QPanda::X(Qubit* qubit);
QGate QPanda::U1(Qubit*, double angle);
QGate QPanda::U2(Qubit*, double phi, double lambda);
QGate QPanda::U3(Qubit*, double theta, double phi, double lambda);
QGate QPanda::CU(double alpha, double beta, double gamma, double delta, Qubit*, Qubit*);
```

​	关于各个门的定义可以在QPanda的[官方文档](https://qpanda-tutorial.readthedocs.io/zh/latest/QGate.html)中找到。其中CU门的定义为：
$$
\left(
\begin{matrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & u_0 & u_1\\
0 & 0 & u_2 & u_3\\
\end{matrix}
\right)
= 
\left(
\begin{matrix}
I & O\\
O & U\\
\end{matrix}
\right)
$$
​	但是实际测试了QPanda的代码，其U门的结构应该如下：
$$
U =\left(
 \begin{matrix}
 a & b\\
 -\bar{b} & \bar{a}\\
 \end{matrix}
 \right)
 ,
 |a|^2 + |b|^2 = 1,
 a = u_0 + u_1j,\ b = u_2+u_3j,\ j^2 = -1
$$
​	也就是输入的四个参数分别是$a$的实部和虚部与$b$的实部和虚部。	

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

### QProgram

### 其它接口

**QCircuit createEmptyCircuit()**

所在位置：[Circuit.h](https://github.com/OriginQ/QPanda-2/blob/master/Core/QuantumCircuit/QCircuit.cpp)

QPanda2提供的用于创建空线路的基本接口。返回类型为`QCircuit`，即为所需的空线路。