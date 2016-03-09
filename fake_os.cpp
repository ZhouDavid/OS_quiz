#include <stdio.h>
#define MEMORY_SIZE 4096


void *memory;
int pdbr;
void load_memory(){
	pdbr = 0x220;
	FILE *f = fopen("processed_data.txt", "r");
	char *mem = new char[MEMORY_SIZE];
	int num;

	for(int i = 0; i < MEMORY_SIZE; i++){
		fscanf(f, "%d", &num);
		mem[i] = num;
		printf("%d: %d\n", i, num);
	}
	memory = (void*)mem;
}

int visit_memory(int address){
	printf("visiting virtual address %0x\n", address);
	int pde = address >> 10;
	int pte = (address >> 5) & 31;
	int pt = ((char *)memory)[pdbr + pde];
	int valid = (pt >> 7) & 1;
	
	pt &= 127;
	int pfn = ((char *)memory)[(pt<<5) + pte];
	valid &= ((pfn >> 7) & 1);
	
	pfn &= 127;
	int pa = (pfn << 5) + (address & 31);
	int data = ((char *)memory)[pa];
	printf("\tpde,pte,pt,pfn,pa: %0x,%0x,%0x,%0x,%0x\n", pde,pte,pt,pfn,pa);
	
	if(valid != 1){
		printf("\tinvalid addr\n");
		return -1;
	}else{
		printf("\tdata is %0x\n", data);
	}

	return data;
}

int main(){
	load_memory();
	printf("memory loaded...\n");
	visit_memory(0x317a);
	visit_memory(0x4546);
}
