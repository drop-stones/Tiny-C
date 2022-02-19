int add(int a, int b) {
  return a + b;
}

int main() {
  int x;
  int y;
  x = 100;
  y = 200;
  x = add(x, y);
  if (x == 300) {
    y = 0;
  } else {
    y = 1;
  }
  return y;
}