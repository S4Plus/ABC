# DBeaver教程
## DBeaver的安装
### 安装准备
#### 确保下载了docker desktop并正确运行 DBeaver的运行需要在docker desktop运行的条件之下
### 安装路径
####  https://dbeaver.io/ 默认下载即可 不要随便更改路径
## DBeaver连接数据库
### 在计算机上安装 DBeaver 后，打开软件，点击 左上角「连接」图标，选择需要连接的数据库，创建一个新连接。DBeaver 支持的数据库很多。
### 使用DBeaver直接连接MySQL
#### 如果该数据库对外开放允许远程访问的话，可以选择直接连接。
### 使用 DBeaver 通过 SSH 远程登录后再本地方式连接 MySQL
#### 如果数据库为了安全考虑没有打开数据库远程连接，而是通过 SSH 先登录服务器，然后再以本地登录的方式登录数据库的话。
#### 第一步：在「常规」里设置本地登录 MySQL 数据库的信息，注意主机地址要写「localhost」
#### 第二步：选择「SSH」的选项卡配置远程服务器登录信息。
#### DBeaver 会先使用 SSH 远程登录到服务器后，再以本地登录（localhost）的方式登录 MySQL 服务器，对安全更有保证
## DBeaver管理数据库
### 连接数据库后，可以查看 MySQL 实例信息，数据库状态、当前连接数、数据库配置以及用户和权限。DBeaver 还支持导入和导出 SQL 文件，可以使用 DBeaver 对数据库进行备份和恢复。
## DBeaver的写查询
### DBeaver 可以点击菜单栏中的「SQL」按钮，进入 SQL 编辑框，在这里写复杂的 SQL 命令，点击「执行」（黄色三角 icon）执行 SQL 命令。执行结果先是在软件下方。
## 备注
### 更加详细的教程可以参见
### https://kalacloud.com/blog/dbeaver-connect-to-mysql/

