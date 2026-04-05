# MIT 6.S081 Obsidian Wiki 设计

## 背景

目标是将 `https://mit-public-courses-cn-translatio.gitbook.io/mit6-s081/` 的内容整理进 Obsidian vault `/Users/qingcongyu/Documents/mit6.s081`，并严格按 `obsidian-wiki` 风格构建为可持续维护的 wiki，而不是一次性导出的网页镜像。

该 wiki 需要同时服务两类场景：

- 用户按课程、实验、概念进行学习和复习
- AI 在 vault 中基于结构化笔记进行检索、关联和回答

## 目标

- 建立符合 `obsidian-wiki` 的 vault 入口、领域目录、MOC 页面与变更日志
- 将 GitBook 页面以“原始材料 + wiki 页面”的双层结构导入
- 让课程内容可按 Lecture、Lab、概念和 xv6 代码映射进行导航
- 让后续增量导入、问答沉淀和健康检查有稳定落点

## 非目标

- 不修改 GitBook 原站内容
- 不把 xv6 源码整体复制进 vault
- 不在首次整理阶段追求所有概念页的深度完善
- 不依赖运行中的 Obsidian CLI 作为前置条件

## 约束

- 必须遵循 `obsidian-wiki` 的核心结构：`Home.md`、`log.md`、领域目录、`material/`、`templates/`
- 必须遵循 `obsidian-markdown` 的 frontmatter、wikilink、callout 和交叉导航规范
- `material/` 中的原始材料视为不可变来源，后续只读不改
- 新页面必须具备入链和出链，避免孤儿页
- 仓库当前存在用户未提交改动，本设计阶段只新增文档，不碰现有变更

## Vault 目标结构

```text
/Users/qingcongyu/Documents/mit6.s081/
├── Home.md
├── log.md
├── material/
│   ├── _README.md
│   └── mit6-s081/
├── templates/
│   └── note-template.md
├── 课程总览/
│   └── _MOC_课程总览.md
├── Lecture/
│   ├── _MOC_Lecture.md
│   └── Lec01 引言与示例.md
├── Lab/
│   ├── _MOC_Lab.md
│   └── Lab pgtbl.md
├── 概念/
│   ├── _MOC_概念.md
│   └── 系统调用.md
└── xv6源码映射/
    ├── _MOC_xv6源码映射.md
    └── trap 相关文件.md
```

## 知识领域设计

### 课程总览

承载课程说明、学习路线、Lecture 与 Lab 的总览关系，以及外部资源入口。

### Lecture

每个 GitBook lecture 页面对应一篇 `source` 类型 wiki 页面。文件名保留课程顺序号，便于线性阅读和排序。

### Lab

每个 lab 页面或实验相关页面对应一篇 `source` 类型 wiki 页面。强调实验目标、入口代码、关键系统调用、常见错误和调试线索。

### 概念

承载跨多篇页面反复出现的核心主题，例如 `系统调用`、`trap`、`页表`、`调度器`、`文件系统`。这些页面是 wiki 长期演化的主干。

### xv6源码映射

承载课程知识与本地 xv6 仓库的对应关系，例如相关目录、关键文件、函数入口、阅读顺序。这里不复制源码正文，只记录定位和解释。

## 页面类型设计

### 1. 原始材料页

位置：`material/mit6-s081/`

用途：

- 保存从 GitBook 抓取的原始 Markdown
- 记录原页面标题、URL、抓取时间
- 作为后续 wiki 页面可追溯的数据来源

规则：

- 原始材料页创建后不再进行理解性修改
- 若后续重新抓取，保留来源一致性，必要时新建版本或追加元数据说明

### 2. source 页

位置：对应领域目录，如 `Lecture/`、`Lab/`

用途：

- 表达某一来源页面“讲了什么”
- 作为用户阅读的主要入口
- 为概念页和映射页提供链接出发点

统一 frontmatter：

```yaml
---
title: 标题
aliases: []
category: Lecture
tags: [source]
---
```

最小内容结构：

- `# 标题`
- `[!abstract] 概述`
- `## 要点`
- `## 关键术语`
- `## 与 xv6 的关系`
- `## 相关笔记`
- `## 参考资料`

### 3. concept 页

位置：`概念/`

用途：

- 聚合同一知识点在多个 lecture/lab 中的出现方式
- 形成持续演进的解释，而不是重复抄写单页内容

统一 frontmatter：

```yaml
---
title: 系统调用
aliases: []
category: 概念
tags: [concept]
---
```

最小内容结构：

- 概念定义
- 在课程中的位置
- 对应 lecture/lab 链接
- 对应 xv6 代码入口
- 常见误区或易混点

