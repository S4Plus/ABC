# Utilities

## Tools

### ProcessOnTraversing

**OptimizerNodeInfo**

```C++
struct OptimizerNodeInfo {
  NodeIter m_iter;
  size_t m_layer;
  QVec m_target_qubits;
  QVec m_ctrl_qubits;
  GateType m_type;
  std::shared_ptr<QNode> m_parent_node;
  int m_sub_graph_index;
}
```

​	该结构用于在线路替换算法的执行过程中保存门节点的相关信息。`QGate`的表达能力不足以表达在线路替换算法中所需要的内容，因此引入了这一结构。

​	同时，在`ProcessOnTraversing.h`文件中还基此定义了其他数据结构：

```c++
using pOptimizerNodeInfo = std::shared_ptr<OptimizerNodeInfo>;
using GatesBufferType = std::pair<size_t, std::list<pOptimizerNodeInfo>>;
using OptimizerSink = std::map<size_t, std::list<pOptimizerNodeInfo>>;
```

​	其中`OptimizerSink`将量子比特映射到与其相关的量子门信息。

**ProcessOnTraversing**

```C++
class ProcessOnTraversing : protected TraverseByNodeIter
{
public:
  using layer_iter_seq = TopologSequence<std::pair<size_t, NodeIter>>; 
public:
  virtual ~ProcessOnTraversing() {}
  
  virtual void process(bool on_travel_end) = 0;
  virtual void run_traversal(QProg src_prog, const QVec qubits = {});
  
  void execute(std::shared_ptr<AbstractQGateNode> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) override;
	void execute(std::shared_ptr<AbstractQuantumMeasure> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) override;
	void execute(std::shared_ptr<AbstractQuantumReset> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) override;
	void execute(std::shared_ptr<AbstractControlFlowNode> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) override;
	void execute(std::shared_ptr<AbstractClassicalProg> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) override;
	void execute(std::shared_ptr<AbstractQuantumCircuit> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) override;
	void execute(std::shared_ptr<AbstractQuantumProgram> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter) override;
  
protected:
  virtual void add_gate_to_buffer(NodeIter iter, QCircuitParam &cir_param, std::shared_ptr<QNode> parent_node, OptimizerSink& gates_buffer);
	virtual void add_non_gate_to_buffer(NodeIter iter, NodeType node_type, QVec gate_qubits, 
     QCircuitParam &cir_param, OptimizerSink& gates_buffer, 
     std::shared_ptr<QNode> parent_node = nullptr);
	virtual size_t get_node_layer(QVec gate_qubits, OptimizerSink& gate_buffer);
	virtual size_t get_min_include_layers();
  // 初始化QGateBuffer
	void init_gate_buf() {
		for (const auto& item : m_qubits)
		{
			m_cur_gates_buffer.insert(GatesBufferType(item->getPhysicalQubitPtr()->getQubitAddr(), std::list<pOptimizerNodeInfo>()));
		}
	}
  
protected:
  QVec m_qubits;
  OptimizerSink m_cur_gates_buffer;
  size_t m_min_layer;
}
```

**成员变量说明**

| 成员名             | 数据类型      | 备注                                                         |
| ------------------ | ------------- | ------------------------------------------------------------ |
| m_qubits           | QVec          | 保存Qubit的向量<br>在init_gate_buf函数中利用m_qubits包含的信息初始化m_cur_gate_buffer |
| m_cur_gates_buffer | OptimizerSink | OptimizerSink结构是一个从qubit到<br/>`std::list<pOptimizerNodeInfo>`的结构，<br/>保存的是与每个量子比特相关的结点信息。<br/>这里的节点可以是量子门。 |
| m_min_layer        | size_t        |                                                              |

**成员函数说明**

​	其中，值得注意的是一系列的`execute()`函数。这些`execute()`函数服务于线路替换算法过程，其功能类似于访问者模式中的`visit()`函数。以第一个输入参数`Abstract__`（各结点基类）进行区分，这些`execute()`函数分别对应于`NodeType`中的`GATE_NODE`, `MEASURE_NODE`, `RESET_NODE`, `WHILE_START_NODE`, `CIRCUIT_NODE`和`PROG_NODE`。其中，缺少与`AbstractClassicalProg`所对应的`NodeType`。

