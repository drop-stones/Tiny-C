// RUN: %tiny-c %s
// RUN: cat %S/while.ll | FileCheck %s

// CHECK: define i32 @OneHundred()
int OneHundred() {
// CHECK:     entry:
// CHECK-NEXT:  %0 = alloca i32
  int x;
// CHECK:       store i32 0, i32* %0
  x = 0;
// CHECK:     while.cond:
// CHECK-NEXT:  %1 = load i32, i32* %0
// CHECK-NEXT:  %2 = icmp ne i32 %1, 100
// CHECK-NEXT:  br i1 %2, label %while.body, label %after.while
  while (x != 100) {
// CHECK:     while.body
// CHECK:       %3 = load i32, i32* %0
// CHECK:       %4 = add nsw i32 %3, 1
// CHECK:       store i32 %4, i32* %0
    x = x + 1;
// CHECK:       br label %while.cond
  }
// CHECK:      after.while:
// CHECK:       %5 = load i32, i32* %0
// CHECK:       ret i32 %5
  return x;
}