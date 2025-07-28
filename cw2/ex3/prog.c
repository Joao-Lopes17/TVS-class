#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>

#define DATA_SIZE 16*1024*1024 // 16MB 
#define INCREASE_RSS 3*1024*1024 // 3MB
#define SIZE_TO_KEEP 1.5*1024*1024 // 1.5MB
#define BYTES_TO_ACCESS 256
#define DIRTY_SIZE_PAGES 128*1024 // 128KB
#define CODE_PAGES 1
#define DATA_PAGES 128

char info[DATA_SIZE];
char data[DATA_SIZE] = {1};

int main() {
	printf("PID: %u\n", getpid());

	const long PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
	printf("PAGE_SIZE: %ld\n", PAGE_SIZE);

	printf("#1 (press ENTER to continue)"); getchar();

	// a - Increase the resident set (Rss) by about 3MB in the .bss region
	for (int i = 0; i < INCREASE_RSS; i++) {
	 	info[i] = 1;
	}	
	printf("#2 (press ENTER to continue)"); getchar();
	// b - Access 256 bytes of initialized data (.data) with maximum impact in Private Clean pages.
	for (int i = 0; i < BYTES_TO_ACCESS * PAGE_SIZE; i++) {
        char temp = data[i]; 
		(void)temp;
    }	
	printf("#3 (press ENTER to continue)"); getchar();

	// c - Reduce the Pss of non-initialized data (.bss) to around 1.5MB for 30 seconds, while keeping Rss.
    madvise(info + (size_t) SIZE_TO_KEEP, (size_t) (DATA_SIZE-SIZE_TO_KEEP), MADV_DONTNEED);
	sleep(30); 
	
	printf("#4 (press ENTER to continue)"); getchar();

	// d - Execute a single operating system function that results in two new regions being added to the
	//	   existing address space, one with code (~ 4KB) and one with data (~ 512KB).

    void *handle = dlopen("./library.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Erro ao carregar a biblioteca: %s\n", dlerror());
        return EXIT_FAILURE;
    }
    void *(*alloc_text)(size_t) = dlsym(handle, "alloc_text");
    void *(*alloc_data)(size_t) = dlsym(handle, "alloc_data");

    if (!alloc_text || !alloc_data) {
        fprintf(stderr, "Erro ao carregar as funções: %s\n", dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

	void *code = alloc_text(CODE_PAGES);
    void *data = alloc_data(DATA_PAGES);
	printf("\nCode: %p\n", code);
	printf("Data: %p\n", data);
	sleep(30); 
	printf("after 30s"); getchar();

	dlclose(handle);

	printf("#5 (press ENTER to continue)"); getchar();

	// e - Increase Private dirty pages by about 128KB in the region for the data section created in d).
	for(int i = 0; i < DIRTY_SIZE_PAGES; i++) {
		((char *)data)[i] = 1;
	}
	printf("END (press ENTER to continue)"); getchar();
	return 0;
}
