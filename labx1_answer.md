##1.lab_x##
```c
    all user-mode processes have quit.
    kernel panic at kern/process/proc.c:837:
    assertion failed: nr_free_pages_store == nr_free_pages()
    Welcome to the kernel debug monitor!!
    Type 'help' for a list of commands.
```

这种情况出现的原因是nr_free_pages_store！=nr_free_pages()，通过函数调用分析，我们得到在proc_init>>init_main>>kernel_thread>>
do_fork>>setup_kstack函数内的fork_out分配两个页面
而结果也是正好差了两个页面，所以需要我们找到究竟是那一次内存分配没有释放，因此我打印出了函数调用过程的进程的切换情况和
内存分配情况。
在内存分配中，我们共用do_fork()产生了33个进程，其中一个是要返回作为kernel_thread的结果的，而另外的所有子进程都在运行完之后被do_wait回收。
另外，我们在do_exit()中看到:
```c
    if (current == idleproc) {
        panic("idleproc exit.\n");
    }
    if (current == initproc) {
        panic("initproc exit.\n");
    }
```
如果是init或者是idleproc的话，就不会执行。考虑到了这两个进程的特殊性，考虑可能是这两个进程的问题，通过打印调用过程，我们发现，进程的序号是这样的：<br />
idleproc：0 ，init_proc: 1, forktest: 2,其他的32个子进程，所以进程号一直排到了34.
那么，我们只考虑分配和回收的这两个点，分配的时候分别分配16+2个页面，而回收的时候do_exit会回收16个页面，而do_wait()会回收2个页面，从打印的数据来看，从2号进程到32号进程分配的都回收了，不得不怀疑是1号进程了。
1号进程（init_proc）是 
```c
   int pid = kernel_thread(init_main, NULL, 0);
   initproc = find_proc(pid);
```
这样产生的，那么在kernel_thread>>do_fork中返回的值便是这个pid，那么我们可以确定这个地方的setup_kstack(proc)，也就是建立用户内核堆栈所用的空间被分配了，并且计入到了分配的总空间里面，但是并没有在init_main这个函数中释放，同时我们确定如果是要像程序中所写的，那么应该是assert(nr_free_pages_store == nr_free_pages()+2);也就是说程序并没有错，但是在这里写这个声明是有问题的。

