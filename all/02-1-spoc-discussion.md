#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.1 BIOS
 1. 比较UEFI和BIOS的区别。<br />

> (1)UEFI已具备文件系统的支持，而BIOS不具备文件系统支持 <br />
  (2)BIOS下启动操作系统之前，必须从硬盘上指定扇区读取系统启动代码（包含在主引导记录中），然后从活动分区中引导启动操作系统。而UEFI则不需要。<br />
  (3)BIOS的空间比较有限，很难有新的提升，并且对新硬件支持不足，相反，UEFI能够很好地支持。<br />
  
 1. 描述PXE的大致启动流程。

> 
* 客户端个人电脑开机后， 在 TCP/IP Bootrom 获得控制权之前先做自我测试。
* Bootprom 送出 BOOTP/DHCP 要求以取得IP。
* 如果服务器收到个人电脑所送出的要求， 就会送回 BOOTP/DHCP 回应，内容包括客户端的 IP 地址，预设网关，及开机映像文件。否则，服务器会忽略这个要求。
* Bootprom 由 TFTP 通讯协议从服务器下载开机映像文件。
* 个人电脑通过这个开机映像文件开机， 这个开机文件可以只是单纯的开机程式也可以是操作系统。
* 开机映像文件将包含 kernel loader 及压缩过的 kernel，此 kernel 将支持NTFS root系统。
* 远程客户端根据下载的文件启动机器。
 

## 3.2 系统启动流程
 1. 了解NTLDR的启动流程。<br />

> 
1、电源自检程序开始运行<br />
2、主引导记录被装入内存，并且程序开始执行<br />
3、活动分区的引导扇区被装入内存<br />
4、NTLDR从引导扇区被装入并初始化<br />
5、将处理器的实模式改为32位平滑内存模式<br />
6、NTLDR开始运行适当的小文件系统驱动程序，小文件系统驱动程序是建立在NTLDR内部的，它能读FAT或NTFS<br />
7、NTLDR读boot.ini文件<br />
8、NTLDR装载所选操作系统<br />
9.Ntdetect搜索计算机硬件并将列表传送给NTLDR，以便将这些信息写进\\HKE Y_LOCAL_MACHINE\HARDWARE中。<br />
10.然后NTLDR装载Ntoskrnl.exe，Hal.dll和系统信息集合。<br />
11.Ntldr搜索系统信息集合，并装载设备驱动配置以便设备在启动时开始工作<br />
12.Ntldr把控制权交给Ntoskrnl.exe，这时,启动程序结束,装载阶段开始<br />

 1. 了解GRUB的启动流程。<br />

> 
1、系统加电后，BIOS进行检测系统参数<br />
2、装载基本的引导装载程式stage1，用来装载第二引导程式<br />
3、装载第二引导程式，以便用户选择允许载入一个特定的操作系统，这步通常是用户显示一个菜单或者是输入命令等等
（stage2因为比较大，所以处于文件系统中）<br />
4、装载在一个特定分区上的操作系统<br />

 1. 比较NTLDR和GRUB的功能有差异。<br />

> ntldr功能很少，只能引导win,只能装在硬盘。<br />
  grub是第三方操作系统引导器，可以引导多种介质和操作系统。<br />

 1. 了解u-boot的功能。

> U-Boot可支持的主要功能列表：<br />
* 系统引导支持NFS挂载、RAMDISK(压缩或非压缩)形式的根文件系统；支持NFS挂载、从FLASH中引导压缩或非压缩系统内核；
* 基本辅助功能强大的操作系统接口功能；可灵活设置、传递多个关键参数给操作系统，适合系统在不同开发阶段的调试要求与产品发布，尤以Linux支持最为强劲；支持目标板环境参数多种存储方式，如FLASH、NVRAM、EEPROM；
* CRC32校验可校验FLASH中内核、RAMDISK镜像文件是否完好；
* 设备驱动串口、SDRAM、FLASH、以太网、LCD、NVRAM、EEPROM、键盘、USB、PCMCIA、PCI、RTC等驱动支持；
* 上电自检功能SDRAM、FLASH大小自动检测；SDRAM故障检测；CPU型号；
* 特殊功能XIP内核引导；

