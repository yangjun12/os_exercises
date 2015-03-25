#lec8 虚拟内存spoc练习


NOTICE
- 有"w4l2"标记的题是助教要提交到学堂在线上的。
- 有"w4l2"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。


## 个人思考题
---
(1)(w4l2)下面是一个体现内存访问局部性好的简单应用程序例子，请参考，在linux中写一个简单应用程序，体现内存局部性差，并给出其执行时间。
```
#include <stdio.h>
#define NUM 1024
#define COUNT 10
int A[NUM][NUM];
void main (void) {
  int i,j,k;
  for (k = 0; k<COUNT; k++)
  for (i = 0; i < NUM; i++)
  for (j = 0; j	 < NUM; j++)
      A[i][j] = i+j;
  printf("%d count computing over!\n",i*j*k);
}
```
可以用下的命令来编译和运行此程序：
```
gcc -O0 -o goodlocality goodlocality.c
time ./goodlocality
```
可以看到其执行时间。

> 第一个程序的时间输出结果如下：  
10485760 count computing over!  
real	0m0.174s  
user	0m0.124s  
sys	0m0.028s  
如果将两个循环当中的i和j调换位置的话结果如下：  
10485760 count computing over!  
real	0m0.670s  
user	0m0.624s  
sys	0m0.012s  
显然第二个局部性差一些，执行的时间更长



## 小组思考题目
----

（1）缺页异常可用于虚拟内存管理中。如果在中断服务例程中进行缺页异常的处理时，再次出现缺页异常，这时计算机系统（软件或硬件）会如何处理？请给出你的合理设计和解释。

> 我认为这是属于异常嵌套的一种情况，显然这种中断服务例程当中的缺页异常的情况优先级更高，我想计算机系统会将上一个缺页异常的情况入栈保存起来，优先处理这种优先级别高的缺页异常，当发生优先级别更高的异常嵌套的时候还是会把优先级别低的存到栈里面，当最高级别优先级的异常处理完之后再开始从栈中一个一个地把异常弹出再依次处理。
以上是计原课程的认识……根据操作系统intel的知识，两次异常系统就崩溃了……

（2）如果80386机器的一条机器指令(指字长4个字节)，其功能是把一个32位字的数据装入寄存器，指令本身包含了要装入的字所在的32位地址。这个过程最多会引起几次缺页中断？
> 会产生16次的缺页中断

