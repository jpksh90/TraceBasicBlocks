; ModuleID = 'foo_instrumented.bc'
source_filename = "foo.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx12.0.0"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [14 x i8] c"n is negative\00", align 1
@__counter = internal global i64 0
@0 = private unnamed_addr constant [26 x i8] c"\0A\0A[___BB_EXECUTED___ %d]\0A\00", align 1
@1 = private unnamed_addr constant [25 x i8] c"[___NB_STATIC_BB___ %d]\0A\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @fib(i32 %0) #0 {
  %2 = load i64, i64* @__counter, align 8
  %3 = add i64 %2, 1
  store i64 %3, i64* @__counter, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 %0, i32* %4, align 4
  store i32 0, i32* %5, align 4
  store i32 1, i32* %6, align 4
  store i32 1, i32* %8, align 4
  br label %9

9:                                                ; preds = %23, %1
  %10 = load i64, i64* @__counter, align 8
  %11 = add i64 %10, 1
  store i64 %11, i64* @__counter, align 8
  %12 = load i32, i32* %8, align 4
  %13 = load i32, i32* %4, align 4
  %14 = icmp sle i32 %12, %13
  br i1 %14, label %15, label %28

15:                                               ; preds = %9
  %16 = load i64, i64* @__counter, align 8
  %17 = add i64 %16, 1
  store i64 %17, i64* @__counter, align 8
  %18 = load i32, i32* %5, align 4
  %19 = load i32, i32* %6, align 4
  %20 = add nsw i32 %18, %19
  store i32 %20, i32* %7, align 4
  %21 = load i32, i32* %6, align 4
  store i32 %21, i32* %5, align 4
  %22 = load i32, i32* %7, align 4
  store i32 %22, i32* %6, align 4
  br label %23

23:                                               ; preds = %15
  %24 = load i64, i64* @__counter, align 8
  %25 = add i64 %24, 1
  store i64 %25, i64* @__counter, align 8
  %26 = load i32, i32* %8, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, i32* %8, align 4
  br label %9, !llvm.loop !10

28:                                               ; preds = %9
  %29 = load i64, i64* @__counter, align 8
  %30 = add i64 %29, 1
  store i64 %30, i64* @__counter, align 8
  %31 = load i32, i32* %7, align 4
  ret i32 %31
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = load i64, i64* @__counter, align 8
  %2 = add i64 %1, 1
  store i64 %2, i64* @__counter, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  %5 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32* %4)
  %6 = load i32, i32* %4, align 4
  %7 = icmp slt i32 %6, 0
  br i1 %7, label %8, label %12

8:                                                ; preds = %0
  %9 = load i64, i64* @__counter, align 8
  %10 = add i64 %9, 1
  store i64 %10, i64* @__counter, align 8
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i64 0, i64 0))
  store i32 -1, i32* %3, align 4
  br label %18

12:                                               ; preds = %0
  %13 = load i64, i64* @__counter, align 8
  %14 = add i64 %13, 1
  store i64 %14, i64* @__counter, align 8
  %15 = load i32, i32* %4, align 4
  %16 = call i32 @fib(i32 %15)
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %16)
  store i32 0, i32* %3, align 4
  br label %18

18:                                               ; preds = %12, %8
  %19 = load i64, i64* @__counter, align 8
  %20 = add i64 %19, 1
  store i64 %20, i64* @__counter, align 8
  %21 = load i32, i32* %3, align 4
  %22 = load i64, i64* @__counter, align 8
  %call-block = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @0, i32 0, i32 0), i64 %22)
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @1, i32 0, i32 0), i32 9)
  ret i32 %21
}

declare i32 @scanf(i8*, ...) #1

declare i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone ssp uwtable "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.5a,+zcm,+zcz" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.5a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 12, i32 3]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 1, !"branch-target-enforcement", i32 0}
!3 = !{i32 1, !"sign-return-address", i32 0}
!4 = !{i32 1, !"sign-return-address-all", i32 0}
!5 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!6 = !{i32 7, !"PIC Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 1}
!8 = !{i32 7, !"frame-pointer", i32 1}
!9 = !{!"Apple clang version 13.1.6 (clang-1316.0.21.2.5)"}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
