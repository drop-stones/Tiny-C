int main() {
  int cond;
  int num;
  cond = 1;
  num = 0;
  if (cond == 1) {
    num = 1;
  } else {
    num = 2;
  }
  while (num <= 100) {
    num = num + 1;
  }
  return num;
}