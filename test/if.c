// RUN: %tiny-c %s
// RUN: cat %S/if.ll | FileCheck %s

// CHECK: define i32 @isOneHundred(i32 %0) {
int isOneHundred(int a) {
// CHECK:       entry:
// CHECK-NEXT:    %1 = alloca i32
// CHECK-NEXT:    store i32 %0, i32* %1
// CHECK:         %2 = alloca i32
  int ret;
// CHECK:         store i32 0, i32* %2
  ret = 0;
// CHECK:         %3 = load i32, i32* %1
// CHECK:         %4 = icmp eq i32 %3, 100
// CHECK:         br i1 %4, label %if.body, label %else.body
  if (a == 100) {
// CHECK:       if.body:
// CHECK:         store i32 1, i32* %2
    ret = 1;
// CHECK:         br label %after.if
  } else {
// CHECK:       else.body:
// CHECK:         store i32 0, i32* %2
    ret = 0;
// CHECK:         br label %after.if
  }
// CHECK:       after.if:
// CHECK-NEXT:    %5 = load i32, i32* %2
// CHECK-NEXT:    ret i32 %5
  return ret;
}