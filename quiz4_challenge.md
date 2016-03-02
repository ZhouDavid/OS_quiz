####(challenge)写出一些简单的小程序（c or asm）来体现这些特权级的区别和联系。
challenge代码：
使用lab1的答案进行修改。
我们修改ds寄存器，设置为GD_KDATA|0(RPL为0)没有什么问题，但是GD_KDATA|1(RPL is 1)就发生了unexpected trap in kernel,一定程度上反映了设置DS时候需要满足下面的式子

DPL >= max(CPL, RPL)


	lab1_print_cur_status();

    asm volatile("movl %0, %%eax;"
    		"movw %%ax, %%ds;"
    		:
    		: "i"(GD_KDATA|0));

    asm volatile("movl %0, %%eax;"
        		"movw %%ax, %%ds;"
        		:
        		: "i"(GD_KDATA|1));

    lab1_print_cur_status();


####(challenge)写出一些简单的小程序（c or asm）来显示出不同特权级的的中断切换的堆栈变化情况。
```
static void
lab1_switch_test(void) {
    lab1_print_cur_status();
    print_stackframe();

    cprintf("+++ switch to  user  mode +++\n");
    lab1_switch_to_user();
    lab1_print_cur_status();
    print_stackframe();

    cprintf("+++ switch to kernel mode +++\n");
    lab1_switch_to_kernel();
	lab1_print_cur_status();
	print_stackframe();
}
```

得到的结果如下

```
0: @ring 0
0:  cs = 8
0:  ds = 10
0:  es = 10
0:  ss = 10
0:  esp = 7ba0
0:  ebp = 7bb8
ebp:0x00007bb8 eip:0x001009b7 args:0x0010e560 0x00000100 0x00007be8 0x00100069 
    kern/debug/kdebug.c:305: print_stackframe+21
ebp:0x00007bc8 eip:0x001001fa args:0x00000000 0x00000000 0x0010fd80 0x00103440 
    kern/init/init.c:137: lab1_switch_test+15
ebp:0x00007be8 eip:0x00100069 args:0x00000000 0x00000000 0x00000000 0x00007c4f 
    kern/init/init.c:46: kern_init+104
ebp:0x00007bf8 eip:0x00007d63 args:0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8 
    <unknow>: -- 0x00007d62 --
+++ switch to  user  mode +++
1: @ring 3
1:  cs = 1b
1:  ds = 23
1:  es = 23
1:  ss = 23
1:  esp = 7ba0
1:  ebp = 7bb8
ebp:0x00007bb8 eip:0x001009b7 args:0x0010e560 0x00000100 0x00007be8 0x00100069 
    kern/debug/kdebug.c:305: print_stackframe+21
ebp:0x00007bc8 eip:0x00100219 args:0x00000000 0x00000000 0x0010fd80 0x00103440 
    kern/init/init.c:142: lab1_switch_test+46
ebp:0x00007be8 eip:0x00100069 args:0x00000000 0x00000000 0x00000000 0x00007c4f 
    kern/init/init.c:46: kern_init+104
ebp:0x00007bf8 eip:0x00007d63 args:0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8 
    <unknow>: -- 0x00007d62 --
+++ switch to kernel mode +++
2: @ring 0
2:  cs = 8
2:  ds = 10
2:  es = 10
2:  ss = 10
2:  esp = 7ba0
2:  ebp = 7bb8
ebp:0x00007bb8 eip:0x001009b7 args:0x0010e560 0x00000100 0x00007be8 0x00100069 
    kern/debug/kdebug.c:305: print_stackframe+21
ebp:0x00007bc8 eip:0x00100238 args:0x00000000 0x00000000 0x0010fd80 0x00103440 
    kern/init/init.c:147: lab1_switch_test+77
ebp:0x00007be8 eip:0x00100069 args:0x00000000 0x00000000 0x00000000 0x00007c4f 
    kern/init/init.c:46: kern_init+104
ebp:0x00007bf8 eip:0x00007d63 args:0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8 
    <unknow>: -- 0x00007d62 --
```
