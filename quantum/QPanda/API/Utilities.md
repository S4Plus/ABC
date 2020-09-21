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

​	该结构用于在线路替换算法的执行过程中保存门节点的相关信息。`QGate`的表达能力

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
```



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

**QCircuitOptimizer**

```c++
class QCircuitOptimizer : public ProcessOnTraversing
```

