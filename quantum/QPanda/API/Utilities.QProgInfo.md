## Utilities.QProgInfo

## NodeInfo

**NodeInfo**

```c++
class NodeInfo {
  NodeInfo():
  	m_gate_type(GATE_UNDEFINED), m_node_type(NODE_UNDEFINED), m_is_dagger(false) {}
  
  void clear() {
    m_itr = NodeIter();
    m_node_type = NODE_UNDEFINED;
    m_gate_type = GATE_UNDEFINED;
    m_is_dagger = false;
    m_qubits.clear();
    m_control_qubits.clear();
  }
  
  NodeIter m_itr;
  NodeType m_node_type;
  GateType m_gate_type;
  bool m_is_dagger;
  QVec m_qubits;
  QVec m_control_qubits;
}
```

​	`NodeInfo`节点中保存着`QProg`节点的详细信息。

**成员变量**

| 变量名           | 数据类型 | 说明                                       |
| ---------------- | -------- | ------------------------------------------ |
| m_itr            | NodeIter | 结点的迭代器                               |
| m_node_type      | NodeType | 当前结点的类型                             |
| m_gate_type      | GateType | 如果当前结点的类型为门结点，保存当前门类型 |
| m_is_dagger      | bool     | 是否为dagger                               |
| m_qubits         | QVec     | 当前结点的量子比特                         |
| m_control_qubits | QVec     | 当前结点的控制量子比特                     |

**成员函数（不包含构造函数和析构函数）**

| 函数名  | 函数参数 | 函数返回值 | 函数说明         |
| ------- | -------- | ---------- | ---------------- |
| clear() |          | void       | 清除节点中的信息 |

**QuantumParam**

```c++
class QuantumParam
{
public:
  QCircuitParam() {
    m_is_dagger = false;
  }
  
  virtual ~QCircuitParam() {}
  
  QCircuitParam(const QCircuit& rhs){
    m_is_dagger = rhs.m_is_dagger;
    m_control_qubits = rhs.m_control_qubits;
  }
  
  virtual std::shared<QCircuitParam> clone() {
    return std::make_shared<QCircuitParam>(*this);
  }
  
  void append_control_qubits(const QVec &ctrl_qubits) {
    m_control_qubits.insert(m_control_qubits.end(), ctrl_qubits.begin(), ctrl_qubits.end());
  }
  
  static QVec get_real_append_qubits(QVec append_qubits, QVec target_qubits) {
		if (0 == target_qubits.size())
		{
			return append_qubits;
		}
		if (0 == append_qubits.size())
		{
			return QVec();
		}
		auto sort_fun = [](Qubit*a, Qubit* b) {return a->getPhysicalQubitPtr()->getQubitAddr() < b->getPhysicalQubitPtr()->getQubitAddr(); };
		std::sort(append_qubits.begin(), append_qubits.end(), sort_fun);
		std::sort(target_qubits.begin(), target_qubits.end(), sort_fun);
		QVec result_vec;
		set_difference(append_qubits.begin(), append_qubits.end(), target_qubits.begin(), target_qubits.end(), std::back_inserter(result_vec));
		return result_vec;
	}
  
	bool m_is_dagger;  /**< dagger information */
	std::vector<QPanda::Qubit*> m_control_qubits;/**< control Quantum bits */
};
```

​	线路参数信息。

**成员函数说明**

| 函数名                 | 函数参数                                  | 函数返回值                 | 函数说明                   |
| ---------------------- | ----------------------------------------- | -------------------------- | -------------------------- |
| clone                  |                                           | std::shared<QCircuitParam> | 克隆函数                   |
| append_control_qubits  | const QVec &ctrl_qubits                   | void                       | 添加控制量子比特           |
| get_real_append_qubits | QVec append_qubits,<br>QVec target_qubits | QVec                       | 获得真正增加的控制量子比特 |

​	其中`get_real_append_qubits()`函数的输入参数`append_qubits`为增加的控制量子比特，可能存在某些重复的。而`target_qubits`为已经受控的量子比特。返回值为真正增加的控制量子比特。

**TraverByNodeIter**

