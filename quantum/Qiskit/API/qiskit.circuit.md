# Quantum Circuits(qiskit.circuit)

## Gate

简介：幺正量子门。创建一个新的量子门

```python
class Gate(name, num_qubits, params, label=None)
```

**参数说明**

+ name(`str`)-门的Qobj名

+ num_qubits(`int`)-量子门作用的量子比特数
+ params(`List`)-参数列表
+ label(`Optional`[`str`])-对于量子门的可选标签

**属性**

