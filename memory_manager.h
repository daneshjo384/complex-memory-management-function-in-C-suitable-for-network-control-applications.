#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// ?????? ???? ?????? ???? ?????
typedef struct MemBlock {
    void *ptr;
    size_t size;
    int in_use;
    struct MemBlock *next;
} MemBlock;

// ?????? ???? pool
typedef struct MemPool {
    char name[64];
    size_t total_size;
    size_t used_size;
    size_t block_count;
    MemBlock *blocks;
    pthread_mutex_t lock;
} MemPool;

// ???????? ?????? ?????
MemPool* mem_pool_init(const char *name, size_t total_size);
void* mem_pool_alloc(MemPool *pool, size_t size);
void mem_pool_free(MemPool *pool, void *ptr);
void mem_pool_destroy(MemPool *pool);
void mem_pool_print_status(MemPool *pool);

#endif