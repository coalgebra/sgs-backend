
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

%fucker = type { i32, i8, i32 }

define i32 @printFucker(%fucker* %a) {
entry:
  %access.res = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 0
  %load = load i32, i32* %access.res
  %call.res = call i32 @printNum(i32 %load)
  %call.res1 = call i32 @putchar(i32 32)
  %access.res2 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 1
  %load3 = load i8, i8* %access.res2
  %param.sext = sext i8 %load3 to i32
  %call.res4 = call i32 @putchar(i32 %param.sext)
  %call.res5 = call i32 @putchar(i32 32)
  %access.res6 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 2
  %load7 = load i32, i32* %access.res6
  %call.res8 = call i32 @printNum(i32 %load7)
  ret i32 0
}

define i32 @main() {
entry:
  %a = alloca %fucker
  %access.res = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 0
  store i32 1, i32* %access.res
  %access.res1 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 1
  store i8 71, i8* %access.res1
  %access.res2 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 2
  store i32 10, i32* %access.res2
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %access.res3 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 2
  %load.val = load i32, i32* %access.res3
  %gt.res = icmp sgt i32 %load.val, 0
  br i1 %gt.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %access.res4 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 1
  %load = load i8, i8* %access.res4
  %param.sext = sext i8 %load to i32
  %call.res = call i32 @putchar(i32 %param.sext)
  %access.res5 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 0
  %access.res6 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 0
  %load.val7 = load i32, i32* %access.res6
  %add.res = add i32 %load.val7, 1
  store i32 %add.res, i32* %access.res5
  %access.res8 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 2
  %access.res9 = getelementptr inbounds %fucker, %fucker* %a, i32 0, i32 2
  %load.val10 = load i32, i32* %access.res9
  %sub.res = sub i32 %load.val10, 1
  store i32 %sub.res, i32* %access.res8
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  call i32 @newline()
  %call.res11 = call i32 @printFucker(%fucker* %a)
  ret i32 0
}
