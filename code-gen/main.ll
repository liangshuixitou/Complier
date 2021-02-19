; ModuleID = 'main'

@0 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@1 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@2 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@3 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@4 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@5 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@6 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@7 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@8 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@9 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@10 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@11 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@12 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@13 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"
@14 = private unnamed_addr constant [12 x i8] c"value = %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = alloca i32
  %1 = alloca i32
  store i32 0, i32* %1
  %2 = alloca i32
  store i32 0, i32* %2
  store i32 5, i32* %0
  %3 = load i32, i32* %0
  %4 = load i32, i32* %1
  %addtmp = add i32 %3, %4
  store i32 %addtmp, i32* %1
  %5 = load i32, i32* %0
  %6 = load i32, i32* %1
  %addtmp1 = add i32 %5, %6
  %7 = load i32, i32* %2
  %addtmp2 = add i32 %addtmp1, %7
  store i32 %addtmp2, i32* %2
  %8 = load i32, i32* %0
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0), i32 %8)
  %10 = load i32, i32* %1
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @1, i32 0, i32 0), i32 %10)
  %12 = load i32, i32* %2
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i32 %12)
  %14 = load i32, i32* %0
  %15 = load i32, i32* %0
  %cmptmp = icmp eq i32 %14, %15
  %16 = icmp ne i1 %cmptmp, false
  br i1 %16, label %then, label %ifcont9

then:                                             ; preds = %entry
  %17 = alloca i32
  %18 = alloca i32
  %19 = alloca i32
  store i32 10, i32* %17
  %20 = load i32, i32* %17
  %21 = load i32, i32* %18
  %addtmp3 = add i32 %20, %21
  store i32 %addtmp3, i32* %18
  %22 = load i32, i32* %17
  %23 = load i32, i32* %18
  %addtmp4 = add i32 %22, %23
  %24 = load i32, i32* %19
  %addtmp5 = add i32 %addtmp4, %24
  store i32 %addtmp5, i32* %19
  %25 = load i32, i32* %17
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @3, i32 0, i32 0), i32 %25)
  %27 = load i32, i32* %18
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @4, i32 0, i32 0), i32 %27)
  %29 = load i32, i32* %19
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @5, i32 0, i32 0), i32 %29)
  %31 = load i32, i32* %18
  %32 = load i32, i32* %19
  %cmptmp6 = icmp ult i32 %31, %32
  %33 = icmp ne i1 %cmptmp6, false
  br i1 %33, label %then7, label %ifcont

then7:                                            ; preds = %then
  %34 = alloca i32
  %35 = alloca i32
  %36 = load i32, i32* %17
  %multmp = mul i32 %36, 5
  store i32 %multmp, i32* %34
  %37 = load i32, i32* %17
  %multmp8 = mul i32 %37, 10
  store i32 %multmp8, i32* %35
  %38 = load i32, i32* %17
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @6, i32 0, i32 0), i32 %38)
  %40 = load i32, i32* %34
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @7, i32 0, i32 0), i32 %40)
  %42 = load i32, i32* %35
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @8, i32 0, i32 0), i32 %42)
  br label %ifcont

ifcont:                                           ; preds = %then7, %then
  %44 = load i32, i32* %17
  %45 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @9, i32 0, i32 0), i32 %44)
  %46 = load i32, i32* %18
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @10, i32 0, i32 0), i32 %46)
  %48 = load i32, i32* %19
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @11, i32 0, i32 0), i32 %48)
  br label %ifcont9

ifcont9:                                          ; preds = %ifcont, %entry
  %50 = load i32, i32* %0
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @12, i32 0, i32 0), i32 %50)
  %52 = load i32, i32* %1
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @13, i32 0, i32 0), i32 %52)
  %54 = load i32, i32* %2
  %55 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @14, i32 0, i32 0), i32 %54)
  ret i32 0
}
