primef = func(n) {
    if (n % 2 == 0) 
        return 2;
    i = 3;
    while (n % i != 0 && i * i <= n)
        i = i + 2;
    if (i * i <= n) 
        return i;
    return n;
}

N = ?;

print primef(N);