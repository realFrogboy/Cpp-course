# OpenCL
Some applications in OpenCL.

## Bitonic sort
I have implemented the algorithm proposed in the book ["OpenCL in Action"](https://www.amazon.com/OpenCL-Action-Accelerate-Graphics-Computations/dp/1617290173) - Matthew Scarpino.

## Instalation
Run:

        git clone "git@github.com:realFrogboy/Cpp-course.git"

## Build
        cd Cpp_course/6_OpenCL
        mkdir build
        cd build
        cmake ../
        cmake -D CMAKE_BUILD_TYPE=Release ./
        cmake --build ./

## Test
        ./bitonic_sort_test
        
Program generates different sequences of floats, sort them and check answers. Also it compares time spent by bitonic sort and time spent by std::sort.

        ./bitonic_sort_test <number>

Program generates a sequence of *number* floats and then it executes similarly "./bitonic_sort_test".

        ./gtest

Program runs gtests with handmade sequences and corner cases.