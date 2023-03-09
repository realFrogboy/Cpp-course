Hanoi = func(rings, a, b, c) : hanoi{
    if (rings > 0) {
        hanoi(rings - 1, a, c, b);
        print a; print c;
        print 100000;
        hanoi(rings - 1, b, a, c);
    }
}

rings_n = ?;
hanoi(rings_n, 1, 2, 3);