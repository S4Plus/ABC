# Qiskit 学习笔记

## Qiskit特点

### Terra

​	Terra，四大元素之中的土元素，作为Qiskit剩余部分的基础。Terra提供了在电路和脉冲层面组成量子程序的基础，目的是优化对于特定设备的约束，并管理远程访问设备上批处理实验的执行。Terra通过定义界面，获得了理想的终端用户体验，以及优化层次，脉冲调度和后端通信高效处理。

​	Qiskit.Terra由下面的六个模块组成：

1. `qiskit.circuit`
2. `qiskit.pulse`
3. `qiskit.transpiler`
4. `qiskit.provider`
5. `qiskit.quantum_info`
6. `qiskit.visualization`

## Qiskit安装

### Github学习

​	Git是目前世界上最先进的分布式版本控制系统，不需要服务器端软件，就可以运作版本控制，使得源代码的发布和交流极其方便。git的速度很快，这对于诸如Linux内核这样的大项目来说自然很重要。git最为出色的是它的合并追踪（merge tracing）能力。而**GitHub**是通过Git进行版本控制的软件源代码托管服务平台。

​	关于git的具体教程可以参考[廖雪峰的Git教程](https://www.liaoxuefeng.com/wiki/896043488029600)。接下来以Qiskit为例，简单介绍如何从Github上clone项目：

1. 首先，找到你想要clone的项目在Github中的位置。以Qiskit为例，其代码仓库在Github中的网址为[Qiskit](https://github.com/Qiskit/qiskit)；
2. 其次，在当前网址中的右上方找到绿色的**Code**按键，点击后会出现如下的视图

<img src="fig/1-1.png" style="zoom:50%">

3. 这时你有两种选择将代码拷贝到本地：

   <img src="fig/1-2.png" style="zoom:50%">

   + 选择红框中的按钮保存链接，在终端中(Linux, Mac)或者WSL中(Windows)中利用指令

     ```
     git clone CLONE_URL
     ```

     将代码库clone到当前所在目录下

   + 选择蓝框中的按钮下载ZIP包，下载完成后利用解压获得代码库

4. 之后，如果Github上的代码更新，可以在工作目录下通过`git pull`指令将新的版本“拉”到本地

### 辅助工具安装

​	在安装Qiskit包之前，我们需要安装`anaconda`和`jupyter`两项工具。

#### Anaconda

##### 介绍

​	Anaconda（[官方网站](https://link.zhihu.com/?target=https%3A//www.anaconda.com/download/%23macos)）就是可以便捷获取包且对包能够进行管理，同时对环境可以统一管理的发行版本。Anaconda包含了conda、Python在内的超过180个科学包及其依赖项。

##### 安装

​	可以在官网下载对应系统的包进行安装，可以参考网站[2]。

#### Jupyter安装

##### 介绍

​	Jupyter Notebook（前身是IPython Notebook）是一个基于Web的交互式计算环境，用于创建Jupyter Notebook文档。Notebook一词可以通俗地引用许多不同的实体，主要是Jupyter Web应用程序、Jupyter Python Web服务器或Jupyter文档格式（取决于上下文）。

##### 安装

在新创建的conda环境中，利用pip包管理器安装即可：

```
pip3 install jupyter notebook
```

此时你可以通过`which jupyter`指令看到jupyter在你的新环境中，例如对于我来说，我的虚拟环境名为`qiskit`，其输出为：

```
/home/parallels/Documents/Anaconda/envs/qiskit/bin/jupyter
```

也可以使用conda安装：

```
conda install jupyter
```

### Qiskit 安装[4]

#### 安装

​	创建一个只有安装Python的最小环境：

```
conda create -n name_of_my_env python=3.8
```

​	激活你的环境：

```
conda activate name_of_my_env
```

​	接下来，安装Qiskit包，其中包含了Terra, Aer, Ignis和Aqua

```sh
pip install qiskit
```

​	如果包成功安装，你可以用`pip list | grep qiskit`检查，结果如下：

```
qiskit                0.19.6
qiskit-aer            0.5.2
qiskit-aqua           0.7.3
qiskit-ibmq-provider  0.7.2
qiskit-ignis          0.3.3
qiskit-terra          0.14.2
```

​	如果你希望使用可视化功能，推荐安装visualization包作为额外需求：

```sh
pip install qiskit[visualization]
```

·	安装好了之后，你可以通过Python将其导入到环境中：

```Python
import qiskit
```

#### 访问IBM量子系统

​	完成了之前的步骤后，你就可以通过Qiskit.Aer提供的模拟器在自己的经典计算机上执行量子电路。如果你想要在真实机器上运行，你可以按照以下步骤：

1. 创建一个IBM账号；

2. 打开导航栏中的用户->**My Account**；
3. 点击**Copy token**将令牌复制到剪贴板；
4. 在本地执行以下指令以将API令牌存储在本地的配置文件`qiskitrc`当中，替代`MY_API_TOKEN`为你在剪贴板中存取的值：

```python
from qiskit import IBMQ
IBMQ.save_account(`MY_API_TOKEN`)
```

### Q&A

**问题一**

Q: 无法创建新环境。执行`conda create -n qiskit python=3.8`后终端输出

```sh
Collecting package metadata (repodata.json): done
Solving environment: done
```

然后直接退出，但是`conda env list`中没有qiskit。

A: 解决办法：利用`conda clean --all`清理安装包，之后再重新创建新环境。

**问题二**

Q: conda删除虚拟环境出错。在用conda建立虚拟环境后，未指定python版本

```sh
conda create -n env_name
conda remove -n env_name --all
```

出现：PackagesNotFoundError: The following packages are missing from the target environment:

A: 采用以下命令即可

```sh
conda env remove -n env_name
```

## Qiskit 使用

在工作目录下运行：

```
jupyter notebook
```

以我的电脑为例，之后会输出：

```
(qiskit)  chenmingyu@parallels-vm  ~/Documents/work/Qiskit  jupyter notebook
[I 18:16:24.346 NotebookApp] Serving notebooks from local directory: /home/parallels/Documents/work/Qiskit
[I 18:16:24.347 NotebookApp] The Jupyter Notebook is running at:
[I 18:16:24.347 NotebookApp] http://localhost:8888/?token=c1ce5048a8c3fd0cdeacd760e9f7c4fe8a5b27507a9941cf
[I 18:16:24.347 NotebookApp]  or http://127.0.0.1:8888/?token=c1ce5048a8c3fd0cdeacd760e9f7c4fe8a5b27507a9941cf
[I 18:16:24.347 NotebookApp] Use Control-C to stop this server and shut down all kernels (twice to skip confirmation).
```

此时代表jupyter已经运行在[网页](http://127.0.0.1:8888/?token=c1ce5048a8c3fd0cdeacd760e9f7c4fe8a5b27507a9941cf)上。

之后关于jupyter的使用请参照[3]，而当前文件夹下包含了以下若干个程序：

| 程序         | 完成功能                                                     |
| ------------ | ------------------------------------------------------------ |
| plot_H.ipynb | 程序来自[1]，主要完成了Qiskit的基本使用，如量子电路的构建和模拟器上运行量子电路 |
| Ep3.ipynb    | 程序来自[Qiksit视频系列](https://www.youtube.com/playlist?list=PLOFEBzvs-Vvp2xg9-POLJhQwtVktlYGbY)的Ep3，部分内容与上一个程序相同，主要对比了在模拟器上运行和在真实机器上运行之间的差别。在程序中实现了在IBMQ机器运行程序 |
| Ep4.ipynb    | 程序来自[Qiskit视频系列](https://www.youtube.com/playlist?list=PLOFEBzvs-Vvp2xg9-POLJhQwtVktlYGbY)的Ep4，程序分别用Bloch球，状态向量和测量的方式表示了量子态 |
| Ep5.ipynb    | 程序来自[Qiskit视频系列](https://www.youtube.com/playlist?list=PLOFEBzvs-Vvp2xg9-POLJhQwtVktlYGbY)的Ep5，程序利用量子线路实现了量子信息的传送 |

### 注意事项

1. 如果在jupyter使用过程中出现ImportError，请查看Jupyter当前使用的Python版本是否是包所安装的对应版本。

## 参考

[1] [Getting Started with Qiskit](https://qiskit.org/documentation/getting_started.html)

[2] [Anaconda介绍、安装及使用教程](https://zhuanlan.zhihu.com/p/32925500)

[3] [Jupyter Notebook介绍、安装及使用教程](https://zhuanlan.zhihu.com/p/33105153)

[4] [Installing Qiskit](https://qiskit.org/documentation/install.html)

