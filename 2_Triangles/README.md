# Triangles
Project for the 2nd task of the C++ course.

## Task
Print the numbers of all triangles that intersect with any other once.

## Algorithm
I have implemented the algorithm proposed in the book "Geometric Tools for Computer Graphics" - Eberly, Schneider:
![Image text](https://github.com/realFrogboy/Pictures/blob/main/triangles.png)

## Visualization
I have implemented visualization of triangles on vulkan.
You can run program with **-h** flag, to get information about keyboard movement.
![Image text](https://github.com/realFrogboy/Pictures/blob/main/10001_visualization.png)

## Instalation
Run:

        git clone "git@github.com:realFrogboy/Cpp-course.git"

## Build

        cd Cpp_course/2_Triangles
        mkdir build
        cd build
        cmake ../
        cmake --build ./

## Tests

        ./Test

Generate 10000 triangles and run program.

        ./generate <Number>

Generate file "Generation.txt" with *Number* triangles.