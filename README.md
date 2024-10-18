C语言练习题：实现一个基于链表的 YAMM 内存管理器

## 背景需求：
在操作系统中，内存管理是关键的一部分。为了高效地管理内存，很多系统采用链表的方式来跟踪和分配内存块。本次练习要求用 C 语言实现一个简单的内存管理器 yamm (Yet Another Memory Manager)。采用 Linux 内核的链表机制来管理内存块，并要求使用三个链表来跟踪内存的状态。

## 实现接口（API）

需要实现的API如下(yamm.h)
在yamm.c中实现

```C
/**
 * @brief 初始化 YAMM 内存管理器
 * 
 * 初始化内存管理器，并分配一个大小为 `size` 的内存池。该内存池将在管理器内部通过链表管理。
 * 
 * @param size 总共管理的内存大小（字节）
 * @return int 返回 0 表示初始化成功，返回 -1 表示失败（例如，内存分配失败）
 */
int yammInit(int size);

/**
 * @brief 分配内存
 * 
 * 从空闲链表中找到首个大小足够的内存块，并返回其地址。使用首次适应（First-Fit）算法进行分配。
 * 如果找到的内存块大于请求大小，则将其拆分，分配出适合的块。如果没有足够的内存，或内存块数量不足，则返回 NULL。
 * 
 * @param size 请求的内存大小（字节）
 * @return void* 返回分配的内存块的起始地址，如果分配失败则返回 NULL
 */
void* yammAlloc(int size);

/**
 * @brief 释放已分配的内存块
 * 
 * 释放指定地址处的内存块，将其返回到空闲链表中。如果可能，合并相邻的空闲块以减少内存碎片。
 * 
 * @param ptr 指向要释放的内存块的地址
 * @return int 返回 0 表示释放成功，返回 -1 表示释放失败（如该地址未分配或无效）
 */
int yammFree(void* ptr);

/**
 * @brief 销毁内存管理器并释放所有资源
 * 
 * 释放内存池并清空所有链表。销毁过程中会释放所有内存块及管理器的内存资源。
 * 
 * @return int 返回 0 表示成功销毁管理器，返回 -1 表示销毁失败
 */
int yammDestroy();

/**
 * @brief 输出内存管理器当前状态
 * 
 * 打印出当前内存管理器的状态，包括已分配内存块和空闲内存块的大小、地址，以及空闲结点数量等信息。用于调试。
 * 
 * @return int 返回 0 表示成功输出状态，返回 -1 表示失败
 */
int yammDump();

```

## 要求

- 使用 __首次适应（First Fit）算法__ 选取内存块进行分配。
- __只能在初始化时使用 malloc__ ：整个内存池(包括管理节点的内存分配)是在初始化时分配的，之后的内存管理必须通过内存块的分割和回收来完成。

- __只能在销毁时使用 free__ ：整个内存池的释放在管理器销毁时完成。

- 内存块管理：内存管理器通过 __链表__ 来组织 __最多 32 个__ 内存块，并采用 __Linux 内核链表__ 的实现方式进行管理。内存块空间的获取与释放，仍然只能在初始化/销毁时进行。

- 测试用例均为 __单线程__ 执行，暂不需要考虑多线程并发的情况。

- 注意处理各种 __非法参数__ 的情况（按要求返回-1或NULL）

## 链表

通过三个链表来管理内存块：

- 空闲链表（free_list）：存储当前未分配的内存块。

- 已分配链表（allocated_list）：存储已分配的内存块。

- 空闲结点链表（idle_list）：存储未使用的结点，用于动态创建新的内存块结构体。


链表结点定义如下
```C
struct list_head {
    struct list_head *next, *prev;
};

struct memory_block {
    struct list_head list;
    // TODO: some code goes here
    // 添加其他必要的属性
};
```

使用 __双向循环链表__ 实现，结点定义与平时写的链表不同，请参考`linux/list.h`。

通过`list_entry`来获得对应的`block`，不能修改`list_head`的定义

部分实现如下，请根据需要，补充添加API。参考`linux/list.h`的实现即可。

> *（选做） 尝试理解宏container_of，offsetof的机制

```C
#ifndef __LIST_H__
#define __LIST_H__
#include<stdio.h>

// 链表结点定义
struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    // TODO
}

// 使用:
// struct memory_block* block = (struct memory_block*)list_entry(pos, struct memory_block, list)
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

#define offsetof(type, member) ((size_t) &((type *)0)->member)

#define list_empty(head) (/* TODO */)

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    // TODO
}

// TODO
#define list_for_each(pos, head)

#endif
```


## 编译运行

编写了一些测试用例(`main.c`), 请运行`main.c`，要求通过所有测试用例

`yammDump`主要用于打印调试，暂时没有对应的测试用例，请自行测试。

`yammDump`打印结果应该类似以下（格式自定）

```
yammDump:
Allocated:
64 0x5573492cb6b0
64 0x5573492cb6f0
64 0x5573492cb730
64 0x5573492cb770
64 0x5573492cb7b0
64 0x5573492cb7f0
64 0x5573492cb830
64 0x5573492cb870
64 0x5573492cb8b0
64 0x5573492cb8f0
64 0x5573492cb930

Free:
320 0x5573492cb970

Idle blocks: 20
yammDump End
```

调试过程中，可以自行添加简单的用例测试。多使用`yammDump`, 打印链表信息，帮助调试

### Linux

推荐在$Linux$环境下完成该题目。可以配置虚拟机，连接`ssh`的方式。

已经提供了`Makefile`，使用`gcc`编译代码：

```bash
make clean
make
./main
```

可以使用`gdb`等工具进行调试

### windows
如使用$windows$系统，代码的编译运行环境请自行完成。
