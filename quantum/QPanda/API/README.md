# QPandaAPI

​	该文件夹主要保存QPanda API的阅读笔记以及关于所实现的某些功能的笔记。API笔记基于对QPanda的`include`文件夹下的头文件以及相应的库文件的阅读。分类上基于`include`的目录树。

## 代码阅读

这一部分根据`include/`头文件目录树进行分类，放在不同的markdown文件当中：

| 文件夹名                                 | 备注 |
| ---------------------------------------- | ---- |
| [include/Core/QuantumCircuit](./Core.md) |      |
| [include/Core/Utilities](./Utilities.md) |      |

## 量子线路替换

​	这个部分分为两个部分，第一个是读取QPandaConfig.json中所保存的替换线路数据，第二个部分是程序实现量子线路替换算法。

​	从`QCircuitOptimizer.test.cpp`以及QPanda工作人员的介绍看来，对量子线路优化的入口函数是`QPanda::sub_cir_optimizer`。注意这里不要与`QCircuitOptimizer::sub_cir_optimizer`弄混。

```C++
void QPanda::sub_cir_optimizer(QCircuit& src_cir, std::vector<std::pair<QCircuit, QCircuit>> optimizer_cir_vec, const OptimizerFlag mode)
{
  flattern(src_cir);
  
  QCircuitOptimizer tmp_optimizer;
  for (auto& optimizer_cir_pair : optimizer_cir_vec)
	{
		tmp_optimizer.register_optimize_sub_cir(optimizer_cir_pair.first, optimizer_cir_pair.second);
	}
	tmp_optimizer.register_single_gate_optimizer(mode);
	tmp_optimizer.run_optimize(src_cir/*, used_qubits*/);

	flatten(tmp_optimizer.m_new_prog, true);
	src_cir = QProgFlattening::prog_flatten_to_cir(tmp_optimizer.m_new_prog);
}
```

​	该函数具有两个重载版本，`src_cir`的数据类型分别为`QCircuit&`和`QProg&`。结合`QCircuitOptimizer.test.cpp`，输入参数`src_cir`显然是需要进行优化的线路。而`optimizer_cir_vec`从函数实现看来，其保存着需要进行替换的线路对。其函数的实现也可以看出这一点：

```C++
void QCircuitOptimizer::register_optimize_sub_cir(QCircuit sub_cir, QCircuit replace_to_cir) {
  OptimizerSubCir tmp_optimizer_sub_cir;
  tmp_optimizer_sub_cir.target_sub_cir = sub_cir;
  tmp_optimizer_sub_cir.replace_to_sub_cir = replase_to_cir;
  m_optimizer_cir_vec.push_back(tmp_optimizer_sub_cir);
}
```

​	在`QCircuitOptimizer.test.cpp`中，输入的`optimizer_cir_vec`是一个未被赋值的线路对向量

```c++
std::vector<std::pair<QCircuit, QCircuit>> optimizer_cir;
sub_cir_optimizer(prog, optimizer_cir, QCircuitOptimizer::Merge_U3);
```

​	而在`QCircuitOptimizer.h`中的`cir_optimizer_config`函数中：

```C++
void cir_optimizer_by_config(T &src_cir, const OptimizerFlag mode = QCircuitOPtimizer::Merge_H_X) {
	std::vector<std::pair<QCircuit, QCircuit>> optimitzer_cir;
	QCircuitOptimizerConfig::get_instance().get_replace_cir(optimitzer_cir);
	sub_cir_optimizer(src_cir, optimitzer_cir, mode);
}
```

​	推测其中`optimizer_cir_vec`从`QCircuitOptimizerConfig`类中获得了其中的替换线路数据。这一部分信息放到变量`optimizer_cir`中传入`sub_cir_optimizer()`函数中。结合以上两段对`sub_cir_optimizer()`函数的调用，推测`optimizer_cir_vec`传入的是替换线路的数据，而`mode`变量传入的是线路优化的策略。

​	`sub_cir_optimizer`完成的主要功能就是：添加用户自定义的替换线路对，并执行优化函数`run_optimize()`。而`run_optimize`将输入参数`src_prog`和`b_enable_I`保存到类的私有成员变量当中，并调用`run_traversal()`函数。

```C++
void ProcessOnTraversing::run_traversal(QProg src_prog, const QVec qubits /*= {}*/)
{
	if (qubits.size() == 0)
	{
		get_all_used_qubits(src_prog, m_qubits);
	}
	else
	{
		m_qubits = qubits;
	}
	init_gate_buf();
	traverse_qprog(src_prog);
	PTrace("finished traverse_qprog.");
	//At the end of the traversal, call process again and clear all the gate-buf
	process(true);
}
```

