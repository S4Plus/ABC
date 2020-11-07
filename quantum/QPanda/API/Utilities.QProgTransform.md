# Utilities.QProgTransform

## TransformDecompistion

**DecomposeDoubleQGate**

```c++
class DecomposeDoubleQGate : public TraversalInterface<>
{
  
}
```

**DecomposeMultipleControlQGate**

```C++
class DecomposeMultipleControlQGate : public TraversalInterface<>
{
  public:
  	void execute(std::shared_ptr<AbstractQGateNode> cur_node, std::shared_ptr<QNode> parent_node);
  	//...
  private:
  	void QGateExponentArithmetic
}
```

## 功能

### 多控门分解

​		该功能的实现接口`decompose_multiple_control_qgate`位于`TransformDecompostion.cpp`文件最后：

```c++
void QPanda::decompose_multiple_control_qgate(QProg& prog, QuantumMachine *quantum_machine, const std::string& config_data/* = CONFIG_PATH*/)
{
	if (!(CheckMultipleControlQGate().exist_multiple_gate(prog)))
	{
		return;
	}

	QuantumMetadata meta_data(config_data);
	std::vector<string> vec_single_gates;
	std::vector<string> vec_double_gates;
	meta_data.getQGate(vec_single_gates, vec_double_gates);

	std::vector<std::vector<std::string>> gates(KMETADATA_GATE_TYPE_COUNT, vector<string>(0));
	std::vector<std::vector<std::string>> valid_gate(KMETADATA_GATE_TYPE_COUNT, vector<string>(0));
	for (auto& item : vec_single_gates)
	{
		gates[MetadataGateType::METADATA_SINGLE_GATE].emplace_back(item);
	}
	for (auto& item : vec_double_gates)
	{
		gates[MetadataGateType::METADATA_DOUBLE_GATE].emplace_back(item);
	}
	SingleGateTypeValidator::GateType(gates[MetadataGateType::METADATA_SINGLE_GATE],
		valid_gate[MetadataGateType::METADATA_SINGLE_GATE]);  /* single gate data MetadataValidity */
	DoubleGateTypeValidator::GateType(gates[MetadataGateType::METADATA_DOUBLE_GATE],
		valid_gate[MetadataGateType::METADATA_DOUBLE_GATE]);  /* double gate data MetadataValidity */

	auto p_transf_decompos = std::make_shared<TransformDecomposition>(valid_gate, gates, quantum_machine);
	p_transf_decompos->TraversalOptimizationMerge(prog);
}
```

