typedef struct {
	int free; //is the space after free or not
	int size; //size of free space?
} metadata;

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void totalBytes();

#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
