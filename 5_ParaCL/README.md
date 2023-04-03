# ParaCL
## Frontend
Frontend and interpreter of ParaCL language. Program parses source code, builds ast and then just bypasses ast executing commands one by one.<br />
For now interpreter supports:
1. Basic arithmetic
2. Control flows (if/else, while)
3. Functions

## Backend
LLVM IR code generation from source code. Program parses source code, builds ast and then bypasses ast and generate IR. After this program creates object file `output.o` and compilers it to executable file `prog`.<br />
For now compiler supports:
1. Basic arithmetic
2. Control flows (if/else, while)

## Build
First of all, you should clone git repository
```sh
$ git clone "git@github.com:realFrogboy/Cpp-course.git"
$ cd Cpp-course/5_ParaCL
```
Create build directory
```sh
$ mkdir build
$ cd build
```
Build
```sh
$ cmake -D CMAKE_BUILD_TYPE=Release ../
$ cmake --build ./
```
There are 2 executables: <br />
`paracl`  - compiler. Creates `output.o` and `prog` <br />
`paracli` - interpreter.
## AST
If you want to see on generated AST, you should build with DUMP flag:
```sh
$ cmake -D DUMP=true -D CMAKE_BUILD_TYPE=Release ../
$ cmake --build ./
``` 

## Examples
### n-th Fibonacci number
fib.cl:

        scan(n);
        f1 = 0;
        f2 = 1;
        c = 2;

        while (c < n) {
            fn = f1 + f2;
            f1 = f2;
            f2 = fn;
            c = c + 1;
        }

        if (n < 1)
            print(-1);
        else if (n == 1)
            print(0);
        else
            print(f2);


### The number of combinations C(n,k) (recursion)
c_n_k.cl:

        combinations = func(n, k) : C {
            if (k == n) return 1;
            if (k == 1) return n;
            res = C(n - 1, k - 1) + C(n - 1, k);
        }

        n = ?;
        k = ?;
        print C(n, k);
