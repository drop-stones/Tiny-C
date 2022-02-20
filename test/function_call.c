// RUN: %tiny-c %s
// RUN: cat %S/function_call.ll | FileCheck %s

// CHECK: define i32 @add(i32 %0, i32 %1)
int add(int a, int b) {
// CHECK: entry:
// CHECK:   %2 = alloca i32
// CHECK:   store i32 %0, i32* %2
// CHECK:   %3 = alloca i32
// CHECK:   store i32 %1, i32* %3

// CHECK:   %4 = load i32, i32* %2
// CHECK:   %5 = load i32, i32* %3
// CHECK:   %6 = add nsw i32 %4, %5
// CHECK:   ret i32 %6
  return a + b;
}

// CHECK: define i32 @main()
int main() {
// CHECK: entry:
// CHECK:   %0 = alloca i32
// CHECK:   %1 = alloca i32
  int x;
  int y;
// CHECK:   store i32 100, i32* %0
  x = 100;
// CHECK:   store i32 200, i32* %1
  y = 200;
// CHECK:   %2 = load i32, i32* %0
// CHECK:   %3 = load i32, i32* %1
// CHECK:   %4 = call i32 @add(i32 %2, i32 %3)
// CHECK:   ret i32 %4
  return add(x, y);
}