### 4. analysis 页

位置：通常在 `概念/`，必要时在其他领域目录

用途：

- 记录跨来源比较、总结和高价值问答沉淀
- 例如“中断、异常、系统调用的统一视角”

统一 frontmatter：

```yaml
---
title: 中断、异常与系统调用比较
aliases: []
category: 概念
tags: [analysis]
---
```

## 导航设计

### Home.md

`Home.md` 是整个 vault 的唯一入口，包含：

- 知识领域表：领域名、说明、对应 MOC
- 最近更新表：链接、日期、标签
- 目录结构代码块
- `[!note]` 或 `[!tip]` 维护说明，解释 ingest、query、lint 的工作流

### MOC 页面

每个领域目录创建一个固定命名的 MOC 页面：

- `课程总览/_MOC_课程总览.md`
- `Lecture/_MOC_Lecture.md`
- `Lab/_MOC_Lab.md`
- `概念/_MOC_概念.md`
- `xv6源码映射/_MOC_xv6源码映射.md`

每个 MOC 页面包含：

- 领域概述 callout
- Mermaid 关系图
- 索引表：笔记、摘要、关键词
- 推荐阅读顺序 callout

### 交叉导航 Footer

每一篇正式 wiki 页面末尾追加：

```markdown
## 相关笔记

- 返回：[[_MOC_{domain}|{domain}导航]]
- 首页：[[Home|mit6.s081 Wiki]]
```

## 命名规范

- Lecture 页面使用 `LecXX 中文标题`
- Lab 页面使用 `Lab 原实验名`
- 概念页面使用简洁中文知识名
- xv6 映射页面使用“主题 + 映射对象”命名，避免直接复用源码文件名作为唯一标题
- `material/` 内部文件名可以保留 slug 或抓取特征，但正式 wiki 页面不直接使用网页 slug 作为最终文件名

## 导入流程

一次完整导入一个 GitBook 页面时，流程如下：

1. 抓取并保存原始材料到 `material/mit6-s081/`
2. 根据页面类型创建或更新对应 `source` 页
3. 检查高频术语，必要时创建或更新 `concept` 页
4. 若形成高价值综合结论，创建 `analysis` 页
5. 更新相关领域 MOC
6. 更新 `Home.md` 最近更新区域
7. 向 `log.md` 追加本次变更记录

## 数据流

```text
GitBook 页面
  -> material 原始材料
  -> source 页面
     -> concept 页面
     -> analysis 页面
     -> xv6源码映射 页面
  -> MOC
  -> Home.md
  -> log.md
```

## 与 xv6 仓库的关系

vault 与仓库分离：

- vault 负责知识组织和长期积累
- 当前仓库 `/Users/qingcongyu/cscourses/mit6.s081` 仍是代码和实验环境

wiki 页面可引用以下信息：

- 本地代码路径
- 关键文件名
- 关键函数名
- 对应 lab 或 lecture

但不在 vault 中镜像整个源码树。

## 冲突与错误处理

### 内容矛盾

如果不同来源或不同整理阶段出现矛盾，在相关页面同时加入：

```markdown
> [!warning] 矛盾
> 本文声称 X，但 [[另一篇笔记]] 认为 Y。
> 待确认：需要更多资料验证。
```

### 链接完整性

- 新建页面必须至少被一个 MOC 链接
- 新建页面必须至少链接到一个相关页面或 Home/MOC
- 重命名页面时必须批量更新 wikilink

### 抓取异常

- 抓取失败的页面记录到 `log.md`
- 页面结构异常时先保留原始材料，再人工整理为 wiki 页面
- 页面缺失时在 MOC 中标记待补

## 测试与验证

本阶段只定义设计，后续实现时至少验证：

- 目录结构是否符合 `obsidian-wiki`
- `Home.md`、各 MOC、`log.md` 是否创建正确
- source 页是否包含统一 frontmatter 和 footer
- 概念页是否有有效的双向导航
- 导入后的 wikilink 在 Obsidian 中可解析
- 随机抽样页面是否能追溯回 `material/` 原始来源

## 实施边界

首轮实施优先完成：

- vault 基础骨架
- 首页与各领域 MOC
- 从 GitBook 导入一批 representative 页面
- 建立第一批核心概念页
- 建立 log 记录机制

后续再扩大到整站批量导入、补全概念页和 xv6 映射页。

## 开放问题

- GitBook 全站的页面清单需要在实现阶段进一步枚举和去重
- 某些页面标题可能需要人工润色为更适合中文 wiki 的文件名
- 是否将课程视频、PDF、实验说明一并纳入 `material/`，可在实施计划阶段决定
