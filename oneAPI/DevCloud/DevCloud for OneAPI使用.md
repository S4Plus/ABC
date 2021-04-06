# DevCloud for OneAPI使用

首先登录到[Intel DevCloud](https://software.intel.com/content/www/cn/zh/develop/tools/devcloud.html)，选择oneAPI环境

## 配置连接

进入[示例界面](https://devcloud.intel.com/oneapi/get_started/baseToolkitSamples/)，按照提示进行配置

### windows

官方提供的连接方式对于windows只有通过Cygwin进行连接，不过所有的连接方式都是使用ssh进行连接，所以不需要下载Cygwin，而使用windows自带的ssh或者其他可使用ssh的软件均可进行连接。

这种方式无法选择自动配置，只能手动配置：

1. 下载SSH key，需要注意的是，示例界面下载的SSH key是一个空文件，需要进入[Documentation](https://devcloud.intel.com/oneapi/documentation/connect-with-ssh-linux-macos/)中相应文档中下载。下载后将SSH key 文件保存至自己的`.ssh`文件夹下。

2. 在`.ssh`文件夹下的`config`文件中添加`Devcloud`的连接信息：

   ```
   Host devcloud
   User uxxxxx	#自己的uid
   IdentityFile PATH—TO-YOUR-.SSH/devcloud-access-key-xxxxx.txt
   ProxyCommand ssh -T -i ~/.ssh/devcloud-access-key-xxxxx.txt guest@devcloud.intel.com
   ```

3. 使用`cmd`或`PowerShell`进行连接：

   ```powershell
   ssh devcloud
   ```

   由于网络原因延迟比较高，需要等待。

   也可直接使用`VScode`的`remote SSH`插件进行连接，不过在测试时发现延迟较高，经常会断开连接甚至连接不上。

### Linux

可以直接下载自动配置脚本进行config配置，配置好之后可以直接连接。

手动配置与Windows一致，不再赘述。

## DevCloud命令行工具

### 启动和管理工作

+ 创建工作脚本

  ```shell
  cat > job.sh
  #!/bin/bash
  source /opt/intel/inteloneapi/setvars.sh
  make run
  ```

  Ctrl+C关闭文件

+ 提交批处理作业

  ```shell
  qsub -l nodes=1:gpu:ppn=2 -d . job.sh
  ```

  注意：

  + `-l nodes=1:gpu:ppn=2`（小写L）用于为作业分配一个完整的GPU节点
  + `-d .`用于将当前文件夹配置为任务的工作目录
  + `job.sh`是在计算节点上执行的脚本

+ 使用交互模式

  ```shell
  qsub -I -l nodes=1:gpu:ppn=2 -d .
  ```

  注意：

  + `-I`（大写i）是用于请求交互式会话的参数。

+ 运行工作

  进入交互模式后

  ```shell
  bash job.sh
  ```

  即可执行

  使用

  ```shell
  exit
  ```

  即可退出交互模式

+ 测量工作执行时间

  直接将时间戳在脚本运行前后输出到标准输出中：

  ```bash
  #!/bin/bash
  
  echo
  echo start: $(date "+%y%m%d.%H%M%S.%3N")
  echo
  
  # TODO list
  
  echo
  echo stop:  $(date "+%y%m%d.%H%M%S.%3N")
  echo
  ```

+ 将一个作业配置为在另一个作业成功完成后自动触发

  以FPGA HW为例：

  1. 提交构建FPGA HW作业，并记下返回的作业ID XXXX：

     ```shell
     qsub -l nodes=1:fpga_compile:ppn=2 -d . build_fpga_hw.sh
     ```

  2. 提交FPGA HW执行作业时，请使用-Wdepend = afterok：[job_id]参数：

     ```shell
     qsub -l nodes=1:fpga_runtime:ppn=2 -d . run_fpga_hw.sh -W depend=afterok:XXXX
     ```

+ 更改最大作业时长

  默认情况下，所有作业会在6h标记处自动终止，如果作业需要6小时以上才能使用以下语法：

  ```shell
  qsub […] -l walltime=hh:mm:ss
  ```

+ 监控工作

  通常而言，有些工作不会很快完成，可以通过`watch`工具监控`qstat`显示的工作执行状态

  ```shell
  watch -n 1 qstat -n -1
  ```

+ 终止工作

  ```shell
  qdel <job_id>
  ```

### 访问计算节点

+ 请求交互式命令行

  ```shell
  qsub -I [...]
  ```

+ 按属性请求节点

  ```shell
  qsub [...] -l nodes=1:[property]:ppn=2
  ```

+ 按节点名称请求节点

  ```shell
  qsub [...] -l nodes=[node_name]]:ppn=2
  ```

+ 列出所有计算节点及其属性

  ```shell
  pbsnodes
  ```

+ 列出可用的计算节点

  ```shell
  pbsnodes -l free
  ```

### 列出计算节点属性

```shell
pbsnodes | sort | grep properties 
```

这些属性用于描述计算节点上可用的各种功能，例如：CPU类型和名称，加速器类型和名称，可用DRAM，互连类型，可用的加速器设备数量及其类型和预期或建议的用途。

一些属性描述了设备的类别：

+ core
+ fpga
+ gpu
+ xeon

其他属性按名称描述设备（包括nda）：

- arria10
- e-2176g
- gen9
- gold6128
- i9-10920x
- iris_xe_max
- plat8153

设备数量：

- dual_gpu
- quad_gpu

有可能使用的属性：

- batch
- fpga_compile
- fpga_runtime
- jupyter
- renderkit

### 使用特定的计算节点

用于将作业提交到托管GPU的计算节点的典型命令是：

```shell
qsub -l nodes=1:gpu:ppn=2 job_script.sh 
```

这会将作业脚本提交到托管GPU的随机计算节点。可能是`Intel®UHD Graphics P630`或`Intel®Iris®Xe MAX Graphics`。

可以具体使用某种特定的节点：

```shell
qsub -l nodes=1:iris_xe_max:ppn=2 job_script.sh 
```

该命令将请求到双或四 Intel®Iris®Xe MAX图形计算节点。为了请求双Intel®Iris®Xe MAX Graphics计算节点，需要同时使用iris_xe_max和dual_gpu属性：

```shell
qsub -l nodes=1:iris_xe_max:dual_gpu:ppn=2 job_script.sh 
```

同样的，有

```shell
qsub -l nodes=1:iris_xe_max:quad_gpu:ppn=2 job_script.sh 
```

### 传输文件

与一般的服务器一致，使用`scp`工具进行

上传：

```shell
scp file devcloud:~/
```

下载：

```bash
scp devcloud:~/file .
```

## 示例使用方式

### 下载示例库

1. 连接到DevCloud

   ```shell
   ssh devcloud
   ```

2. 下载示例库

   ```shell
   git clone https://github.com/oneapi-src/oneAPI-samples.git
   ```

### 执行某一示例

进入某一示例文件夹下

#### 以批处理方式运行

+ 创建作业脚本:

  + build.sh:

    ```bash
    #!/bin/bash
    source /opt/intel/inteloneapi/setvars.sh
    make clean
    make all
    ```

  + run.sh:

    ```bash
    #!/bin/bash
    source /opt/intel/inteloneapi/setvars.sh
    make run
    ```

+ 构建并运行

  以批处理方式提交的作业被放入队列中，等待必要的资源（计算节点）可用。作业将基于具有请求的属性或标签的第一个可用节点以先到先得的方式执行。

  1. 在gpu节点上构建示例

     ```shell
     qsub -l nodes=1:gpu:ppn=2 -d . build.sh
     ```

  2. 构建作业成功完成后，在gpu节点上运行示例。

     ```shell
     qsub -l nodes=1:gpu:ppn=2 -d . run.sh
     ```

  3. 确定作业是否完成的最佳方法是使用qstat实用程序。作业终止时，会将几个文件写入磁盘：

     - <script_name> .sh.eXXXX，它是作业的stderr
     - <script_name> .sh.oXXXX，它是作业的stdout

     这里的XXXX是作业ID，在每个qsub命令之后将其打印到屏幕上。

  4. 检查样本的输出

     ```shell
     cat run.sh.oXXXX
     ```

  5. 删除stdout和stderr文件并清理项目文件。

     ```shell
     rm build.sh.*; rm run.sh.*; make clean
     ```

#### 以交互模式构建和运行示例

一些警告：

+ 首先，对交互式会话的请求将被放入队列中。将尽快分配计算节点，并且无法保证等待时间。
+ 其次，当连接中断时，以交互方式在计算节点上启动的所有任务都将终止。

建议谨慎使用交互模式，并且仅用于在批处理模式下不可行的方案，例如在调试应用程序时单步执行代码。

1. 使用qsub在DevCloud GPU节点上请求交互式会话

   ```shell
   qsub -I -l nodes=1:gpu:ppn=2 -d .
   ```

2. 在GPU节点上的交互式会话中，构建并运行示例。

   ```shell
   make all && make run
   ```

3. 清理输出

   ```shell
   make clean
   ```

4. 终止交互式会话

   ```
   exit
   ```

