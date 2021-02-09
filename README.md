# Weighted-Buddy-Allocator
Simulation of the standard malloc and free methods in C using a character array. The following [paper](https://dl.acm.org/doi/pdf/10.1145/355620.361164) was implemented as part of the Course project.

Three types of implementation were tried-

1. One Bin
2. SP Heap(Weighted buddy)
3. Standard Malloc

The simulations were carried out generating requests from a uniform and exponential distributions.

# Results

1. Standard Malloc was performing the best in terms of speed of memory allocation and deallocation.
2. The fragmentation and defragmentation rates were relatively lower for the weighted buddy memory allocator.
