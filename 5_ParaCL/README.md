# ParaCL
Frontend and interpreter of ParaCL language. Program parses source code, builds ast and then just bypasses ast.<br />
For now it supports:
1. Basic arithmetic
2. Control flow (if/else, while)
3. Functions

## AST
If you want to see on generated AST, you should build with DUMP flag:

        cmake --configure . -DDUMP=true
    

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
