# CS5600 assignment-3
Starting code for assignment 3: enhanced K&amp;R memory allocator
DUE DATE: Fri, March 13, 2020 by 11:59:59pm
A Dynamic Memory Manager
In class we examined a simple dynamic memory manager based on the original design by Brian Kernighan and Dennis Ritchie. The design used a singly-linked circular list of free storage blocks with a header just before the payload area that contains a pointer to the next block and the total size of the block in multiples of the header size. The list is maintained in order of ascending address to facilitate coalescing adjacent free blocks to minimize exernal fragmentation.

The advantage of this design is its simplicity, but we also noted some problems with the design and looked at an improved design that eliminates several of these problems. In this assignment, you will implement these improvements to the original K&R implementation.

The K&R implementation provided with this assignment in the file "mm_heap_kr.c" has been updated to a modern dialect of C, and modified to use mmlib, a framework for testing new dynamic memory manager implementations.

The mmlib framework provides surrogate memory management function prototypes mm_malloc(), mm_realloc() and mm_free() to implement. It also provides a mem_sbrk() function that is a surrogate for the sbrk() system function for extending the heap. It allocates a pool of memory using malloc() that mem_sbrk() uses as its heap space. This allows isolating new dynamic memory management functions from the actual ones provided by C. It also allows resetting the surrogate heap to run multiple tests without restarting the program.
A test program "test_heap.c" exercises the functions, determines that they are functionaing properly, and measures their performance. The test program takes one or more data (".rep") files that it uses to exercise the memory allocation functions. These data files contain commands to malloc, realloc, and free blocks of memory. For each file, "test_heap.c" resets the heap back to its initial condition, runs the commands, and reports on the results.

At the end, the program shows a sumary report for all the tests. The program has several flags including "-v" to show progress for each test, and "-d" to show a detailed log of each operation performed.

You will be creating a new allocator with the improvements described in the lecture 6 slides. Add safeguards to this version to ensure that it is not possible to free storage that was already freed or storage that is not from the heap.

Create a new "mm_heap_kr2.c" for these changes. Do not modify the original "mm_heap_kr.c" file. You can exclude the other "mm_heap_*.c" files in your project to test this version.

Starting Code
Begin with the source code we studied in Lecture 6, which is located in the 2020SPCS5600SV/assignment-3 repository. The repository contains source code and a directory with the test files.
