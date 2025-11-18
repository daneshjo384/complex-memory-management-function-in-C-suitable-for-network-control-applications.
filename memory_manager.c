#include "memory_manager.h"

// ???? ????? ?? pool ?????
MemPool* mem_pool_init(const char *name, size_t total_size) {
    MemPool *pool = (MemPool*)calloc(1, sizeof(MemPool));
    if (!pool) {
        perror("Failed to allocate MemPool");
        return NULL;
    }

    strncpy(pool->name, name, sizeof(pool->name) - 1);
    pool->total_size = total_size;
    pool->used_size = 0;
    pool->block_count = 0;

    // ????? ????? ????? ???? ???? ???????
    pool->blocks = (MemBlock*)calloc(1000, sizeof(MemBlock)); // ?????? 1000 ????
    if (!pool->blocks) {
        free(pool);
        perror("Failed to allocate block list");
        return NULL;
    }

    pthread_mutex_init(&pool->lock, NULL);

    printf("[MemPool] Pool '%s' initialized with %zu bytes\n", name, total_size);
    return pool;
}

// ???? ????? ?? pool
void* mem_pool_alloc(MemPool *pool, size_t size) {
    if (!pool || size == 0) return NULL;

    pthread_mutex_lock(&pool->lock);

    // ????? ???? ?? ???? ????
    for (size_t i = 0; i < 1000; i++) {
        if (!pool->blocks[i].in_use) {
            void *ptr = malloc(size);
            if (!ptr) {
                pthread_mutex_unlock(&pool->lock);
                return NULL;
            }

            pool->blocks[i].ptr = ptr;
            pool->blocks[i].size = size;
            pool->blocks[i].in_use = 1;
            pool->used_size += size;
            pool->block_count++;

            pthread_mutex_unlock(&pool->lock);
            printf("[MemPool] Allocated %zu bytes at %p in block %zu\n", size, ptr, i);
            return ptr;
        }
    }

    pthread_mutex_unlock(&pool->lock);
    fprintf(stderr, "[MemPool] No free block available in pool '%s'\n", pool->name);
    return NULL;
}

// ???? ???????? ?? pool
void mem_pool_free(MemPool *pool, void *ptr) {
    if (!pool || !ptr) return;

    pthread_mutex_lock(&pool->lock);

    for (size_t i = 0; i < 1000; i++) {
        if (pool->blocks[i].ptr == ptr && pool->blocks[i].in_use) {
            free(pool->blocks[i].ptr);
            pool->blocks[i].ptr = NULL;
            pool->blocks[i].size = 0;
            pool->blocks[i].in_use = 0;
            pool->used_size -= pool->blocks[i].size;
            pool->block_count--;

            pthread_mutex_unlock(&pool->lock);
            printf("[MemPool] Freed block at %p\n", ptr);
            return;
        }
    }

    pthread_mutex_unlock(&pool->lock);
    fprintf(stderr, "[MemPool] Attempted to free unknown pointer %p\n", ptr);
}

// ???? ????? ????? pool
void mem_pool_print_status(MemPool *pool) {
    if (!pool) return;

    pthread_mutex_lock(&pool->lock);
    printf("\n[MemPool Status: %s]\n", pool->name);
    printf("Total Size: %zu bytes\n", pool->total_size);
    printf("Used Size: %zu bytes\n", pool->used_size);
    printf("Active Blocks: %zu\n", pool->block_count);
    pthread_mutex_unlock(&pool->lock);
}

// ???? ?????????? pool
void mem_pool_destroy(MemPool *pool) {
    if (!pool) return;

    pthread_mutex_lock(&pool->lock);

    for (size_t i = 0; i < 1000; i++) {
        if (pool->blocks[i].in_use) {
            free(pool->blocks[i].ptr);
            pool->blocks[i].ptr = NULL;
            pool->blocks[i].in_use = 0;
        }
    }

    free(pool->blocks);
    pthread_mutex_unlock(&pool->lock);
    pthread_mutex_destroy(&pool->lock);
    free(pool);

    printf("[MemPool] Pool destroyed.\n");
}