## 3.3 中断、异常和系统调用比较
 1. 举例说明Linux中有哪些中断，哪些异常？

> (1)计算机对硬件的设备进行管理主要有中断和轮寻两种，中断是指硬件在需要的时候向内核发出信号占用资源，结束后让出资源。<br />
例如：<br />
* 软件中断（softIRQ）是内核提供的一种延迟执行机制，它完全由软件触发，虽然说是延迟机制，实际上，在大多数情况下，它与普通进程相比，能得到更快的响应时间。软中断也是其他一些内核机制的基础，比如tasklet，高分辨率timer等。<br />
* 硬中断是由硬件产生的，比如，像磁盘，网卡，键盘，时钟等。每个设备或设备集都有它自己的IRQ（中断请求）。基于IRQ，CPU可以将相应的请求分发到对应的硬件驱动上<br />
  (2)异常，就是可以打断CPU正常运行流程的一些事情，如代码运行错误，CPU内部错误，内核缺页异常，试图修改只读的数据等等。<br />

 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)

> linux的系统调用有大概上百个，主要分类如下：<br />
一、进程管理：有四五十个，包括创建新进程、终止进程、创建子进程、设置或获取调度优先级等等；<br /> 二、文件操作：四十个左右，包括文件读写操作，以及文件系统操作（创建目录、删除目录、改变当前目录、获取当前目录等等） <br />
三、系统控制：二十多个，可获取与设置系统资源上限、打开与关闭交换文件和设备等 <br /> 四、内存管理：十几个，如内存页面加锁与解锁、改编数据段空间分配等 <br /> 五、网络管理：六七个，如取域名、设置域名、以及获取与设置主机标识号等 <br /> 六、socket控制：二十个左右，如建立socket、绑定socket、监听socket、响应socket等 <br /> 七、用户管理：二十个左右，获取、设置用户、组标识号等等 <br /> 通过上面可以看到，Linux的系统调用数量有一百多个，主要有有进程管理、文件操作、内存管理、系统控制、网络管理、socket控制、用户管理等。

```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
 
> 系统的调用有22个,主要的系统分类为文件操作、进程管理、内存管理等，因为网络方面没有做基本要求，所以没有，这些方面函数的归类如下所示:<br />
* 进程管理: sys_exit sys_fork sys_wait sys_exec sys_yield sys_kill sys_getpid sys_putc sys_pgdir sys_gettime sys_lab6_set_priority sys_sleep 
* 文件操作: sys_open sys_close sys_read sys_write sys_seek sys_fstat sys_fsync sys_getcwd sys_getcwd sys_getdirentry sys_dup 

 ```
  + 采分点：说明了ucore的大致数量（二十几个），说明了ucore系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)

