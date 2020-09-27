# QPandaAPI

​	该文件夹主要保存QPanda API的阅读笔记以及关于所实现的某些功能的笔记。API笔记基于对QPanda的`include`文件夹下的头文件以及相应的库文件的阅读。分类上基于`include`的目录树。

## 代码阅读

### Core

​	这一部分记录在[Core.md](./Core.md)中

### Uitilities

​	这一部分记录在[Utilities.md](./Utilities.md)中

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

​	`init_gate_buf()`是在类`ProcessOnTraversing`中定义的函数。该函数利用`ProcessOnTraversing`中的数据对`m_cur_gate_buffer`进行初始化。