​	另外，`ProcessOnTraversing`类并没有实现父类`TraverseByNodeIter`的`traverse_qprog()`函数。所以在`run_traversal()`过程中会直接调用父类的函数。

​	`execute()`函数的第一个参数为需要访问的结点，第二个参数类似于语法树中的父节点。

```C++
void ProcessOnTraversing::execute(std::shared_ptr<AbstractQGateNode> cur_node, std::shared_ptr<QNode> parent_node, QCircuitParam &cir_param, NodeIter& cur_node_iter)
{
	// push_back cur_node_iter to related qubits
	add_gate_to_buffer(cur_node_iter, cir_param, parent_node, m_cur_gates_buffer);

	//optimizer
	size_t min_include_layers = get_min_include_layers();
	if (min_include_layers > MAX_INCLUDE_LAYERS)
	{
		process(false);
	}
}
```

​	其中，`MAX_INCLUDE_LAYERS`是一个在同一文件中定义的宏变量，其值为8192。该函数的功能如下：

+ 将当前门结点加入到`m_cur_gates_buffer`中
+ 如果当前门的层数已经高于`MAX_INCLUDE_LAYERS`时，对其进行处理

​    其余的`execute()`函数均调用父类`TraverseByNodeIter`对应的`execute()`函数进行处理，而这些函数都是空定义。所以这一系列函数中，唯一有用的函数。



### QCircuitOptimizer

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
  // 替换的目标子线路
  QCircuit target_sub_cir;
  // 被替换的源子线路
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

**QCircuitOptimizer**

```C++
class QCircuitOPtimizer : public ProcessOnTraversing
{
public:
  QCircuitOptimizer();
  ~QCircuitOptimizer();
  void process(bool on_travel_end = false) override;
	void register_single_gate_optimizer(const OptimizerFlag mode);
	void register_optimize_sub_cir(QCircuit sub_cir, QCircuit replase_to_cir);
	void run_optimize(QProg src_prog, const QVec qubits = {}, bool b_enable_I = false);
  // 虽然代码中是replase_sub_cir，但是在笔记中会记作replace_sub_cir
  QProg replase_sub_cir(std::function<QCircuit(const size_t)> get_cir_fun);
	void sub_cir_optimizer(const size_t optimizer_sub_cir_index);
	void do_optimizer();
protected:
  void mark_sub_graph(const std::vector<TopologSequence<pOptimizerNodeInfo>>& sub_graph_vec);
public:
	QProg m_new_prog;
	/* Optimise mode */
	static const unsigned char Merge_H_X = 1;
	static const unsigned char Merge_U3 = (1 << 1);
	static const unsigned char Merge_RX = (1 << 2);
	static const unsigned char Merge_RY = (1 << 3);
	static const unsigned char Merge_RZ = (1 << 4);
private:
  QProg m_sub_prog;
	std::vector<std::shared_ptr<AbstractCirOptimizer>> m_optimizers;
	std::vector<OptimizerSubCir> m_optimizer_cir_vec;
	TopologSequence<pOptimizerNodeInfo> m_topolog_sequence;
	FindSubCircuit m_sub_cir_finder;
	std::vector<std::map<size_t, Qubit*>> m_sub_graph_qubit_map_vec;/* qubit mapping of each sub-graph */
	size_t m_cur_optimizer_sub_cir_index;
	std::vector<QProg> m_tmp_cir_vec; /* Temporary QCircuits for storing QGate nodes */
	std::vector<std::vector<double>> m_angle_vec; /* Angle variable of each sub-graph */ 
	bool m_b_enable_I;
}
```

​	

### JsonConfigParam

**JsonConfigParam**

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

