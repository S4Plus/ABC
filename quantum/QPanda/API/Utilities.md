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
  virtual void run_traversal(QProg src_prog, const QVec qubits = {});
  
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

## 问题清单