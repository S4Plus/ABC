# docker的环境配置
## 配置好wsl
### 具体操作可见wsl配置部分，记得升级成wsl2
## 安装ubuntu
### https://cn.ubuntu.com/download 选择Windows版本下载 默认安装
## 更新apt并换源
### 下载更新apt
#### 在Ubuntu中输入 sudo apt update
### 换源
#### sudo sed -i 's/archive.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
#### 可参考教程http://mirrors.ustc.edu.cn/help/ubuntu.html
## 安装pip并换源 
### 安装pip
#### 在Ubuntu中输入sudo apt install python3-pip 利用pip -V可以检查是否安装成功
### 换源
#### 在Ubuntu中输入pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
## docker换源
### https://lug.ustc.edu.cn/wiki/mirrors/help/docker/
### 具体操作是在docker的配置文件中将daemon的JSON文件中加入一行
### “registry-mirrors":["https://docker.mirrors.ustc.edu.cn"]
}
## 一些备注
### 若docker出现docker desktop stopping或者docker desktop shutting down可以选择老一点的版本
### docker在使用过程中可能会自己关闭或崩溃，备选方案有：
#### 在Linux虚拟机里面安装Docker
#### 在Linux虚拟机里面安装Docker
#### 在Windows上安装Podman
#### 使用Windows原生的PostgreSQL
#### 放弃PostgreSQL，换用SQLite3