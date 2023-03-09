Translate = func(num, base) : translate {
    if (!num) return 0;

    translate(num / base, base);
    print num % base;
}

n = ?;
base = ?;
translate(n, base);