​	如果传入的`qubits`参数为空集，那么就直接从`QProg`类型的`src_prog`类型中获取量子比特个数，将其保存在成员变量`m_qubits`中。如果不是空集，则将其数据传给`m_qubits`。

​	以下分三个部分讨论使用的三个函数：

**init_gate_buf()**

​	`init_gate_buf()`是在类`ProcessOnTraversing`中定义的函数。该函数利用`ProcessOnTraversing`类中的数据对`m_cur_gate_buffer`进行初始化。

**tracerse_qprog()**

```C++
void TraverseByNodeIter::traverse_qprog(QProg qprog) {
  NodeIter itr = NodeIter();
  auto param = std::make_shared<QCircuitParam>();
  execute(prog.getImplementationPtr(), nullptr, *param, itr);
}
```

​	观察`traverse_qprog()`函数，可以得知这个函数的功能集中在`execute()`函数的执行上。关于`execute()`函数，从其声明看来，类似于语法分析中访问者模式的`visit()`函数。需要注意的是，这里调用的是`TraverseByNodeIter::execute()`函数，而不是`ProcessOnTraversing::execute()`函数。关于每个函数的具体分析，见相关的代码阅读部分。

​	总之，只有`execute(std::shared_ptr<AbstractQGateNode> cur_node,...)`在本算法中是重要的，而`execute(std::shared_ptr<AbstractQuantumCircuit>,...)`是必要的存在，除此之外函数定义都为空。

​	下面来看对于`AbstractQGateNode`的`execute()`函数：

```c++
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

​	`add_gate_to_buffer()`函数正如其名，将当前结点添加到`m_cur_gates_buffer`中。其具体实现却没有这么简单：

```c++
void ProcessOnTraversing::add_gate_to_buffer(NodeIter iter, QCircuitParam & cir_param, std::shared_ptr<QNode> parent_node, OptimizerSink & gates_buffer)
{
  auto gate_node = std::dynamic_pointer_cast<AbstractQGateNode>(*iter);
  QVec gate_qubits;
	gate_node->getQuBitVector(gate_qubits);

	QVec target_qubits_int;
	QVec control_qubits_int;

	for (auto i : gate_qubits)
	{
		target_qubits_int.push_back(i);
	}

	QVec control_qubits;
	gate_node->getControlVector(control_qubits);

	for (auto i : control_qubits)
	{
		control_qubits_int.push_back(i);
	}

	for (auto i : cir_param.m_control_qubits)
	{
		control_qubits_int.push_back(i);
	}

	QVec total_qubits;
	total_qubits.insert(total_qubits.end(), target_qubits_int.begin(), target_qubits_int.end());
	total_qubits.insert(total_qubits.end(), control_qubits_int.begin(), control_qubits_int.end());
	
  size_t layer = get_node_layer(total_qubits, gates_buffer);

	//PTrace("On layer: %lld\n", layer);

	pOptimizerNodeInfo tmp_node = std::make_shared<OptimizerNodeInfo>(iter, layer,
	target_qubits_int, control_qubits_int, (GateType)(gate_node->getQGate()->getGateType()), parent_node, cir_param.m_is_dagger);
	for (const auto& i : total_qubits)
	{
		gates_buffer.at(i->get_phy_addr()).push_back(tmp_node);
	}
}
```

​	`gate_node`变量保存需要插入的门结点。`gate_qubits`和`target_qubits_int`保存了当前门结点的目标比特，`control_qubits`和`control_qubits_int`则是保存了当前门结点的控制比特信息。

​	之后，在第28行到第30行将目标量子比特和控制量子比特插入到局部变量`total_qubits`当中。

​	之后利用得到的信息生成`pOptimizerNodeInfo`数据结构，将其插入到`gates_buffer`形参中，即`m_cur_gates_buffer`中。

​	回到`execute()`函数当中，在函数体的后半部分还有一个条件判断。这个条件判断的意义在于根据当前的层数决定是否对当前线路进行处理。而处理的核心函数就是`process()`函数。
**process()**

​	下面重点介绍算法的核心函数`process()`。由于`TraverseByNodeIter`类并没有定义纯虚函数`process()`，实际中对它的调用是对它的子类`QCircuitOptimizer`所定义的`process()`函数的调用。该函数定义如下：

```C++
void QCircuitOPtimizer::process(bool on_travel_end /*= false*/)
{
	PTrace("On process...\n");
	do_optimizer();

	//pop some layers to new circuit
	clean_gate_buf_to_cir(m_new_prog, on_travel_end);
	PTrace("process end.\n");
}
```

​	该函数功能由`do_optimizer()`和`clean_gate_buf_to_cir()`两个函数实现。总体而言，`process()`函数完成的功能是将`m_cur_gates_buffer`中的线路进行优化，然后利用新的线路生成新的量子程序`m_new_prog`。

​	

