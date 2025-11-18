#include "memory_manager.h"

int main() {
    MemPool *pool = mem_pool_init("NetworkControlPool", 1024 * 1024); // 1MB

    if (!pool) {
        fprintf(stderr, "Failed to initialize memory pool.\n");
        return 1;
    }

    // ????? ??? ????
    void *ptr1 = mem_pool_alloc(pool, 256);
    void *ptr2 = mem_pool_alloc(pool, 512);
    void *ptr3 = mem_pool_alloc(pool, 1024);

    mem_pool_print_status(pool);

    // ????????
    mem_pool_free(pool, ptr2);

    mem_pool_print_status(pool);

    // ??????????
    mem_pool_destroy(pool);

    return 0;
}