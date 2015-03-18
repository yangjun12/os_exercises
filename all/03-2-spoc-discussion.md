# lec6 SPOC思考题


NOTICE
- 有"w3l2"标记的题是助教要提交到学堂在线上的。
- 有"w3l2"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。


## 个人思考题
---

（1） (w3l2) 请简要分析64bit CPU体系结构下的分页机制是如何实现的
```
  + 采分点：说明64bit CPU架构的分页机制的大致特点和页表执行过程
  - 答案没有涉及如下3点；（0分）
  - 正确描述了64bit CPU支持的物理内存大小限制（1分）
  - 正确描述了64bit CPU下的多级页表的级数和多级页表的结构或反置页表的结构（2分）
  - 除上述两点外，进一步描述了在多级页表或反置页表下的虚拟地址-->物理地址的映射过程（3分）
 ```

>  64bit的CPU的物理内存大小的限制是1.68*10^7T，所有CPU的多级页表的级数一般是自己规定的，要根据页表的大小以及页表项的大小进行判定，一般32bit的CPU是两级页表就可以满足要求，而64bit的多级页表就需要三级页表或者更多级，多级页表当中页面对应了页表项，页表当中存储页表项，每一个页表项存储着对应的帧号，存在位，修改位和引用位等。<br />
在多级页表下，逻辑地址分成若干个部分，通过每个部分的偏移量得到第一个页表项，这样可以减少叶表的大小。如此循环下去最终得到物理地址。

## 小组思考题
---

（1）(spoc) 某系统使用请求分页存储管理，若页在内存中，满足一个内存请求需要150ns。若缺页率是10%，为使有效访问时间达到0.5ms,求不在内存的页面的平均访问时间。请给出计算步骤。 

- [x]  

> 500=0.9*150+0.1*x <br />
0.1x=500-135 <br />
x=3650ns 

（2）(spoc) 有一台假想的计算机，页大小（page size）为32 Bytes，支持32KB的虚拟地址空间（virtual address space）,有4KB的物理内存空间（physical memory），采用二级页表，一个页目录项（page directory entry ，PDE）大小为1 Byte,一个页表项（page-table entries
PTEs）大小为1 Byte，1个页目录表大小为32 Bytes，1个页表大小为32 Bytes。页目录基址寄存器（page directory base register，PDBR）保存了页目录表的物理地址（按页对齐）。

PTE格式（8 bit） :
```
  VALID | PFN6 ... PFN0
```
PDE格式（8 bit） :
```
  VALID | PT6 ... PT0
```
其
```
VALID==1表示，表示映射存在；VALID==0表示，表示映射不存在。
PFN6..0:页帧号
PT6..0:页表的物理基址>>5
```
在[物理内存模拟数据文件](./03-2-spoc-testdata.md)中，给出了4KB物理内存空间的值，请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents。
```
Virtual Address 6c74
Virtual Address 6b22
Virtual Address 03df
Virtual Address 69dc
Virtual Address 317a
Virtual Address 4546
Virtual Address 2c03
Virtual Address 7fd7
Virtual Address 390e
Virtual Address 748b
```

比如答案可以如下表示：
```
Virtual Address 7570:
  --> pde index:0x1d  pde contents:(valid 1, pfn 0x33)
    --> pte index:0xb  pte contents:(valid 0, pfn 0x7f)
      --> Fault (page table entry not valid)
      
Virtual Address 21e1:
  --> pde index:0x8  pde contents:(valid 0, pfn 0x7f)
      --> Fault (page directory entry not valid)

Virtual Address 7268:
  --> pde index:0x1c  pde contents:(valid 1, pfn 0x5e)
    --> pte index:0x13  pte contents:(valid 1, pfn 0x65)
      --> Translates to Physical Address 0xca8 --> Value: 16
```

> Virtual Address 6c74:    
  --> pde index:0x1b  pde contents:(valid 1, pfn 0x20)  
    --> pte index:0x3  pte contents:(valid 1, pfn 0x61)  
      --> Translates to Physical Address 0xc34 --> Value: 6   
Virtual Address 6b22:  
  --> pde index:0x1a  pde contents:(valid 1, pfn 0x52)  
    --> pte index:0x19  pte contents:(valid 1, pfn 0x47)  
      --> Translates to Physical Address 0x8e2 --> Value: 1a   
Virtual Address 3df:  
  --> pde index:0x0  pde contents:(valid 1, pfn 0x5a)  
    --> pte index:0x1e  pte contents:(valid 1, pfn 0x5)  
      --> Translates to Physical Address 0xbf --> Value: f   
