#include <u.h>

ivec(void *isr)
{
	asm(LL, 8);
	asm(IVEC);
}

alltraps()
{
	asm(PSHA);
	asm(PSHB);

	asm(BIN);//read
	asm(PSHA);// a-->b
	asm(POPB);
	asm(LI, 1);// 1-->a
	asm(BOUT);// out

	asm(POPB);
	asm(POPA);
	asm(RTI);
}

main()
{
	ivec(alltraps);
	asm(STI);
	while(1);
}