# lab5 spoc 思考题

- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。


## 个人思考题

### 总体介绍

 - 第一个用户进程创建有什么特殊的？
 - 系统调用的参数传递过程？
 - getpid的返回值放在什么地方了？

### 进程的内存布局

 - 尝试在进程运行过程中获取内核堆栈和用户堆栈的调用栈？
 - 尝试在进程运行过程中获取内核空间中各进程相同的页表项（代码段）和不同的页表项（内核堆栈）？

### 执行ELF格式的二进制代码-do_execve的实现

 - 在do_execve中进程清空父进程时，当前进程是哪一个？在什么时候开始使用新加载进程的地址空间？
 - 新加载进程的第一级页表的建立代码在哪？

### 执行ELF格式的二进制代码-load_icode的实现

 - 第一个内核线程和第一个用户进程的创建有什么不同？
 - 尝试跟踪分析新创建的用户进程的开始执行过程？

### 进程复制

 - 为什么新进程的内核堆栈可以先于进程地址空间复制进行创建？
 - 进程复制的代码在哪？复制了哪些内容？
 - 进程复制过程中有哪些修改？为什么要修改？

### 内存管理的copy-on-write机制
 - 什么是写时复制？
 - 写时复制的页表在什么时候进行复制？共享地址空间和写时复制有什么不同？

## 小组练习与思考题

### (1)(spoc) 在真实机器的u盘上启动并运行ucore lab,

请准备一个空闲u盘，然后请参考如下网址完成练习

https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-boot-with-grub2-in-udisk.md

> 按照网址说明的步骤，最先做的是一些准备工作，这些工作包括u盘的格式化并且能够建立一个分区，这些在windows下面需要专业的工具，但是在linux下只需要一些命令即可。接下来需要在U盘上安装grub2,作为引导程序，这时候在U盘根目录出现/boot/grub/目录，在此目录下创建配置文件grub.cfg。接下来，编译ucore，利用make mboot，产生了bin/grub_kernel文件,将此执行文件放到/boot目录下。重启机器，选择U盘启动，即可用u盘启动。

(报告可课后完成)请理解grub multiboot spec的含义，并分析ucore_lab是如何实现符合grub multiboot spec的，并形成spoc练习报告。

### (2)(spoc) 理解用户进程的生命周期。

> 需写练习报告和简单编码，完成后放到git server 对应的git repo中

### 练习用的[lab5 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab5/lab5-spoc-discuss)


#### 掌握知识点
1. 用户进程的启动、运行、就绪、等待、退出
2. 用户进程的管理与简单调度
3. 用户进程的上下文切换过程
4. 用户进程的特权级切换过程
5. 用户进程的创建过程并完成资源占用
6. 用户进程的退出过程并完成资源回收

> 注意，请关注：内核如何创建用户进程的？用户进程是如何在用户态开始执行的？用户态的堆栈是保存在哪里的？

阅读代码，在现有基础上再增加一个用户进程A，并通过增加cprintf函数到ucore代码中，
能够把个人思考题和上述知识点中的内容展示出来：即在ucore运行过程中通过`cprintf`函数来完整地展现出来进程A相关的动态执行和内部数据/状态变化的细节。(约全面细致约好)
```
                      exchange  stack(esp)
                       exchange page
                         alloce  init all fields of proc_struct  !
   in user_main
the process 0 is now waiting
                       exchange  stack(esp)
                       exchange page
kernel_execve: pid = 2, name = "spin".
run a process!
has changed to user mode!
I am the parent. Forking the child...
                         alloce  init all fields of proc_struct  !
I am the parent. Running the child...
                       exchange  stack(esp)
                       exchange page
I am the child. spinning ...
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
I am the parent.  Killing the child...
kill the process 3
kill returns 0
the process 3 is now waiting
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
wait returns 0
spin may pass.
                       exchange  stack(esp)
                       exchange page
                       exchange context
proc_exchange!
the process 0 is now waiting
```
> 关于如何打印从用户态到内核态，以及内核态到用户态的转换的临界状态，我需要再研究一下。课堂上面要弄懂太难，容我下来再研究一下。

请完成如下练习，完成代码填写，并形成spoc练习报告
