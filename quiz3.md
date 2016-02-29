## 3.5 ucore系统调用分析
 1. ucore的系统调用中参数传递代码分析。

	ucore中的系统调用参数传递通过寄存器完成。

	
		static inline int
		syscall(int num, ...) {
		    va_list ap;
		    va_start(ap, num);
		    uint32_t a[MAX_ARGS];
		    int i, ret;
		    for (i = 0; i < MAX_ARGS; i ++) {
		        a[i] = va_arg(ap, uint32_t);
		    }
		    va_end(ap);
		
		    asm volatile (
		        "int %1;"
		        : "=a" (ret)
		        : "i" (T_SYSCALL),
		          "a" (num),
		          "d" (a[0]),
		          "c" (a[1]),
		          "b" (a[2]),
		          "D" (a[3]),
		          "S" (a[4])
		        : "cc", "memory");
		    return ret;
		}

	首先把syscall的不定参数传到数组a中
	
	一次传入num、a[0]...a[4]到各个寄存器，eax,edx,ecx,ebx,edi,esi,多的传到内存
	
 
 1. 以getpid为例，分析ucore的系统调用中返回结果的传递代码。
		
				        : "=a" (ret)
				        
	实际上传到了eax寄存器，然后到ret变量，进行返回
	
 
 1. 以ucore lab8的answer为例，分析ucore应用的系统调用编写和含义。

	ucore应用可以使用syscall.c中的包装好了的系统调用。或者一些其他的。
	
	首先有一个syscall原始的系统调用函数，然后基于syscall进行包装的有sys_putc、sys_getpid等系统调用，最后还有fork(),wait()等库函数对系统调用进一步包装，便于用户使用。
 
 1. 以ucore lab8的answer为例，尝试修改并运行ucore OS kernel代码，使其具有类似Linux应用工具`strace`的功能，即能够显示出应用程序发出的系统调用，从而可以分析ucore应用的系统调用执行过程。
 
基本思路，主要在syscall中添加一些代码，对各个syscall number的调用次数进行统计即可

	
 