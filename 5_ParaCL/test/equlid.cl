gcd = func(a, b) {

while (a != 0 && b != 0)
{
    if (a > b)
       a = a % b;
    else
        b = b % a;
}
a + b;
}

lcm = func(a, b) {
    a / gcd(a, b) * b;
}

a = ?;
b = ?;

print gcd(a, b);
print lcm(a, b);
