global = ?;

foo = func(x) {
  y = 10;
  if (global > 0)
    return y;
  x + y;
};

x = 10;

bar = {
  y = 10;
  print foo(11);
  if (global > 0)
    return y;
  x + y;
};

print foo(10);
print bar;