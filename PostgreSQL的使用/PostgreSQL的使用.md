# PostgreSQL的使用
## postgreSQL的架构基础
### postgreSQL使用C/S模型，一次POstgreSQL会话由一个服务器进程和需要执行数据库操作的用户的客户端应用组成。 客户端和服务器可以是在不同主机上，利用TCO/IP网络连接通讯。
## postgreSQL的使用手册
### http://www.runoob.com/manual/PostgreSQL/
### 该手册涵盖了整个postgre的教程以及一些安装运行中的可能错误
## PostgreSQL的环境配置
### 安装依赖
#### 在Ubuntu中执行poetry install
### 激活虚拟环境
#### 在ubuntu中执行poetry shell
### 下载连接数据库的软件DBeaver
#### 安装并运行docker desktop
#### DBeaver需要在docker desktop正常运行的条件下运行
#### 下载路径
#### https://dbeaver.io/ 默认下载即可
## 用docker运行PostgreSQL数据库
#### docker run --restart=always -p 5432:5432 --name PostgreSQL -e POSTGRES_PASSWORD=postgres -d postgres:14.2-alpine3.15 （在运行前需要把docker换源）
## 在数据库中建表
#### SQL_HOST=localhost SQL_PORT=5432 ./manage.py migrate 打开即可看到E-R图