（3）(spoc) 有一台假想的计算机，页大小（page size）为32 Bytes，支持8KB的虚拟地址空间（virtual address space）,有4KB的物理内存空间（physical memory），采用二级页表，一个页目录项（page directory entry ，PDE）大小为1 Byte,一个页表项（page-table entries
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
VALID==1表示，表示映射存在；VALID==0表示，表示内存映射不存在（有两种情况：a.对应的物理页帧swap out在硬盘上；b.既没有在内存中，页没有在硬盘上）。
PFN6..0:页帧号
PT6..0:页表的物理基址>>5
```

已经建立好了1个页目录表和8个页表，且页目录表的index为0~7的页目录项分别对应了这8个页表。

在[物理内存模拟数据文件](./04-1-spoc-memdiskdata.md)中，给出了4KB物理内存空间和4KBdisk空间的值，PDBR的值。

请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents，the value of addr in phy page OR disk sector。
```
Virtual Address 6653:
Virtual Address 1c13:
Virtual Address 6890:
Virtual Address 0af6:
Virtual Address 1e6f:
```

回答可参考以如下表示：
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

Virtual Address 106f:
  --> pde index:0x3  pde contents:(valid 1, pfn 0x2d)
    --> pte index:0x14  pte contents:(valid 0, pfn 0x06)
      --> To Disk Sector Address 0x167 --> Value: 2c
```
**提示:**
```
页大小（page size）为32 Bytes(2^5)
页表项1B

8KB的虚拟地址空间(2^15)
一级页表：2^5
PDBR content: 0xd80（1101_100 0_0000, page 0x6c）

page 6c: e1(1110 0001) b5(1011 0101) a1(1010 0001) c1(1100 0001)
         b3(1011 0011) e4(1110 0100) a6(1010 0110) bd(1011 1101)
二级页表：2^5
页内偏移：2^5

4KB的物理内存空间（physical memory）(2^12)
物理帧号：2^7

Virtual Address 0330(0 00000 11001 1_0000):
  --> pde index:0x0(00000)  pde contents:(0xe1, 11100001, valid 1, pfn 0x61(page 0x61))
  page 61: 7c 7f 7f 4e 4a 7f 3b 5a 2a be 7f 6d 7f 66 7f a7
           69 96 7f c8 3a 7f a5 83 07 e3 7f 37 62 30 7f 3f 
    --> pte index:0x19(11001)  pte contents:(0xe3, 1 110_0011, valid 1, pfn 0x63)
  page 63: 16 00 0d 15 00 1c 1d 16 02 02 0b 00 0a 00 1e 19
           02 1b 06 06 14 1d 03 00 0b 00 12 1a 05 03 0a 1d
      --> To Physical Address 0xc70(110001110000, 0xc70) --> Value: 02

Virtual Address 1e6f(0 001_11 10_011 0_1111):
  --> pde index:0x7(00111)  pde contents:(0xbd, 10111101, valid 1, pfn 0x3d)
  page 6c: e1 b5 a1 c1 b3 e4 a6 bd 7f 7f 7f 7f 7f 7f 7f 7f
           7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f
  page 3d: f6 7f 5d 4d 7f 04 29 7f 1e 7f ef 51 0c 1c 7f 7f
           7f 76 d1 16 7f 17 ab 55 9a 65 ba 7f 7f 0b 7f 7f 
    --> pte index:0x13  pte contents:(0x16, valid 0, pfn 0x16)
  disk 16: 00 0a 15 1a 03 00 09 13 1c 0a 18 03 13 07 17 1c 
           0d 15 0a 1a 0c 12 1e 11 0e 02 1d 10 15 14 07 13
      --> To Disk Sector Address 0x2cf(0001011001111) --> Value: 1c
```
> #include <stdio.h>
#include <stdlib.h>
unsigned int  mem[128][32];
unsigned int disk[128][32];
int char2int(char a){
	return (a>58)?(a-87):(a-48);
}
int value2(char* a,int start){
	return char2int(a[start])*16+char2int(a[start+1]);
}
int value4(char* a,int start){
	int i=0,sum=0;
	for(;i<4;i++){
		sum*=16;
		sum+=char2int(a[start+i]);
	}
	return sum;
}
void catch(int value){
	//printf("%d %d\n",value>>10,mem[17][value>>10]);
	if(mem[108][value>>10]/128==0){
		printf("	--> pde index:0x%x  pde contents:(valid 0, pfn 0x%x)\n",value>>10,mem[108][value>>10]%128);
      		printf("		--> Fault (page directory entry not valid)\n");
	}else{
		printf("	--> pde index:0x%x  pde contents:(valid 1, pfn 0x%x)\n",value>>10,mem[108][value>>10]%128);
		if(mem[mem[108][value>>10]%128][(value>>5)%32]/128==0){
			printf("		--> pte index:0x%x  pte contents:(valid 0, pfn 0x%x)\n",(value>>5)%32,mem[mem[108][value>>10]%128][(value>>5)%32]%128);
			//printf("			--> Fault (page table entry not valid)\n");
			int address=value%32+(mem[mem[108][value>>10]%128][(value>>5)%32]%128)*32;
			printf("			--> To Disk Sector Address 0x%x --> Value:%x\n",address,disk[address/32][address%32]);
		}else{
			printf("		--> pte index:0x%x  pte contents:(valid 1, pfn 0x%x)\n",(value>>5)%32,mem[mem[108][value>>10]%128][(value>>5)%32]%128);
			int address=value%32+(mem[mem[108][value>>10]%128][(value>>5)%32]%128)*32;
			//printf("%d\n",address);
			printf("			 --> Translates to Physical Address 0x%x --> Value: 0x%x\n",address,mem[address/32][address%32]);
		}
	}
}
int main(){
	FILE *fp=fopen("data.txt","rt");
	int index=0;
	char a[1024];
	int offset = 9;
	while(!feof(fp)){
		fgets(a,1000,fp);
		int i=0;
		for(;i<32;i++){
			mem[index][i]=value2(a,offset+3*i);		
		}
		index++;
	};
FILE *fp1=fopen("disk.txt","rt");
	index=0;
	while(!feof(fp1)){
		fgets(a,1000,fp1);
		int i=0;
		for(;i<32;i++){
			//printf("%d %d %d\n",index,i,value2(a,offset+3*i));
			disk[index][i]=value2(a,offset+3*i);		
		}
		index++;
	};

	FILE *fin=fopen("add.txt","rt");
	while(!feof(fin)){
		fgets(a,100,fin);
		printf("Virtual Address 0x%x:\n",value4(a,16));
		catch(value4(a,16));
	}
	return 0;
}
请运行代码……经检验正确

## 扩展思考题
---
(1)请分析原理课的缺页异常的处理流程与lab3中的缺页异常的处理流程（分析粒度到函数级别）的异同之处。
> pass
