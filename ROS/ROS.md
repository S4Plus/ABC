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

### 安装PIP3

```bash
sudo apt -y install python3-pip
```

### 更新PIP3

```bash
pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple --upgrade pip
```

### 换源

```bash
python3 -m pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

### 安装ROS的Python3支持

```bash
python3 -m pip install rospkg netifaces defusedxml
```

## Graphviz相关

```bash
sudo apt -y install graphviz graphviz-dev
python3 -m pip install graphviz
```