+ 下面结合cpp文件具体介绍各个函数
+ `bool JsonConfigParam::load_config(const std::string file_name)`
	+ 首先，程序利用`std::string::substr()`函数和`std::transform()`函数提取出输入参数`file_name`的最后五个字符作为后缀。并将其与`.json`进行比对
	+ 如果后缀名不是`.json`，说明此时输入的`config_data`是配置数据，将其直接赋值给私有变量`m_json_content`；
	+ 如果后缀名是`.json`，说明此时输入的`config_data`是配置文件名。利用`reader`将内容读取出来放到`m_json_content`中；

**QCircuitOptimizerConfig**

```c++
class QCircuitOptimizerConfig {
#define ANGLE_VAR_BASE 1024
public:
  static QCircuitOptimizerConfig& get_instance() {
    static QCircuitOptimizerConfig _instance;
    return _instance;
  }
	~QCircuitOptimizerConfig();
  bool get_replace_cir(std::vector<std::pair<QCircuit, QCircuit>>& replace_cir_vec);
private:
  QCircuitOptimizerConfig();
	QCircuit read_cir(const rapidjson::Value& gates);
	QGate build_sing_ratation_gate(std::string gate_name, Qubit* target_qubit, double angle);
	QGate build_sing_gate(std::string gate_name, Qubit* target_qubit);
	QGate build_double_gate(std::string gate_name, QVec qubits);
	QGate build_double_ratation_gate(std::string gate_name, QVec qubits, double angle);
	double angle_str_to_double(const std::string angle_str);
private:
	JsonConfigParam m_config_file;
	CPUQVM m_qvm;
	QVec m_qubits;
}
```

​	从该类的名字看来，该类保存的是Json文件中定义的关于QCircuitOptmizer，即量子线路优化有关的配置。在构造函数中：

```C++
QCircuitOptimizerConfig::QCircuitOptimizerConfig()
{
  m_config_file.load_config(CONFIG_PATH);
  m_qvm.init();
}
```

​	其中`m_config_file`是`QCircuitOptimizerConfig`的成员变量，数据类型为`JsonConfigParam`。从这来看，`m_config_file`用来保存和读取Json文件中的配置信息。其中比较重要的函数为`get_replace_cir`(代码中写的是`get_replase_cir`)和`read_cir`。

## QProgInfo

### NodeInfo

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

## 问题清单

1. 在`ProcessOnTraversing`类所定义的`execute()`函数组中，与`AbstractClassicalProg`对应的`execute()`函数缺少执行部分。同时，也没有在`NodeType`中与该结点对应的类型名。`AbstractClassicalProg`对应的结点类型在现实中对应的实体是什么？是否多余？

​        处理经典节点的接口，在这个场景下用不到，但这个接口是从父类父类继承下来的，所以不需要管

2. 一个可能的改进点：在遍历线路的过程中所使用的各个`execute()`函数所访问的节点设置为相同父类的派生类，可以使程序结构比较简单。目前虽然该方法类似访问者模式，但是`execute()`访问的结点，例如`AbstractQGateNode`等，都不具有父类。

> 赵工认为，这种方案最终还需要判断类型，然后对于不同的类型，进行不同的处理，和现在的是一样的。其实现在的做法就是访问者模式，基础框架有类型判断，派生类只需要实现不同节点的处理函数就行，不需要进行类型判断。
>
> ​	而且这样每次实现一个业务都需要进行类型判断，在大部分情况下，circuit或者prog等节点的处理方式是一样的，所以这些应该放到基类里面。在具体的个人业务里面，只需要实现要特殊处理的节点函数就行。比如大部分上层业务只需要处理gate   measure  等节点就行

​	不过我感觉他没有理解我的意思。

3. 在`ProcessOnTraversing::run_traversal()`调用的`process()`函数对应的是哪一个`process()`函数。`ProcessOnTraversing`类定义的`process()`函数是纯虚函数。

​    这是一个纯虚函数，`process()`函数的真正实现在优化实现类中，即`QCircuitOptimizer`类中。`ProcessOnTraversing`是一个抽象的处理框架，只负责遍历，遍历到一定程度，就会调用派生类的`process()`处理函数。

