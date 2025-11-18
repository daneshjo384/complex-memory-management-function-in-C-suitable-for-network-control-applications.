# complex-memory-management-function-in-C-suitable-for-network-control-applications.
complex memory management function in C suitable for network control applications.
-------------
Features of the function:
mem_pool_init: initialization of a memory pool mem_pool_alloc: safe allocation from the pool mem_pool_free: safe release to the pool mem_pool_destroy: complete cleanup of the pool thread-safe management (usability in multithreading) error and status logging
--------
gcc -o mem_test main.c memory_manager.c -lpthread
----------
sample output

[MemPool] Pool 'NetworkControlPool' initialized with 1048576 bytes
[MemPool] Allocated 256 bytes at 0x55f0b8001260 in block 0
[MemPool] Allocated 512 bytes at 0x55f0b8001370 in block 1
[MemPool] Allocated 1024 bytes at 0x55f0b8001590 in block 2

[MemPool Status: NetworkControlPool]
Total Size: 1048576 bytes
Used Size: 1792 bytes
Active Blocks: 3

[MemPool] Freed block at 0x55f0b8001370
[MemPool Status: NetworkControlPool]
Total Size: 1048576 bytes
Used Size: 1280 bytes
Active Blocks: 2
-------
[MemPool] Pool destroyed.
Features of this system:
Thread-Safe with pthread_mutex Track allocated blocks Avoid memory leaks Ability to display status Usability in network control applications (e.g. message processing servers, session managers, etc.)
