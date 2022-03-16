#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MY_BUFFER_SIZE 1024
#define max(a, b) ((a)>(b)?(a):(b))
#include <time.h>

struct timespec start, stop;
char myBuffer[MY_BUFFER_SIZE];
char* lastPos = myBuffer;
char* allocAgorithm;

/*
 *  ========= BLOCK STRUCT =========
 *  (unsigned int): allocated bytes | *((unsigned int*) &myBuffer)
 *  (unsigned int): chunk size      | *((unsigned int*) &myBuffer + 1)
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
 *  (char*): chunk conentents       |
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
 *  (char): has next block          | &myBuffer + *((unsigned int*) &myBuffer + 1) + 2*sizeof(unigned int)
 */

const int sizeBook = 2* sizeof(unsigned int) + sizeof(char);
struct info{
    int chunks;
    int totalFailed;
    int totalFailedCnt;
}info;

/*
 * === PRINT MODES === 
 * 0: print each block 
 * 1: print only total stats
 * 2: print all stats
 */
void printBufferStats(int mode)
{
    printf("Information about %s\n", allocAgorithm);
    unsigned int x;
    double meanInnerFrag;
    double meanOutterFrag;
    double setBlocks = 0.0, setTotalMemory = 0.0, totalFreeMem = 0.0, maximalFreeMemoryBlock = 0.0, totalAverage = 0.0, totalAvgWithBook = 0.0;

    for (x = 0; x < MY_BUFFER_SIZE; x += sizeBook + *((unsigned int*) &myBuffer[x] + 1)) {
        double size = (double)*((unsigned int*) &myBuffer[x]);
        double chunk = (double)*((unsigned int*) &myBuffer[x] + 1);
        double blockSize = sizeBook + chunk;
        maximalFreeMemoryBlock = max(maximalFreeMemoryBlock, chunk - size);
        setTotalMemory += size;
        totalAverage += size/chunk;
        totalAvgWithBook += size/blockSize;
        totalFreeMem += chunk - size;
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
    if (mode == 1) {
        printf("Total set memory:%0.3f, Total free memory:%0.3f \n", setTotalMemory, totalFreeMem);
        printf("Outter fragmentation: %0.3f%%\n", (1 - (maximalFreeMemoryBlock/totalFreeMem)) *100);
        printf("Inner average fragmentation: %0.3f, with bookkeeping: %0.3f\n", (1 - (totalAverage/info.chunks)) * 100, (1 - (totalAvgWithBook/info.chunks)) * 100);
        printf("Failed to allocate count: %i, total memory unallocated: %i\n\n", info.totalFailedCnt, info.totalFailed);
    }
}

/*
 *  Initilizes chunks in myBuffer from file 
 */
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
            info.totalFailed += chunk;
            info.totalFailedCnt++;
            continue;
        }
        *((unsigned int*) &myBuffer[ptr]) = 0;
        ptr+= sizeof(unsigned int);
        *((unsigned int*) &myBuffer[ptr]) = chunk;
        ptr += sizeof(unsigned int);
        ptr += chunk;
        myBuffer[ptr] = '1';
        ptr += sizeof(char);
        info.chunks++;
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
        if (size + *((unsigned int *) &myBuffer[x]) <= *((unsigned int *) &myBuffer[x] + 1)) {
            tmp = &myBuffer[x];
            break;
        }
        if (cnt >= info.chunks) {
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

void* firstFit(size_t size)
{
    unsigned int x;
    for (x = 0; x < MY_BUFFER_SIZE; x += sizeBook + *((unsigned int*) &myBuffer[x] + 1)) {
        if (size + *((unsigned int*) &myBuffer[x]) <= *((unsigned int*) &myBuffer[x] + 1)) {
            return &myBuffer[x];
        }
        if (myBuffer[x + sizeBook + *((unsigned int*) &myBuffer[x] + 1) - sizeof(char)] == '0') break;
    }
    return NULL;
}

int myRandom(number) {
    int i;
    i = (rand() % (number - 1)) + 1;
    return i;
}

void* randomFit(size_t size) {
    int random_result = myRandom(info.chunks) - 1;
    unsigned int x, n = 0;
    for (x = 0; x < MY_BUFFER_SIZE; x += sizeBook + *((unsigned int*)&myBuffer[x] + 1)) {
        if(n != random_result) {
            n++;
            continue;
        } else {
            if (size + *((unsigned int*)&myBuffer[x]) <= *((unsigned int*)&myBuffer[x] + 1)) {
                return &myBuffer[x];
            }
        }
    }
    return NULL;
}

/*
 *  malloc() interpretation, based on selected alogrithm
 */
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

/*
 * Calls myAlloc for each requested memory allocation request in file
 */
void setMyAlloc(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File: %s can't be accessed\n", filename);
        exit(EXIT_FAILURE);
    }

    unsigned int size;
    clock_gettime(CLOCK_REALTIME, &start);
    while (!feof(file)) {
        fscanf(file, "%u\n", &size);
        myAlloc(size);
    }
    clock_gettime(CLOCK_REALTIME, &stop);
    double result = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e6;    // in milliseconds
    printf("\n%s function elapsed time: %0.3f milliseconds\n", allocAgorithm, result);
    fclose(file);
}

/*
 *  Takes 3 arguments:
 *     * -c chunks file
 *     * -s sizes files
 *     * allocation algorithm
 */
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
