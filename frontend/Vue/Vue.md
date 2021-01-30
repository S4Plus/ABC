# Vue

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [学习资源](#学习资源)
- [开发环境配置](#开发环境配置)
  - [Node.js安装](#nodejs安装)
    - [Windows](#windows)
    - [Linux](#linux)
  - [NPM换源](#npm换源)
  - [Visual Studio Code配置](#visual-studio-code配置)
    - [ESLint](#eslint)
    - [Vetur](#vetur)

<!-- /code_chunk_output -->

## 学习资源

* [中文教程](https://cn.vuejs.org/v2/guide/)
* [Bootstrap](https://getbootstrap.com/docs/4.5/getting-started/introduction/)
* [Bootstrap-Vue](https://bootstrap-vue.org/docs)

## 开发环境配置

### Node.js安装

#### Windows

去[官网](https://nodejs.org/)或者[镜像站](http://mirrors.ustc.edu.cn/node/)下载。

#### Linux

使用[NodeSource](https://github.com/nodesource/distributions)。

可以换源：

```
http://mirrors.ustc.edu.cn/nodesource/
https://mirrors.tuna.tsinghua.edu.cn/nodesource/
```

### NPM换源

```bash
npm config set registry https://registry.npm.taobao.org
```

### Visual Studio Code配置

#### ESLint

此插件可以检查和自动缩进代码。

需要在安装后进行设置：ESLint > Format: Enable，将ESLint作为formatter使用。

打开需要排版的文件，在菜单中选择`View`、`Command Palette...`，输入`format`搜索，选择`Format Document With...`，之后选择`Select Default Formatter`，再选择ESLint。

也可在设置中选择Format On Save，每次保存时自动排版文件。

#### Vetur

此插件辅助Vue开发。安装即可。