> 
(1) objdump命令是Linux下的反汇编命令工具。常用命令如下：
* objdump -f lab1-ex0.exe:显示lab1-ex0.exe的文件头信息
* objdump -d lab1-ex0.exe:显示lab1-ex0.exe的需要执行指令的那些section
* objdump -D lab1-ex0.exe:显示lab1-ex0.exe的所有section，汇编代码很清楚，这个指令应该是最常用的
* objdump -h lab1-ex0.exe:显示lab1-ex0.exe的SectionHeader信息
* objdump -x lab1-ex0.exe:显示lab1-ex0.exe的全部Header信息
* objdump -s lab1-ex0.exe:除了显示lab1-ex0.exe的全部Header信息以及对应十六进制文件代码的对照。
objdump的主要功能就是反汇编进行调试。
(2) nm命令显示关于指定 File 中符号的信息，文件可以是对象文件、可执行文件或对象文件库。如果文件没有包含符号信息，nm 命令报告该情况，但不把它解释为出错条件。 nm 命令缺省情况下报告十进制符号表示法下的数字值。<br />
命令格式如右所示：nm -A a.exe。而中间的符号不同的话，表示的含义也不同：<br />
* A    Global absolute 符号。
* a    Local absolute 符号。
* B    Global bss 符号。
* b    Local bss 符号。
* D    Global data 符号。
* d    Local data 符号。
* f    源文件名称符号。
* T    Global text 符号。
* t    Local text 符号。
* U    未定义符号。<br />
(3) 查看文件类型信息
格式：
file[OPTIONS...] [FILE...]
主要参数：
* --help       显示帮助信息
* -v,--version 输出版本信息并退出
* -b,--brief   不显示文件名字
* -f,--files-fromFILE  读取待测试的名称文件
* -F,--seperatorSTRING 使用字符串作为分隔符，不再使用“：”
* -i,--mime 显示文件的mime类型
* -L,--dereference 显示符号链接所指向文件信息
* -d,--debug 输出调试信息<br >
(4)系统调用指运行在用户态的程序向操作系统内核请求需要更高权限运行的服务。<br />
它是用户态进入内核态的入口，在一些有关计算机安全的如内存分配管理、线程维护等等的调用中尤为重要，这样设计提高了系统的安全性，并且使得编程人员对底层透明，不用关心底层的实现。Linux中实现系统调用用了I386体系结构中的软件中断，由内核函数实现。

 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 
 ```
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)

> strace常用来跟踪进程执行时的系统调用和所接收的信号,strace可以跟踪到一个进程产生的系统调用,包括参数，返回值，执行消耗的时间。<br />
gcc -o lab1-ex1.exe lab1-ex1.c返回的结果是基于时间排序的，不符合要求。<br />
本次调用的命令为: gcc -tt lab1-ex1.exe lab1-ex1.c，返回的结果时基于时间的：<br />
* 21:54:15.477561 execve("./lab1-ex1.exe", ["./lab1-ex1.exe"], [/* 63 vars */]) = 0
* 21:54:15.478729 brk(0)                  = 0x92b000
21:54:15.479053 access("/etc/ld.so.nohwcap", F_OK) = -1 ENOENT (No such file or directory)
* 21:54:15.490855 mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f1d3daa0000
* 21:54:15.490937 access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file or directory)
* 21:54:15.490994 open("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14/tls/x86_64/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
* 21:54:15.491044 stat("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14/tls/x86_64", 0x7ffff184a1d0) = -1 ENOENT (No such * file or directory)
* 21:54:15.491082 open("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14/tls/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT * (No such file or directory)
* 21:54:15.491116 stat("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14/tls", 0x7ffff184a1d0) = -1 ENOENT (No such file or directory)
* 21:54:15.491148 open("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14/x86_64/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
* 21:54:15.491182 stat("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14/x86_64", 0x7ffff184a1d0) = -1 ENOENT (No such file or directory)
* 21:54:15.491214 open("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
* 21:54:15.491248 stat("/home/yangjun/ns2/ns-allinone-2.35/otcl-1.14", {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
* 21:54:15.491287 open("/home/yangjun/ns2/ns-allinone-2.35/lib/tls/x86_64/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
* 21:54:15.491321 stat("/home/yangjun/ns2/ns-allinone-2.35/lib/tls/x86_64", 0x7ffff184a1d0) = -1 ENOENT (No such file or directory)
* 21:54:15.491354 open("/home/yangjun/ns2/ns-allinone-2.35/lib/tls/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
* 21:54:15.491387 stat("/home/yangjun/ns2/ns-allinone-2.35/lib/tls", 0x7ffff184a1d0) = -1 ENOENT (No such file or directory)
* 21:54:15.491419 open("/home/yangjun/ns2/ns-allinone-2.35/lib/x86_64/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
* 21:54:15.491452 stat("/home/yangjun/ns2/ns-allinone-2.35/lib/x86_64", 0x7ffff184a1d0) = -1 ENOENT (No such file or directory)
* 21:54:15.491484 open("/home/yangjun/ns2/ns-allinone-2.35/lib/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
* 21:54:15.491517 stat("/home/yangjun/ns2/ns-allinone-2.35/lib", {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
* 21:54:15.491554 open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
* 21:54:15.491590 fstat(3, {st_mode=S_IFREG|0644, st_size=93203, ...}) = 0
* 21:54:15.491622 mmap(NULL, 93203, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f1d3da89000
* 21:54:15.491655 close(3)                = 0
* 21:54:15.491688 access("/etc/ld.so.nohwcap", F_OK) = -1 ENOENT (No such file or directory)
* 21:54:15.491730 open("/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
* 21:54:15.491767 read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\320\37\2\0\0\0\0\0"..., 832) = 832
* 21:54:15.491805 fstat(3, {st_mode=S_IFREG|0755, st_size=1845024, ...}) = 0
* 21:54:15.491844 mmap(NULL, 3953344, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f1d3d4ba000
* 21:54:15.491878 mprotect(0x7f1d3d675000, 2097152, PROT_NONE) = 0
* 21:54:15.491925 mmap(0x7f1d3d875000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bb000) = 0x7f1d3d875000
* 21:54:15.491981 mmap(0x7f1d3d87b000, 17088, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f1d3d87b000
* 21:54:15.492031 close(3)                = 0
* 21:54:15.492082 mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f1d3da88000
* 21:54:15.492129 mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f1d3da86000
* 21:54:15.492178 arch_prctl(ARCH_SET_FS, 0x7f1d3da86740) = 0
* 21:54:15.492391 mprotect(0x7f1d3d875000, 16384, PROT_READ) = 0
* 21:54:15.492522 mprotect(0x600000, 4096, PROT_READ) = 0
* 21:54:15.492578 mprotect(0x7f1d3daa2000, 4096, PROT_READ) = 0
* 21:54:15.492624 munmap(0x7f1d3da89000, 93203) = 0
* 21:54:15.492720 fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 10), ...}) = 0
* 21:54:15.492823 mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f1d3da9f000
* 21:54:15.492887 write(1, "hello world\n", 12hello world
) = 12
* 21:54:15.492993 exit_group(12)          = ?
* 21:54:15.493095 +++ exited with 12 +++
在本次系统调用中，分别调用了mmap,open,access,mprotect,munmap,brk,fstat,write,read,close,execve,arch_prctil这些过程。
* mmap将一个文件或者其它对象映射进内存
* open 函数用于打开和创建文件
* access判断是否具有存取文件的权限
* mprotect: 设置内存访问权限
* munmap删除特定地址区域的对象映射
* brk实现虚拟内存到内存的映射
* fstat由文件描述词取得文件状态
* write系统调用 <br />
Linux内核会先进行若干检查，接着将数据复制进缓冲区。稍后，内核会在后台收集所有“脏”(有数据写入)缓冲区(内容跟相应磁盘块不同的所有缓冲区)，将它们安排成最佳顺序，接着写进磁盘。read函数从打开的设备或文件中读取数据,close关闭文件,execve运行可执行文件,arch-prctil设置架构特定的线程状态,这样就完成了系统调用的过程。

 ```
  + 采分点：说明了strace的大致用途，说明了系统调用的具体执行过程（包括应用，CPU硬件，操作系统的执行过程）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.5 ucore系统调用分析
 1. ucore的系统调用中参数传递代码分析。
 1. ucore的系统调用中返回结果的传递代码分析。
 1. 以ucore lab8的answer为例，分析ucore 应用的系统调用编写和含义。
 1. 以ucore lab8的answer为例，尝试修改并运行代码，分析ucore应用的系统调用执行过程。
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能
 
