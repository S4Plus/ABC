# tag 查询

`git tag` 输出所有 release 标签的时候是按字典序而非时序，就可能出现如下的排列，

`v1.1.0, v1.10.0, v1.11.0, …, v1.19.0, v1.2.0, …, v1.9.0`，

在找最新版本的时候很不方便，可能会 `checkout` 错误的版本。

为此这里介绍一些复杂一点的 git 命令。

查询最新版本：

```bash
git describe --tags $(git rev-list --tags --max-count=1)
```

查询当前版本：

```bash
git describe --tags
```

查询 tag 对应的 release time ([ISO 8601](https://en.wikipedia.org/wiki/ISO_8601))：

```bash
git log -1 --format=%aI --tags
```