```c++
class TraverseByNodeIter : public TraversalInterface<QCrcuitParam&, NodeIter&>
{
public:
  TraverseByNodeIter() {}
  ~TraverseByNodeIter() {}
  
public:
  virtual void execute(std::shared_ptr<AbstractQGateNode> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) {
    // handle QGate node
  }; 
  
  virtual void execute(std::shared_ptr<AbstractQuantumMeasure> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) {
		//handle measure node
	}
  
  	virtual void execute(std::shared_ptr<AbstractQuantumReset> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) {
		//handle reset node
	}

	virtual void execute(std::shared_ptr<AbstractClassicalProg> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) {
		// handle classical prog
	}

	virtual void execute(std::shared_ptr<AbstractControlFlowNode> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) {
		// handle flow control node
		Traversal::traversal(cur_node, *this, cir_param, cur_node_iter);
	}

	virtual void execute(std::shared_ptr<AbstractQuantumCircuit> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter);
	virtual void execute(std::shared_ptr<AbstractQuantumProgram> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter);
  virtual void traverse_qprog(QProg prog);
}
```

**成员函数说明**

​	其中，只有对应`shared_ptr<AbstractQuantumCircuit>`和`shared_ptr<AbstractQuantumProgram>`的`execute()`函数有非空定义。与`shared_ptr<AbstractControlFlowNode>`相关的`execute()`函数定义比较简单，调用了`Traversal::traversal`函数。

​	首先在`traverse_qprog()`函数调用了`execute()`函数，其中传入的输入参数`cir_param`尚处于最初状态。这里的`execute()`函数的输入参数类型是`std::shared_ptr<AbstractQuantumCircuit>`对应的重载类型：

```C++
void TraverseByNodeIter::execute(std::shared_ptr<AbstractQuantumProgram>  cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter)
{
	if (nullptr == cur_node)
	{
		QCERR("param error");
		throw std::invalid_argument("param error");
	}
	auto aiter = cur_node->getFirstNodeIter();
	if (aiter == cur_node->getEndNodeIter())
		return;
	auto pNode = std::dynamic_pointer_cast<QNode>(cur_node);
	if (nullptr == pNode)
	{
		QCERR("pNode is nullptr");
		throw std::invalid_argument("pNode is nullptr");
	}
	while (aiter != cur_node->getEndNodeIter())
	{
		auto next = aiter.getNextIter();
		Traversal::traversalByType(*aiter, pNode, *this, cir_param, aiter);
		aiter = next;
	}
}
```

​	从函数定义可以看出，前3\~16行都是关于边界状况的检查。该函数的主要部分在于17\~22行中对`aiter`，即输入参数的`cur_node`量子程序结点迭代器中的每个结点进行访问。

​	`Traversal::traversalByType()`函数的整体是一个if...else结构，在本例中，根据`aiter`的结点类型确定程序执行内容：

```c++
    template<typename T,typename... Args>
    static void traversalByType(std::shared_ptr<QNode>  node, std::shared_ptr<QNode> parent_node, T & func_class, Args&& ... func_args)
    {
      int iNodeType = node->getNodeType();
      ...
    }
```

​	例如对于`iNodeType`结点为`GATE_NODE`的情况，程序在判断边界条件后，执行`funclass.execute()`函数：

```c++
if (GATE_NODE == iNodeType)
{
    auto gate_node = std::dynamic_pointer_cast<AbstractQGateNode>(node);
    if (!gate_node)
    {
        QCERR("Unknown internal error");
        throw std::runtime_error("Unknown internal error");
    }
   func_class.execute(gate_node, parent_node, std::forward<Args>(func_args)...);
}
```

​	也就是该函数的功能就是通过`iNodeType`判断应该调用哪种类型的`execute()`函数，不过我认为这个函数更加合理的方式是通过以下语句实现：

```C++
if (!node) {
  QCERR("Unknown internal error");
  throw std::runtime_error("Unknown internal error");
}
func_class.execute(node, parent_node, std::forward<Args>(func_args)...);
```

​	但是问题在于部分结点类型不是`QNode`的派生类，所以没有办法通过这种办法实现简单的调用。因此，该函数实现的重点还是在于`fun_class`的`execute()`函数定义。	