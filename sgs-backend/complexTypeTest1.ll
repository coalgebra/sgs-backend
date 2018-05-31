
@printNum.constStr = constant [3 x i8] c"%d\00", align 1
@printStr.constStr = constant[3 x i8] c"%s\00", align 1
@printFloat.constStr = constant[3 x i8] c"%f\00", align 1
declare i32 @printf(i8*, ...)
declare i32 @scanf(i8*, ...)
declare i32 @putchar(i32)
declare i32 @getchar()
declare i8* @strcpy(i8*, i8*)

define i8 @intToChar(i32) {
	%2 = trunc i32 %0 to i8
	ret i8 %2
}

define i32 @charToInt(i8) {
	%2 = alloca i8, align 1
	store i8 %0, i8* %2, align 1
	%3 = load i8, i8* %2, align 1
	%4 = sext i8 %3 to i32
	ret i32 %4
}

define i1 @intToBool(i32) {
	%2 = icmp eq i32 %0, 0
	ret i1 %2
}

define i32 @boolToInt(i1) {
	%2 = sext i1 %0 to i32
	ret i32 %2
}

define float @intToFloat(i32) {
	%2 = sitofp i32 %0 to float
	ret float %2
}

define i32 @floatToInt(float) {
	%2 = fptosi float %0 to i32
	ret i32 %2
}

define i32 @printNum(i32) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32 %0)
	ret i32 0
}
define i32 @printFloat(float) {
	%2 = fpext float %0 to double
	%3 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printFloat.constStr, i32 0, i32 0), double %2)
	ret i32 0
}

define i32 @printStr(i8*) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}

define i32 @readNum() {
	%1 = alloca i32, align 4
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32* %1)
	%3 = load i32, i32* %1, align 4
	ret i32 %3
}
define i32 @newline() {
	call i32 @putchar(i32 10)
	ret i32 0
}

define i32 @readStr(i8*)  {
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}
; ModuleID = 'wtf'
source_filename = "wtf"

%fucker.0 = type { i32, [10 x i8], i32 }

@"123.str" = private constant [4 x i8] c"123\00"

define i32 @main() {
entry:
  %array.temp4 = alloca i8*
  %array.temp2 = alloca i8*
  %array.temp = alloca i8*
  %a = alloca %fucker.0
  %access.res = getelementptr inbounds %fucker.0, %fucker.0* %a, i32 0, i32 1
  %0 = getelementptr inbounds [10 x i8], [10 x i8]* %access.res, i32 0, i32 0
  store i8* %0, i8** %array.temp
  %str.ptr = load i8*, i8** %array.temp
  %1 = call i8* @strcpy(i8* %str.ptr, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @"123.str", i32 0, i32 0))
  %access.res1 = getelementptr inbounds %fucker.0, %fucker.0* %a, i32 0, i32 2
  store i32 6, i32* %access.res1
  %access.res3 = getelementptr inbounds %fucker.0, %fucker.0* %a, i32 0, i32 1
  %2 = getelementptr inbounds [10 x i8], [10 x i8]* %access.res3, i32 0, i32 0
  store i8* %2, i8** %array.temp2
  %array.load = load i8*, i8** %array.temp2
  %visit = getelementptr inbounds i8, i8* %array.load, i32 1
  store i8 97, i8* %visit
  %access.res5 = getelementptr inbounds %fucker.0, %fucker.0* %a, i32 0, i32 1
  %3 = getelementptr inbounds [10 x i8], [10 x i8]* %access.res5, i32 0, i32 0
  store i8* %3, i8** %array.temp4
  %load = load i8*, i8** %array.temp4
  %call.res = call i32 @printStr(i8* %load)
  ret i32 0
}
