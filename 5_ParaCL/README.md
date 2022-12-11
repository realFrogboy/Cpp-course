# ParaCL
Frontend and simulator of the ParaCL language.

## AST
If you want to seen on generated AST, you should build with DUMP flag:

        cmake --build ./ -DDUMP=true
    

## Examples
### n-th Fibonacci number
fib:

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

        if (n < 1) {
            print(-1);
        } else { if (n == 1) {
            print(0);
        } else {
            print(f2);
        } }


AST:

![Image text](https://github.com/realFrogboy/Pictures/blob/main/fib.png)