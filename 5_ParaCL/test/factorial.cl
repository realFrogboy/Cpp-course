fact = func(x) : factorial {
  res = 1;
  boo = func(x) : printf { print x; }
  if (x > 0)
    res = x * factorial(x - 1);

  boo(res);
  res;
}

a = fact(1);
printf(a);
b = factorial(6);
printf(b);