; ModuleID = 'test'
source_filename = "test"

@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare void @print({ i8*, i64 }*)

define i64 @spp_main({ i8*, i64 }* %args) {
entry:
  %0 = getelementptr { i8*, i64 }, { i8*, i64 }* %args, i64 0
  call void @print({ i8*, i64 }* %0)
  %1 = alloca { i8*, i64 }, align 8
  %2 = getelementptr inbounds { i8*, i64 }, { i8*, i64 }* %1, i32 0, i32 1
  store i64 1, i64* %2, align 4
  %3 = getelementptr inbounds { i8*, i64 }, { i8*, i64 }* %1, i32 0, i32 0
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @0, i32 0, i32 0), i8** %3, align 8
  call void @print({ i8*, i64 }* %1)
  ret i64 0
}

