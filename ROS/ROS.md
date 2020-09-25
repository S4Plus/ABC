# ROS Kinetic on Ubuntu 16.04

## 安装

### 软件源

完整列表：见[http://wiki.ros.org/ROS/Installation/UbuntuMirrors](http://wiki.ros.org/ROS/Installation/UbuntuMirrors)

```bash
sudo sh -c '. /etc/lsb-release && echo "deb http://mirrors.tuna.tsinghua.edu.cn/ros/ubuntu/ `lsb_release -cs` main" > /etc/apt/sources.list.d/ros-latest.list'
```

### 软件源的密钥

```bash
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
```

### 安装ROS

```bash
sudo apt-get update
sudo apt-get install ros-kinetic-desktop-full
```

### 环境设置

```bash
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

### 依赖安装

```bash
sudo apt install python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential
```

### 初始化`rosdep`

```bash
sudo rosdep init
rosdep update
```

**注意**：这一步会从GitHub下载文件，可能需要翻墙，设置代理方法：
1. 设置`http_proxy`和`https_proxy`环境变量，例如：`export http_proxy=127.0.0.1:1080;export https_proxy=$http_proxy`；
2. 也可以用`proxychains`。设置好`/etc/proxychains.conf`。

## 添加Python3支持

### 安装Python3.8

Ubuntu 16.04 LTS带的Python版本是上古的Python 3.5，我们安装新的Python 3.8。

```bash
sudo add-apt-repository ppa:deadsnakes/ppa
sudo apt update
sudo apt -y install python3.8 python3.8-venv python3.8-dev
```

### 安装`pip`

```bash
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
python3.8 get-pip.py
```

### 使用`venv`（可选）

```bash
python3.8 -m venv venv
source venv/bin/activate
```

### 换源

```bash
python3.8 -m pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

### 安装ROS的Python3支持

```bash
python3.8 -m pip install rospkg netifaces defusedxml
```

## Graphviz相关

```bash
sudo apt -y install graphviz graphviz-dev
python3.8 -m pip install graphviz
```

## 编程环境配置

### PyCharm

#### 版本

PyCharm有专业版和社区版，社区版免费，但是专业版可以通过学校的教育邮箱白嫖，有远程开发、支持常用框架等等优点。

#### 安装

##### 通过snap安装

社区版：

```bash
sudo snap install pycharm-community --classic
```

专业版：

```bash
sudo snap install pycharm-professional --classic
```

##### 手动安装

从[https://www.jetbrains.com/pycharm/download/#section=linux](https://www.jetbrains.com/pycharm/download/#section=linux)下载并解压，放到一个合适的地方（比如`/opt/`下）。

之后创建并编辑`~/.local/share/applications/pycharm.desktop`，填写如下内容：

```bash
[Desktop Entry]
Version=1.0
Type=Application
Name=PyCharm
Comment=PyCharm
Icon=/opt/pycharm-community-2020.2.2/bin/pycharm.png
Exec=bash -i -c "/opt/pycharm-community-2020.2.2/bin/pycharm.sh" %f
Path=/opt/pycharm-community-2020.2.2/bin
NoDisplay=false
Categories=Development;X-XFCE;X-Xfce-Toplevel;
StartupNotify=false
Terminal=false
```

版本和目录根据下载到的进行相应的变动。

重点是`Exec`，里面一定要用`bash -i -c "pycharm.sh"`，不然没法加载`~/.bashrc`中ROS相关的环境。