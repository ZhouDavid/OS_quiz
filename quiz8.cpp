#include <stdio.h>
#define MEMORY_SIZE 4096
#define DISK_SIZE 4096

void *memory;
void *disk;
int pdbr;
void load_memory(){
	pdbr = 0xd80;
	FILE *f = fopen("quiz8_physics.txt", "r");
	char *mem = new char[MEMORY_SIZE];
	int num;

	for(int i = 0; i < MEMORY_SIZE; i++){
		fscanf(f, "%0x", &num);
		mem[i] = num;
		// printf("%#d: %#0x\n", i, num);
	}
	memory = (void*)mem;
}

void load_disk(){
	pdbr = 0xd80;
	FILE *f = fopen("quiz8_disk.txt", "r");
	char *mem = new char[DISK_SIZE];
	int num;

	for(int i = 0; i < MEMORY_SIZE; i++){
		fscanf(f, "%0x", &num);
		mem[i] = num;
		// printf("%#d: %#0x\n", i, num);
	}
	disk = (void*)mem;
}

unsigned int visit_disk(unsigned int address){
	unsigned int data = ((char*)disk)[address];
	return data;
}

unsigned int visit_memory(unsigned int address){
	printf("visiting virtual address %#0x\n", address);
	unsigned int pde = address >> 10;
	unsigned int pte = (address >> 5) & 31;
	
	unsigned int pt = ((char *)memory)[pdbr + pde];
	unsigned int valid = (pt >> 7) & 1;
	pt &= 127;
	printf("--> pde index:%#0x  pde contents:(valid %d, pt %#0x)\n", pde, valid, pt);
	if(valid != 1){
		if(pt == 0x7f){
			printf("\t\t--> Invalid addr\n");
			return -1;
		}else{
			// in the disk
			printf("\t\t--> unknown\n");
			return -1;
		}
	}

	int pfn = ((char *)memory)[(pt<<5) + pte];
	valid &= ((pfn >> 7) & 1);
	pfn &= 127;

	printf("\t--> pte index:%#0x  pde contents:(valid %d, pfn %#0x)\n", pte, valid, pfn);
	if(valid != 1){
		if(pfn == 0x7f){
			printf("\t\t--> Invalid addr\n");
			return -1;
		}else{
			unsigned int pa = (pfn << 5) + (address & 31);
			unsigned int data = visit_disk(pa);
			printf("\t\t--> To Disk Address: %#0x --->value: %#0x\n", pa, data);
			return -1;
		}	
	}
	
	int pa = (pfn << 5) + (address & 31);
	int data = ((char *)memory)[pa];
	printf("\t\t--> To Physical Address: %#0x --->value: %#0x\n", pa, data);
	
	// if(valid != 1){
	// 	printf("\tinvalid addr\n");
	// 	return -1;
	// }else{
	// 	printf("\tdata is %#0x\n", data);
	// }

	return data;
}

int main(){
	load_memory();
	load_disk();
	printf("memory loaded...\n");
	visit_memory(0x0330);
	visit_memory(0x1e6f);
	

	visit_memory(0x6653);
	visit_memory(0x1c13);
	visit_memory(0x6890);
	visit_memory(0x0af6);
	visit_memory(0x1e6f);
}
