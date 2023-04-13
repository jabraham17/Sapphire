; ModuleID = 'test'
source_filename = "test"

%spp_str = type { ptr, i64 }

@0 = private unnamed_addr constant [5 x i8] c"hewo\00", align 1
@1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare void @print(%spp_str)

define %spp_str @spp_foo() {
entry:
  %0 = alloca %spp_str, align 8
  %1 = alloca %spp_str, align 8
  %2 = getelementptr inbounds %spp_str, ptr %1, i32 0, i32 1
  store i64 4, ptr %2, align 4
  %3 = getelementptr inbounds %spp_str, ptr %1, i32 0, i32 0
  store ptr @0, ptr %3, align 8
  %4 = load %spp_str, ptr %1, align 8
  store %spp_str %4, ptr %0, align 8
  %5 = load %spp_str, ptr %0, align 8
  ret %spp_str %5
}

define i64 @spp_getLocalInt() {
entry:
  %0 = alloca <{ ptr, i64 }>, align 8
  %1 = load <{ ptr, i64 }>, ptr %0, align 1
  %2 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %1, ptr %2, align 1
  %3 = getelementptr inbounds <{ ptr, i64 }>, ptr %2, i32 0, i32 0
  %4 = load ptr, ptr %3, align 8
  %5 = getelementptr i64, ptr %4, i64 0
  %6 = load i64, ptr %5, align 4
  ret i64 %6
}

define i64 @spp_get() {
entry:
  %0 = alloca i64, align 8
  store i64 10, ptr %0, align 4
  %1 = load i64, ptr %0, align 4
  ret i64 %1
}

define i64 @spp_getParamInt(<{ ptr, i64 }> %args) {
entry:
  %0 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %args, ptr %0, align 1
  %1 = load <{ ptr, i64 }>, ptr %0, align 1
  %2 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %1, ptr %2, align 1
  %3 = getelementptr inbounds <{ ptr, i64 }>, ptr %2, i32 0, i32 0
  %4 = load ptr, ptr %3, align 8
  %5 = getelementptr i64, ptr %4, i64 0
  %6 = load i64, ptr %5, align 4
  ret i64 %6
}

define i64 @spp_getParamInt2(<{ ptr, i64 }> %args, i64 %index) {
entry:
  %0 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %args, ptr %0, align 1
  %1 = alloca i64, align 8
  store i64 %index, ptr %1, align 4
  %2 = load <{ ptr, i64 }>, ptr %0, align 1
  %3 = load i64, ptr %1, align 4
  %4 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %2, ptr %4, align 1
  %5 = getelementptr inbounds <{ ptr, i64 }>, ptr %4, i32 0, i32 0
  %6 = load ptr, ptr %5, align 8
  %7 = getelementptr i64, ptr %6, i64 %3
  %8 = load i64, ptr %7, align 4
  ret i64 %8
}

define %spp_str @spp_getLocal() {
entry:
  %0 = alloca <{ ptr, i64 }>, align 8
  %1 = load <{ ptr, i64 }>, ptr %0, align 1
  %2 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %1, ptr %2, align 1
  %3 = getelementptr inbounds <{ ptr, i64 }>, ptr %2, i32 0, i32 0
  %4 = load ptr, ptr %3, align 8
  %5 = getelementptr %spp_str, ptr %4, i64 0
  %6 = load %spp_str, ptr %5, align 8
  ret %spp_str %6
}

define %spp_str @spp_getParam(<{ ptr, i64 }> %args) {
entry:
  %0 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %args, ptr %0, align 1
  %1 = load <{ ptr, i64 }>, ptr %0, align 1
  %2 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %1, ptr %2, align 1
  %3 = getelementptr inbounds <{ ptr, i64 }>, ptr %2, i32 0, i32 0
  %4 = load ptr, ptr %3, align 8
  %5 = getelementptr %spp_str, ptr %4, i64 0
  %6 = load %spp_str, ptr %5, align 8
  ret %spp_str %6
}

define %spp_str @spp_withTemp() {
entry:
  %0 = alloca <{ ptr, i64 }>, align 8
  %1 = alloca %spp_str, align 8
  %2 = load <{ ptr, i64 }>, ptr %0, align 1
  %3 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %2, ptr %3, align 1
  %4 = getelementptr inbounds <{ ptr, i64 }>, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = getelementptr %spp_str, ptr %5, i64 0
  %7 = load %spp_str, ptr %6, align 8
  store %spp_str %7, ptr %1, align 8
  %8 = load %spp_str, ptr %1, align 8
  ret %spp_str %8
}

define i64 @spp_main(<{ ptr, i64 }> %args) {
entry:
  %0 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %args, ptr %0, align 1
  %1 = alloca %spp_str, align 8
  %2 = load <{ ptr, i64 }>, ptr %0, align 1
  %3 = alloca <{ ptr, i64 }>, align 8
  store <{ ptr, i64 }> %2, ptr %3, align 1
  %4 = getelementptr inbounds <{ ptr, i64 }>, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = getelementptr %spp_str, ptr %5, i64 0
  %7 = load %spp_str, ptr %6, align 8
  store %spp_str %7, ptr %1, align 8
  %8 = load %spp_str, ptr %1, align 8
  call void @print(%spp_str %8)
  %9 = alloca %spp_str, align 8
  %10 = getelementptr inbounds %spp_str, ptr %9, i32 0, i32 1
  store i64 1, ptr %10, align 4
  %11 = getelementptr inbounds %spp_str, ptr %9, i32 0, i32 0
  store ptr @1, ptr %11, align 8
  %12 = load %spp_str, ptr %9, align 8
  call void @print(%spp_str %12)
  ret i64 0
}

