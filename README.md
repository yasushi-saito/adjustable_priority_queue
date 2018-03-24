# intrusive_priority_queue

An efficient intrusive priority queue.

- Based on binary heaps
- Object lifetime is managed explicitly by the caller
- Supports in-place rank adjustment

# Installation

You need a C++-11 compiler. I tested using gcc-4.8/libstdc++ and clang-4.0/libc++.
To run the test and benchmark, you need to install google sparsehash. For ubuntu, do:

    sudo apt-get install g++ libsparsehash-dev

Follow the following descriptions to install gtest:

    https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/

To build the test:

    cmake -DCMAKE_BUILD_TYPE=Debug . # or cmake -DCMAKE_BUILD_TYPE=Release .
    make -j8
