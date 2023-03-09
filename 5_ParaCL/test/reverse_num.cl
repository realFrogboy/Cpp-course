revers = func(num) {
    copy = num;
    order = -1;

    while (copy > 0) {
        ++order;
        copy = copy / 10;
    }

    res = 0;
    while (num > 0) {
        dig = num % 10;
        res = res + dig * 10 ** order;
        num = num / 10;
        --order;
    }
    res;
}

a = ?;
print revers(a);