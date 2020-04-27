# assignment-1
Code for Assignment 1: An Arithmetic/Logic Unit (ALU)
DUE DATE: Tue, Feb 4 by 11:59:59pm
An Arithmetic/Logic Unit (ALU)
In lecture 1, we learned that the Central Processing Unit (CPU) of a computer includes a set of high-speed registers, a Control Unit (CU) that executes instructions stored in memory, and an Arithmetic/Logic unit (ALU) for performing arithmetic and logical operations specified by instructions. The instruction fetches a value from memory into a register, and uses the ALU to perform an operation.

In the same lecture, we also looked at the format for various types of data, including an integer value. An integer consists of sequential bytes that are treated by the ALU as a logical word for the purpose of performing operations on the data. Integers typically contain 16, 32, or 64 bits in 2 to 8 byes, with the most-significant bit as the sign bit.

About the assignment
This assignment will give you an opportunity to explore how the arithmetic and logical operations on bits of a word are actually implemented by the ALU. The 2020SPCS5600SV/assignment-1 repository contains data definitions in "word.h" and funcitons in "word.c" that implement a word as an array of unsigned bytes, and provide functions for accessing individual bits. The definitions are for a 32-bit word with 4 bytes, but the values could be adjusted to support any number of bytes per word. The memory is configured for big-endian order, but can be switched to support little-endian order. You will not be changing these settings for this assignment.

The files "alu.h" and "alu.c" implement the functions performed by the ALU. These include arithmetic operations (add, sub, mul, div, negative) logical operations (and, or, xor, not), shift operations (logical, circular, arithmetic, mask), and conditional tests (eq, ge, lt). They operate on the bits of their word parameters, and return a result word. The ALU operations use 2's complement arithmetic.

Implementation details
Some of the functions bodies have been left for you to implement, using the word functions and the examples of how other functions in "alu.c" are implemented. Each of the unimplemented functions have a comment "your code here" and many of them provide notes about how to implement the functions. For example, the addWord() and mulWord() functions provided detailed notes on binary addition and multiplication. You may only access the bits of the input and output word values through the word functions, and you may only operate on one bit at a time or call other functions in "alu.c" (e.g. negativeWord() to change the sign of the word.

A file "assignment_1_test.c" contains a main program that uses the CUnit test framework and includes test functions for the word, math, comparison, shift, and logical functions in "word.c" and "alu.c". The files and the unit tests are set up so that you can compile all the code and run the unit tests, altough all of the ALU tests for the functions you must implement will fail. The memory unit test should work with no changes required to the memory functions.

A recommendation is to comment out the registration of all but one of the ALU test functions in test_all() and get that function working before moving on to the next one. It may be easiest to get the compare and logical functions working first, then move on to the shift functions, and finally to the arithmetic functions. You will find it very helpful to use your debugger to identify and diagnose problems in your code, using the data view to view data. A word is an array of 4 bytes so you may need to view the word as an array.

Project Staffing
You are encouraged to form a team of 2 to work on this project. It would be very helpful to ensure that at least one of the team has non-trivial experience with C and using C development tools. You may also work on this project on your own, but you may have a better experience working as a team. There will be other assignments that also allow teams so this is a good opportunity to try it out.

Turning in the assignment
Turn your assignment in by uploading all the .c and .h files to your 2020SPCS5600SV/assignment-1-ccis_id repository. Do not turn in other files that may have been created by your IDE. The graders will download and run your code, so be sure that all the files are avaiable and no extraneous files have been checked in. Once you have checked in your code, it is a good idea to verify that what you checked in is actually in the repository to avoid delays in grading your assignment.

You are welcome to check in intermediate versions during development as a backup to what is on your computer. Only the final version will be graded.

Important: If you are working as part of a team, both of you should check in your work in your respective repositories. You both must also edit your "README.md" files to list the names and the CCIS-IDs of both of you.

Additional information
Here is a link to a video that provides a short walk-through of the starting code:
https://bluejeans.com/s/WGOch
