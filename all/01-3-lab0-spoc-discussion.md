# lab0 SPOC思考题

## 个人思考题

---

能否读懂ucore中的AT&T格式的X86-32汇编语言？请列出你不理解的汇编语言。

>  在网站http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm可以找到一些示例
，除了一些符号并没有看到过，其表示还是比较清楚的。

虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？

>  虽然对内存的读写，串口等等这方面已经比较熟悉，但是对分时的实现以及内存管理还不够了解

哪些困难（请分优先级）会阻碍你自主完成lab实验？

>  1、对计算机的底层构造不是很清楚
   2、对git的使用不熟悉
   3、对部编程实现不是很清楚

如何把一个在gdb中或执行过程中出现的物理/线性地址与你写的代码源码位置对应起来？

>   gdb出现的地址是汇编代码的地址，我们需要知道汇编代码。所以如果时程序源码的话，需要生成汇编代码，而如果时二进制文件的话则需要反汇编。

了解函数调用栈对lab实验有何帮助？

>   函数调用栈能够使得系统的设计者可以更加清楚程序调用过程中内存的组织和存储方式，能够利用程序调用栈来加快速率。

你希望从lab中学到什么知识？

>   希望从中系统地了解操作系统的组成结构，熟悉底层编程
---

## 小组讨论题

---

搭建好实验环境，请描述碰到的困难和解决的过程。

> 虚拟机运行不了，将virtualbox换成VMware也不可以，最后选择了用linux系统来搭建

熟悉基本的git命令行操作命令，从github上
的 http://www.github.com/chyyuu/ucore_lab 下载
ucore lab实验

> 下载成功并可以运行

尝试用qemu+gdb（or ECLIPSE-CDT）调试lab1

> 已经配好eclipse-CDT环境，可以调试lab1

对于如下的代码段，请说明”：“后面的数字是什么含义
```
/* Gate descriptors for interrupts and traps */
struct gatedesc {
    unsigned gd_off_15_0 : 16;        // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;            // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;            // reserved(should be zero I guess)
    unsigned gd_type : 4;            // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;                // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;                // Present
    unsigned gd_off_31_16 : 16;        // high bits of offset in segment
};
```


> 冒号后面的数字指的是前面的变量的位域，也就是表示的数的比特量的长度。

对于如下的代码段，
```
#define SETGATE(gate, istrap, sel, off, dpl) {            \
    (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;        \
    (gate).gd_ss = (sel);                                \
    (gate).gd_args = 0;                                    \
    (gate).gd_rsv1 = 0;                                    \
    (gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;    \
    (gate).gd_s = 0;                                    \
    (gate).gd_dpl = (dpl);                                \
    (gate).gd_p = 1;                                    \
    (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
}
```

如果在其他代码段中有如下语句，
```
unsigned intr;
intr=8;
SETGATE(intr, 0,1,2,3);
```
请问执行上述指令后， intr的值是多少？

> 65538，因为按照规则算出来之后得到的值为0000 EE00 0001 0002，取低32位之后就是65538

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序

> 这个文件主要实现了一些链表操作的函数。比如增加以及删除，初始化，前一个元素或者后一个元素等等。<br />
  代码如下所示：<br />
  	list_entry_t demo1; <br />
	list_entry_t demo2; <br />
	list_entry_t*  myentry1 =&demo1; <br />
	list_entry_t*  myentry2 =&demo2; <br />
	list_init(myentry1); <br />
	list_add_before(myentry1,myentry2); <br />
	list_entry_t* myentry3 = list_next(myentry2); <br />
---

## 开放思考题

---

是否愿意挑战大实验（大实验内容来源于你的想法或老师列好的题目，需要与老师协商确定，需完成基本lab，但可不参加闭卷考试），如果有，可直接给老师email或课后面谈。

>  不准备挑战大实验

---
