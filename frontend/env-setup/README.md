# 开发环境安装与比较

## Node.js

### 使用系统包管理器（不推荐）

许多 Linux 发行版的软件源中自带了 Node.js 和 npm 等软件。一些发行版（如 NixOS ）支持安装多个版本，而大部分发行版（如 Ubuntu 、 ArchLinux ）的源中只有一个版本。

* 优点
  - 安装方便
* 缺点
  - 依赖源中的版本

### 使用 NodeSource 软件源（不推荐）

NodeSource软件源的[文档](https://github.com/nodesource/distributions)

为 Debian/Ubuntu 和红帽系的发行版提供各种版本的 Node.js 安装。

* 优点
  - 安装方便
* 缺点
  - 只能安装一个版本

### 下载压缩包，解压安装（不推荐）

* 优点
  - 可以下载安装自己想要的版本
* 缺点
  - 操作繁琐，需要自己设置环境变量
  - 不便更新
  - 不便管理多个版本

### 使用 Node 版本管理器（推荐）

有诸如 [n](https://github.com/tj/n) 、 [nvm](https://github.com/nvm-sh/nvm) 等 Node 版本管理器。可以下载任意指定版本的 Node.js 以及与之配套的 npm ，并且可以快速切换 Node 版本。

## npm 与 yarn

我不知道，我只用过 npm

## 编辑器与 IDE

IDE 最好用的就是 JetBrains 的 WebStorm 。没有社区版，可以通过学校邮箱注册账号，免费使用。

Visual Studio Code 也是不错的。

Vim ：正在研究

