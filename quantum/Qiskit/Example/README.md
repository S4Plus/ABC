## Qiskit 样例程序说明

### 代码样例

qiskit-terra在github上提供了[example文件夹](https://github.com/Qiskit/qiskit-terra/tree/master/examples)，包含了python和qasm的量子程序示例。python文件夹中：

+ [using_qiskit_terra_level_0.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/using_qiskit_terra_level_0.py)

	+ **概述** 给新手展示如何使用Qiskit-terra编程。该样例展示了使用qiskit-terra的基本方法，建立了电路并且让其在BasicAer提供的模拟器上运行。
	+ **问题** 并没有在程序中找到注释中提到的"IBMQ(remote IBMQ provider)"和"transpiler function for level 1 user"

+ [circuit_draw.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/circuit_draw.py)

	+ **概述** 展示了如何利用Qiskit作图
	+ **特点** 在做图上调用了[QuantumCircuit](https://qiskit.org/documentation/stubs/qiskit.circuit.QuantumCircuit.html#qiskit.circuit.QuantumCircuit)类本身所具有的draw方法，而非使用更加丰富可视化工具的[`qiskit.visualization`](https://qiskit.org/documentation/apidoc/visualization.html#module-qiskit.visualization)模块

+ [commutation_relation.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/commutation_relation.py)

	+ **概述** 展示了量子线路中存在的对易关系，并利用优化器根据对易关系对量子线路做简化
	+ **特点** 程序中使用了两个[`qiskit.transpiler`](https://qiskit.org/documentation/apidoc/transpiler_passes.html)的两个Pass：[`commutation_analysis`](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/passes/optimization/commutation_analysis.py)和[`commutative_cancellation`](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/passes/optimization/commutative_cancellation.py)。根据程序中的注释，前者实现的是找到DAG结点之间的对易关系，后者则是根据对易关系消除冗余门。
	+ **问题** 原代码中往阶段管理器添加了两个Pass以实现消除对易门的功能，但是如果只添加一个`commutative_cancellation`Pass也是能够得到消除对易门的`new_circuit`>

+ [ghz.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/ghz.py)

	+ **概述** 建立得到GHZ态(Greenberger–Horne–Zeilinger态)的量子电路
	+ **特点** 比较值得注意的应该就是GHZ态的构造方法，在Qiskit的使用上没有什么特别的

+ [initialize.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/initialize.py)

	+ **概述** 利用初始门创建随机纯态
	+ **特点**  
		+ 调用了[QuantumCircuit.initialize](https://qiskit.org/documentation/stubs/qiskit.circuit.QuantumCircuit.initialize.html#qiskit.circuit.QuantumCircuit.initialize)接口初始化线路状态，根据输入的向量添加黑盒使线路得到想要的初态；
		+ 利用模拟器运行的结果与想要的结果一致

+ [load_qasm.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/load_qasm.py)

	+ **概述** 如何加载QASM文件的样例
	+ **说明** 利用[QuantumCircuit.from_qasm_file](https://qiskit.org/documentation/stubs/qiskit.circuit.QuantumCircuit.from_qasm_file.html#qiskit.circuit.QuantumCircuit.from_qasm_file)接收一个QASM文件并产生一个QuantumCircuit对象，对于较大规模的电路构建挺有用的

+ [qft.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/qft.py)

	+ **概述** QFT的示例程序

+ [rippleadd.py](https://arxiv.org/abs/quant-ph/0410184)

	+ **概述** 基于[论文](https://arxiv.org/abs/quant-ph/0410184)设计的ripple adder电路

+ [stochastic_swap.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/stochastic_swap.py)

	+ **概述** 使用`SchocasticSwap`优化阶段的实例
	+ **说明** 在[schocastic_swap.py](https://github.com/Qiskit/qiskit-terra/blob/master/qiskit/transpiler/passes/routing/stochastic_swap.py)中对`SchocaticSwap`类进行了注释：将一个DAG线路通过添加swap操作映射到连通映射，使用的是随机算法。注意：
		1. 测量可能会在线路中出现而且之后有swap操作，这将会导致对同一量子比特的重复测量。短期内的实验没办法实现这这些电路，所以实验过程中需要多注意使用该映射的实验后端目标。
		2. 我们并不基于输入态为0来简化电路

+ [teleport.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/teleport.py)

	+ **概述** 量子态传输实例

	以下是python文件夹下的ibmq文件夹内容，其中包含的程序均实现了远程访问IBMQ设备：

+ [ibmq/ghz.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/ibmq/ghz.py)

	+ **概述** 相对于ghz.py，该程序利用的是远程IBMQ设备运行程序

	+ **特点** 相对于利用模拟器运行的ghz程序，利用真实量子机器运行中的噪声使得结果并不满足1：1的关系，而是存在其他状态。下面是本地运行的结果:

		```
		Physical device (ibmq_london) : 
		{'10001': 2, '01111': 27, '01101': 6, '11011': 10, '11000': 25, '01011': 1, '10010': 1, '10110': 5, '11111': 251, '00001': 8, '11101': 53, '00110': 14, '11010': 5, '00010': 113, '00100': 10, '11001': 4, '10111': 14, '00101': 2, '10000': 6, '11110': 52, '11100': 20, '10011': 1, '00011': 16, '10101': 1, '01100': 4, '01000': 3, '01010': 1, '00111': 7, '01110': 4, '00000': 358}
		```

		对比ghz.py运行的结果：

		```
		Qasm simulator : 
		{'00000': 526, '11111': 498}
		```

	+ **说明** 在访问IBMQ设备上，调用了[`qiskit.provider.ibmq`](https://qiskit.org/documentation/apidoc/ibmq_provider.html?highlight=qiskit provider ibmq#module-qiskit.providers.ibmq)模块的[`least_busy`](https://qiskit.org/documentation/stubs/qiskit.providers.ibmq.least_busy.html#qiskit.providers.ibmq.least_busy)接口。least_busy接口的描述如下：

		> Return the least busy backend from a list.
		>
		> Return the least busy available backend for those that have a `pending_jobs` in their `status`. Note that local backends may not have this attribute.
		>
		> Parameters：**backends** (`List`[`BaseBackend`]) – The backends to choose from.

		在本例中，使用方法如下：

		```python
		provider = IBMQ.load_account()
		
		# Second version: real device
		least_busy_device = least_busy(
		  		provider.backends(simulator=False,
		                       filters = lambda x: x.configuration().n_qubits > 4))
		```

		`least_busy`的参数是后端列表`backends`，在其中还能添加筛选条件。

+ [ibmq/qft.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/ibmq/qft.py)

	+ **概述** qft.py在真实设备上运行的版本，在线路的构造上与qft.py一致，差别仅在于provider

+ [ibmq/hello_quantum.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/ibmq/hello_quantum.py)

	+ **概述** 制备贝尔态，分别利用模拟器(local provider)和IBMQ设备(remote provider)运行量子线路
	+ **特点** 相对简单而且程序的注释比较完善

+ [ibmq/using_qiskit_terra_level_0.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/ibmq/using_qiskit_terra_level_0.py)

	+ **概述** 新手版基本的Qiskit-Terra的实例，相比于ibmq/hello_quantum.py要复杂一点

+ [ibmq/using_qiskit_terra_level_1.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/ibmq/using_qiskit_terra_level_1.py)

	+ **概述** 普通版Qiskit-Terra使用。

	+ **特点** 它创建了几个简单的电路，并利用[qiskit.compiler.transpile](https://qiskit.org/documentation/stubs/qiskit.compiler.transpile.html#qiskit.compiler.transpile)对生成的电路进行转译使之与后端匹配。然后创建qobj作为后端运行的容器，将其提交到不同的后端上。这对于想对比同样的电路在不同的后端上运行的效果或者仅想改变运行参数是有用的。而利用阶段管理器对阶段进行控制，这是level 2用户需要学会的东西。

	+ **说明** qobj的创建不能直接利用创建的电路，需要根据实验用的后端`least_busy_device`先对线路进行处理使之兼容，再得到qobj

		```Python
		# Transpile the circuits to make them compatiable with the experimental backend
		[qc1_new, qc2_new] = transpile(circuits=[qc1, qc2], backend=least_busy_device)
		
		# Assemble the two circuits into a runnable obj
		qobj = assemble([qc1_new, qc2_new], shots=1000)
		```

+ [ibmq/using_qiskit_terra_level_2.py](https://github.com/Qiskit/qiskit-terra/blob/master/examples/python/ibmq/using_qiskit_terra_level_2.py)

	+ **概述** 高级版Qiskit-Terra使用。程序构建了若干程序，并用阶段管理器对线路进行处理。

