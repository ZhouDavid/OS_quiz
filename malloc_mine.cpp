// structure 
// sz, data[sz], sz
#include <stdio.h>

char *vmemory;
int total_size;

struct header{
	int sz;
	char allocated;
	char is_header;
	char offset[2];
};

struct tailer{
	int sz;
	char allocated;
	char is_header;
	char memory_end;
	char offset;
};

void set_memory_block(char *p, int sz){
	((struct header*)p)->sz = sz - sizeof(struct header) * 2;
	((struct header*)p)->allocated = 0;
	((struct header*)p)->is_header = 1;

	p = p + sz - sizeof(struct tailer);
	((struct tailer*)p)->sz = sz - sizeof(struct header) * 2;
	((struct tailer*)p)->allocated = 0;
	((struct tailer*)p)->is_header = 1;	
	((struct tailer*)p)->memory_end = 1;
}

void init(){
	// memory totally 128MB
	total_size = 1024 * 1024 * 128;
	vmemory = (char *)new char[1024 * 1024 * 128];
	set_memory_block(vmemory, total_size);
}

char* previous_block(char *p){
	int previous_sz = ((struct tailer*)(p - sizeof(struct tailer)))->sz;
	return p - previous_sz - sizeof(struct header) * 2;
}

char* next_block(char *p){
	return p + ((struct header*)p)->sz + sizeof(struct header) * 2;
}

int check_allocated(char *p){
	return (int)(((struct header*)p)->allocated);
}

int check_size(char *p, int sz){
	return (int)(((struct header*)p)->sz) >= sz ? 1 : 0;	
}

void * malloc_mine(int sz){
	char *p = vmemory;
	int notfound = 0;
	
	// printf("%d\n", check_allocated(p));
	while((long)(p - vmemory) < total_size && (check_allocated(p) > 0 || check_size(p, sz) <= 0)){
		p = next_block(p);
	}

	if((long)(p - vmemory) >= total_size){
		return 0;
	}else{
		// printf("%0lx\n", (long)((struct header*)p)->sz);
		if(sz < ((struct header*)p)->sz - sizeof(struct header) * 2){
			int block_sz = ((struct header*)p)->sz;
			int malloc_sz = sz;
			((struct header *)(p))->sz = malloc_sz;
			((struct header *)(p))->allocated = 1;
			*(struct header *)(p + malloc_sz + sizeof(struct header)) = *(struct header *)(p);
			char *new_block_header = next_block(p);
			set_memory_block(new_block_header, block_sz - malloc_sz);
		}else{
			((struct header *)(p))->allocated = 1;
		}
		return p + sizeof(struct header);
	}
}

void print_block(){
	char *p = vmemory;
	int notfound = 0;
	
	// printf("%d\n", check_allocated(p));
	while((long)(p - vmemory) < total_size){
		printf("-->%d@%ld (%d)", ((struct header*)(p))->sz, ((long)(p) - (long)vmemory), ((struct header*)(p))->allocated);
		p = next_block(p);
	}
	printf("\n\n");
}

int free_mine(void *p){
	struct header *header = (struct header *)((char *)p - sizeof(struct header));
	if(header->allocated <= 0 || header->is_header <= 0){
		return -1;
	}
	header->allocated = 0;
	int free_sz = header->sz;
	// try to combile free area;
	// find previous
	char *prev = previous_block((char*)header);
	char *next = next_block((char*)header);
	
	if((long)(prev - vmemory) >= 0 && check_allocated(prev) <= 0){
		// combine previous
		// ((struct header*)prev)->sz += free_sz + sizeof(struct header) * 2;
		set_memory_block((char*)prev, ((struct header*)prev)->sz + free_sz + sizeof(struct header) * 4);
	}else{
		prev = (char *)(header);
	}


	if((long)(next - vmemory) < total_size && check_allocated(next) <= 0){
		// combine prev and next
		// ((struct header*)prev)->sz += free_sz + sizeof(struct header) * 2;
		set_memory_block((char*)prev, ((struct header*)prev)->sz + ((struct header*)next)->sz + sizeof(struct header) * 4);
	}
}

int main(){
	init();
	void *p1 = malloc_mine(256);
	printf("p1: %d@%ld\n", 256, (long)p1 - (long)vmemory);
	print_block();

	void *p11 = malloc_mine(256);
	printf("p11: %d@%ld\n", 256, (long)p11 - (long)vmemory);
	print_block();

	void *p2 = malloc_mine(16);
	printf("p2: %d@%ld\n", 16, (long)p2 - (long)vmemory);
	print_block();

	void *p3 = malloc_mine(total_size);
	printf("p3: %d@%ld\n", total_size, (long)p3);
	print_block();
	
	free_mine(p1);
	printf("free p1");
	print_block();

	free_mine(p11);
	printf("free p11");
	print_block();

	void *p4 = malloc_mine(128);
	printf("p4: %d@%ld\n", 128, (long)p4 - (long)vmemory);
	print_block();
	
	void *p5 = malloc_mine(256 + 128);
	printf("p5: %d@%ld\n", 256 + 128, (long)p5 - (long)vmemory);
	print_block();
	
	void *p6 = malloc_mine(1);
	printf("p6: %d@%ld\n", 1, (long)p6 - (long)vmemory);
	print_block();
	

	return 0;
}