Virtual Address 69dc:  
  --> pde index:0x1a  pde contents:(valid 1, pfn 0x52)  
    --> pte index:0xe  pte contents:(valid 0, pfn 0x7f)  
      --> Fault (page directory entry not valid   
Virtual Address 317a:  
  --> pde index:0xc  pde contents:(valid 1, pfn 0x18)  
    --> pte index:0xb  pte contents:(valid 1, pfn 0x35)  
      --> Translates to Physical Address 0x6ba --> Value: 1e   
Virtual Address 4546:  
  --> pde index:0x11  pde contents:(valid 1, pfn 0x21)  
    --> pte index:0xa  pte contents:(valid 0, pfn 0x7f)  
      --> Fault (page directory entry not valid    
Virtual Address 2c03:  
  --> pde index:0xb  pde contents:(valid 1, pfn 0x44)  
    --> pte index:0x0  pte contents:(valid 1, pfn 0x57)  
      --> Translates to Physical Address 0xae3 --> Value: 16    
Virtual Address 7fd7:  
  --> pde index:0x1f  pde contents:(valid 1, pfn 0x12)  
    --> pte index:0x1e  pte contents:(valid 0, pfn 0x7f)  
      --> Fault (page directory entry not valid    
Virtual Address 390e:  
  --> pde index:0xe  pde contents:(valid 0, pfn 0x7f)  
    --> Fault (page directory entry not valid    
Virtual Address 748b:  
  --> pde index:0x1d  pde contents:(valid 1, pfn 0x0)  
    --> pte index:0x4  pte contents:(valid 0, pfn 0x7f)  
      --> Fault (page directory entry not valid  

（3）请基于你对原理课二级页表的理解，并参考Lab2建页表的过程，设计一个应用程序（可基于python, ruby, C, C++，LISP等）可模拟实现(2)题中描述的抽象OS，可正确完成二级页表转换。
      
> 应用程序代码见附件03-2-spoc-discussion.c，首先需要模拟一块内存并且初始化，在要进行虚实转化的时候首先访问第一级页表，第一级页表需要虚拟地址的前几位，然后通过中间几位再访问第二级页表，最后得到对应的物理地址。<br />
(运行此程序需要有data.txt初始化内存中的数据，add.txt放置要访问的内存（虚存），格式如题)

（4）假设你有一台支持[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)的机器，请问你如何设计操作系统支持这种类型计算机？请给出设计方案。

>  首先这个反置页表并不是一个真正的页表，实现支持反置页表的操作系统首先需要有一个非常强大的hash散列函数，这个页表的空间不需要特别大但是也需要足够大，反置页表结合了一个页表和框架表到一个数据结构。其核心是一个表的行数等于固定大小的帧数在内存中。如果有4000帧,倒页表有4000行。为每一行有一个条目为虚拟页码(VPN),物理页号(而不是物理地址)，接着我们以vpn为唯一的标识作为判断是否命中的依据，当已知虚拟地址通过hash映射之后发现进程ID不同的时候我们可以沿着next项向下查找直到找到符合的对应页表项，但是如果一直不命中则证明出现了错误，实现起来的话还需要维护页表项当中的这样一个next项用来处理hash的冲突情况。
 (5)[X86的页面结构](http://os.cs.tsinghua.edu.cn/oscourse/OS2015/lecture06#head-1f58ea81c046bd27b196ea2c366d0a2063b304ab)
--- 

## 扩展思考题

阅读64bit IBM Powerpc CPU架构是如何实现[反置页表](http://en.wikipedia.org/wiki/Page_table#Inverted_page_table)，给出分析报告。

> IPT结合了一个页表和框架表到一个数据结构。其核心是一个表的行数等于固定大小的帧数在内存中。如果有4000帧,倒页表有4000行。为每一行有一个条目为虚拟页码(VPN),物理页号(而不是物理地址),其他一些数据和方法创建一个碰撞链,稍后我们将会看到。
搜索所有条目的核心IPT结构效率低下,和一个哈希表可以使用虚拟地址映射(和地址空间/ PID信息如果需要)一个索引的IPT -这就是碰撞链。这个哈希表被称为哈希表锚。散列函数通常没有覆盖的优化——原始速度是更可取的。当然,哈希表经验碰撞。由于这种选择的哈希函数,我们可能会经历很多碰撞在使用,所以对于每个条目在表中提供的VPN是检查如果是搜索条目或碰撞。我们在实现的时候需要着重考虑hash函数的选择，当出现冲突的时候，我们使用类似于数据结构的解决散列冲突的办法对其进行冲突处理，保存下一个同样的hash值的页表项指针，确保一旦这个逻辑地址可以成功映射到物理地址，我们就一定能找到它而不会错过。
