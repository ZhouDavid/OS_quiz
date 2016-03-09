#（1）(spoc) 某系统使用请求分页存储管理，若页在内存中，满足一个内存请求需要150ns (10^-9s)。若缺页率是10%，为使有效访问时间达到0.5us(10^-6s),求不在内存的页面的平均访问时间。请给出计算步骤。

500=0.9*150+0.1*x

x = 3650ns = 3.65ms

#（2）(spoc) 有一台假想的计算机，页大小（page size）为32 Bytes（5bit），支持32KB(15bit)的虚拟地址空间（virtual address space）,有4KB(12bit)的物理内存空间（physical memory），采用二级页表，一个页目录项（page directory entry ，PDE）大小为1 Byte,一个页表项（page-table entries PTEs）大小为1 Byte，1个页目录表大小为32 Bytes(32项 max 5bit)，1个页表大小为32 Bytes(32项 max 5bit)。页目录基址寄存器（page directory base register，PDBR）保存了页目录表的物理地址（按页对齐）。

PTE格式（8 bit） :

  VALID | PFN6 ... PFN0
PDE格式（8 bit） :

  VALID | PT6 ... PT0
其

VALID==1表示，表示映射存在；VALID==0表示，表示映射不存在。
PFN6..0:页帧号，page frame number(phisical)
PT6..0:页表的物理基址>>5, page table base address
在物理内存模拟数据文件中，给出了4KB物理内存空间的值，请回答下列虚地址是否有合法对应的物理内存，请给出对应的pde index, pde contents, pte index, pte contents。

<!--virtual 15 = 5 + 5 + 5-->
<!--physical 12 = 7 + 5-->

1) Virtual Address 6c74
   Virtual Address 6b22
2) Virtual Address 03df
   Virtual Address 69dc
3) Virtual Address 317a
   Virtual Address 4546
4) Virtual Address 2c03
   Virtual Address 7fd7
5) Virtual Address 390e
   Virtual Address 748b

answer:

3) Virtual Address 317a = 011 00 - 01 011 - 1 1010   
   <!--9d = 1001 1110-->
   
   pde index: 0x0c	pde contents:(valid 1 pt 0x18)
   
   <!--7f = 0111 1111 -->
   pte index: 0x0b	pte contends:(valid 1 pfn 0x35)
   
   Physical addr: 0x6ba  (0110 1011 1010)

   Data: 1e
   
   
   Virtual Address 4546 = 100 01 - 01 010 - 0 0110
   
   pde index: 0x11  pde contents:(valid 1 pt 0x21)
   
   pte indx: 0x0a	pte contens:(valid 0 pfn 0x7f)
   
	invalid
   
<!--
比如答案可以如下表示：

Virtual Address 7570: 0111 0101 0111 0000
  -\-> pde index:0x1d  pde contents:(valid 1, pfn 0x33)
    -\-> pte index:0xb  pte contents:(valid 0, pfn 0x7f)
      -\-> Fault (page table entry not valid)

Virtual Address 21e1:
  -\-> pde index:0x8  pde contents:(valid 0, pfn 0x7f)
      -\-> Fault (page directory entry not valid)

Virtual Address 7268:
  -\-> pde index:0x1c  pde contents:(valid 1, pfn 0x5e)
    -\-> pte index:0x13  pte contents:(valid 1, pfn 0x65)
      -\-> Translates to Physical Address 0xca8 -\-> Value: 16
-->

#（3）请基于你对原理课二级页表的理解，并参考Lab2建页表的过程，设计一个应用程序（可基于python, ruby, C, C++，LISP等）可模拟实现(2)题中描述的抽象OS，可正确完成二级页表转换。

#（4）假设你有一台支持反置页表的机器，请问你如何设计操作系统支持这种类型计算机？请给出设计方案。