// RUN: %tiny-c %s
// RUN: cat %S/binary_expression.ll | FileCheck %s

// CHECK: define i32 @calc(i32 %0, i32 %1, i32 %2, i32 %3)
int calc(int a, int b, int c, int d) {
// CHECK:     entry:
// CHECK-DAG:   %4 = alloca i32
// CHECK-DAG:   %5 = alloca i32
// CHECK-DAG:   %6 = alloca i32
// CHECK-DAG:   %7 = alloca i32
// CHECK-DAG:   store i32 %0, i32* %4
// CHECK-DAG:   store i32 %1, i32* %5
// CHECK-DAG:   store i32 %2, i32* %6
// CHECK-DAG:   store i32 %3, i32* %7

// CHECK:       %8 = load i32, i32* %4
// CHECK:       %9 = load i32, i32* %5
// CHECK:       %10 = load i32, i32* %6
// CHECK:       %11 = mul nsw i32 %9, %10
// CHECK:       %12 = add nsw i32 %8, %11

// CHECK:       %13 = load i32, i32* %7
// CHECK:       %14 = sdiv i32 %12, %13
// CHECK:       ret i32 %14
  return (a + b * c) / d;
}