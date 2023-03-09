combinations = func(n, k) : C {
    if (k == n) return 1;
    if (k == 1) return n;
    res = C(n - 1, k - 1) + C(n - 1, k);
}

n = ?;
k = ?;
print C(n, k);