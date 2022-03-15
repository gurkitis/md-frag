#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MY_BUFFER_SIZE 1024

char myBuffer[MY_BUFFER_SIZE];
char* lastPos = myBuffer;
char* allocAgorithm;

/*
 *  === BLOCK STRUCT ===
 *  (unsigned int): allocated bytes
 *  (unsigned int): chunk size
 *  ~~~
 *  (char*): chunk conentents
 *  ~~~
 *  (char): has next block
 */
const int sizeBook = 2* sizeof(unsigned int) + sizeof(char);
int chunks = 0;
/*
 * === PRINT MODES === 
 * 0: print each block 
 * 1: print only total stats
 * 2: print all stats
 */
void printBufferStats(int mode)
{
    printf("Book size: %i\n", sizeBook);
    unsigned int x;
    double meanInnerFrag;
    double meanOutterFrag;
    double setBlocks = 0;
    double totalBlocks = 0;
    double setTotalMemory = 0;

    for (x = 0; x < MY_BUFFER_SIZE; x += sizeBook + *((unsigned int*) &myBuffer[x] + 1)) {
        double size = (double)*((unsigned int*) &myBuffer[x]);
        double chunk = (double)*((unsigned int*) &myBuffer[x] + 1);
        double blockSize = sizeBook + chunk;

        setTotalMemory += size;
        totalBlocks++;
        if (size > 0) {
            setBlocks++;
            meanInnerFrag += size/chunk;
            meanOutterFrag += size/blockSize;
        }

        if (mode == 0 || mode == 2) {
            printf("Address: %u\n", x);
            printf(
                "=== BLOCK ===\nsize: %u\nchunk: %u\ntotal size: %u\ninner fragmentation: %f\noutter fragmentation: %f\n\n", 
                (unsigned int)size, (unsigned int)chunk, (unsigned int)blockSize, size/chunk, size/blockSize
            );
        }
        if (myBuffer[x + sizeBook + *((unsigned int*) &myBuffer[x] + 1) - sizeof(char)] == '0') break;
    }
}

void myAllocInit(char* filename)
{
    static unsigned int ptr = 0;
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("File: %s can't be accessed\n", filename);
        exit(EXIT_FAILURE);
    }

    unsigned int chunk;
    while (!feof(file)) {
        fscanf(file, "%u\n", &chunk);
        if (ptr + sizeBook + chunk >= MY_BUFFER_SIZE) {
            printf("Buffer overflow!\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        *((unsigned int*) &myBuffer[ptr]) = 0;
        ptr+= sizeof(unsigned int);
        *((unsigned int*) &myBuffer[ptr]) = chunk;
        ptr += sizeof(unsigned int);
        ptr += chunk;
        myBuffer[ptr] = '1';
        ptr += sizeof(char);
        chunks++;
    }

    fclose(file);
    ptr -= sizeof(char);
    myBuffer[ptr] = '0';
}

void* worstFit(size_t size)
{
    char* worstPtr = NULL;
    unsigned int x;
    for (x = 0; x < MY_BUFFER_SIZE; x += sizeBook + *((unsigned int*) &myBuffer[x] + 1)) {
        if (size + *((unsigned int*) &myBuffer[x]) <= *((unsigned int*) &myBuffer[x] + 1)) {
            if (worstPtr == NULL) {
                worstPtr = &myBuffer[x];
            } else if (*((unsigned int*) &myBuffer[x] + 1) > *((unsigned int*) worstPtr + 1)) {
                worstPtr = &myBuffer[x];
            }
        }
        if (myBuffer[x + sizeBook + *((unsigned int*) &myBuffer[x] + 1) - sizeof(char)] == '0') break;
    }
    return (void*) worstPtr;
}
void* nextFit(size_t size)
{
    char *tmp = lastPos;
    /*printf("try to set: %i\n", size);*/
    int cnt = 0, x;
    for (x = lastPos - myBuffer; x < MY_BUFFER_SIZE;) {
        printf("trying to set: %i in %i chunk, takenSpace: %i, totalsize: %i\n", size, x,
               *((unsigned int *) &myBuffer[x]), *((unsigned int *) &myBuffer[x] + 1));
        if (size + *((unsigned int *) &myBuffer[x]) <= *((unsigned int *) &myBuffer[x] + 1)) {
            tmp = &myBuffer[x];
            break;
        }
        if (cnt >= chunks) {
            lastPos = tmp;
            return NULL;
        }
        if (myBuffer[x + sizeBook + *((unsigned int *) &myBuffer[x] + 1) - sizeof(char)] == '0') {
            x = 0;
            cnt++;
            continue;
        }
        x += sizeBook + *((unsigned int *) &myBuffer[x] + 1);
        cnt++;
    }
    lastPos = tmp;
    return (void *) tmp;
}

void* bestFit(size_t size)
{
    char* bestPtr = NULL;
    unsigned int x;
    for (x = 0; x < MY_BUFFER_SIZE; x += sizeBook + *((unsigned int*) &myBuffer[x] + 1)) {
        if (size + *((unsigned int*) &myBuffer[x]) <= *((unsigned int*) &myBuffer[x] + 1)) {
            if (bestPtr == NULL) {
                bestPtr = &myBuffer[x];
            } else if (*((unsigned int*) &myBuffer[x] + 1) < *((unsigned int*) bestPtr + 1)) {
                bestPtr = &myBuffer[x];
            }
        }
        if (myBuffer[x + sizeBook + *((unsigned int*) &myBuffer[x] + 1) - sizeof(char)] == '0') break;
    }
    return (void*) bestPtr;
}

void myAlloc(size_t size)
{
    void* block;
    if (strcmp(allocAgorithm, "worstFit") == 0) {
        block = worstFit(size);
    } else if (strcmp(allocAgorithm, "bestFit") == 0) {
        block = bestFit(size);
    } else if (strcmp(allocAgorithm, "nextFit") == 0) {
        block = nextFit(size);
    } else if (strcmp(allocAgorithm, "firstFit") == 0) {
        block = firstFit(size);
    } else if (strcmp(allocAgorithm, "randomFit") == 0) {
        block = randomFit(size);
    }

    if (block != NULL) {
        *((unsigned int*) block) += size;
    }
}

void setMyAlloc(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File: %s can't be accessed\n", filename);
        exit(EXIT_FAILURE);
    }

    unsigned int size;
    while (!feof(file)) {
        fscanf(file, "%u\n", &size);
        myAlloc(size);
    }
    fclose(file);
}

int main(int argc, char** argv)
{
    if (argc != 6) {
        printf("Missing parameters needed\n");
        return 1;
    }
    int x;
    char* sizes;
    for (x = 1; x < 5; x+=2) {
        if (strcmp(argv[x], "-c") == 0) {
            myAllocInit(argv[x+1]);
        } else if (strcmp(argv[x], "-s") == 0) {
            sizes = argv[x+1];
        }
    }

    allocAgorithm = argv[5];

    setMyAlloc(sizes);

    printBufferStats(1);

    return 0;
}
