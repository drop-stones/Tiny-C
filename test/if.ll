; ModuleID = '/home/shizuku/Learning/Tiny-C/test/if.c'
source_filename = "/home/shizuku/Learning/Tiny-C/test/if.c"

define i32 @isOneHundred(i32 %0) {
entry:
  %1 = alloca i32, align 4
  store i32 %0, i32* %1, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %2, align 4
  %3 = load i32, i32* %1, align 4
  %4 = icmp eq i32 %3, 100
  br i1 %4, label %if.body, label %else.body

if.body:                                          ; preds = %entry
  store i32 1, i32* %2, align 4
  br label %after.if

else.body:                                        ; preds = %entry
  store i32 0, i32* %2, align 4
  br label %after.if

after.if:                                         ; preds = %else.body, %if.body
  %5 = load i32, i32* %2, align 4
  ret i32 %5
}
