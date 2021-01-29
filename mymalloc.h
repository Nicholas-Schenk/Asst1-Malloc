#ifndef HEADER_FILE
#define HEADER_FILE

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

void print_memory();
void initialize_memory();
void* find_free(int size);
void* mymalloc (int size, char file[], int line);
int myfree(void* p, char file[], int line);
void combine_free();

#endif
