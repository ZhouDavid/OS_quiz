###在v9-cpu中如何实现时钟中断的；
使用TIME指令设置timeout，如果timeout不为0，timer会进行计时，在每个周期timer都会增加delta，直到timer>timeout，就会产生一个时钟中断。

如果使能，那么直接goto interrupt,进行中断处理；
如果没有使能，那么在ipend待处理标志位进行标记。

###v9-cpu指令，关键变量描述有误或不全的情况；
 - a: a寄存器
 - b: b寄存器
 - c: c寄存器
 - f: f浮点寄存器
 - g: g浮点寄存器
 - ir:　指令寄存器
 - xpc: pc在host内存中的值
 - fpc: pc在host内存中所在页的下一页的起始地址值
 - tpc: pc在host内存中所在页的起始地址值
 - xsp: sp在host内存中的值
 - tsp: sp在host内存中所在页的起始地址值
 - fsp: 辅助判断是否要经过tr/tw的分析
 - ssp: 核心态栈指针
 - usp: 用户态栈指针
 - cycle: 
 - xcycle:
 - timer:计时变量
 - timeout:计时器timeout中断发生的阈值
 -　delta:计时变量timer每个周期增加的量


###在v9-cpu中的跳转相关操作是如何实现的；
基本都是对xpc进行修改，如非法则跳转至fixpc，否则跳转至next

不过B指令首先会对条件进行判断


###在v9-cpu中如何设计相应指令，可有效实现函数调用与返回；
JSR和JSRA可以很方便地在跳转的同时保存原来的PC

返回的时候直接跳转到RA即可


###emhello/os0/os1等程序被加载到内存的哪个位置,其堆栈是如何设置的；


###在v9-cpu中如何完成一次内存地址的读写的；


###在v9-cpu中如何实现分页机制；



