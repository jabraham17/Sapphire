; ModuleID = 'test'
source_filename = "test"

%spp_string = type { ptr, i64 }
%spp_array_int = type { ptr, i64 }
%spp_array_string = type { ptr, i64 }

@0 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare void @print(%spp_string)

define void @spp_println(%spp_string %x) {
entry:
  %0 = alloca %spp_string, align 8
  %x_local = alloca %spp_string, align 8
  store %spp_string %x, ptr %x_local, align 8
  %1 = load %spp_string, ptr %x_local, align 8
  call void @print(%spp_string %1)
  %2 = getelementptr inbounds %spp_string, ptr %0, i32 0, i32 1
  store i64 1, ptr %2, align 4
  %3 = getelementptr inbounds %spp_string, ptr %0, i32 0, i32 0
  store ptr @0, ptr %3, align 8
  %4 = load %spp_string, ptr %0, align 8
  call void @print(%spp_string %4)
  ret void
}

define %spp_string @spp_str_concat(%spp_string %x) {
entry:
  %x_local = alloca %spp_string, align 8
  store %spp_string %x, ptr %x_local, align 8
  %0 = load %spp_string, ptr %x_local, align 8
  ret %spp_string %0
}

define %spp_array_int @spp_arr_int_concat(%spp_array_int %x) {
entry:
  %x_local = alloca %spp_array_int, align 8
  store %spp_array_int %x, ptr %x_local, align 8
  %0 = load %spp_array_int, ptr %x_local, align 8
  ret %spp_array_int %0
}

define i64 @spp_main(%spp_array_string %args) {
entry:
  %0 = alloca i64, align 8
  %a = alloca %spp_string, align 8
  %loopCount = alloca i64, align 8
  %args_local = alloca %spp_array_string, align 8
  store %spp_array_string %args, ptr %args_local, align 8
  br label %loop_init

loop_init:                                        ; preds = %entry
  store i64 0, ptr %loopCount, align 4
  %1 = getelementptr inbounds %spp_array_string, ptr %args_local, i32 0, i32 1
  %2 = load i64, ptr %1, align 4
  br label %loop_cond

loop_cond:                                        ; preds = %loop_body, %loop_init
  %3 = load i64, ptr %loopCount, align 4
  %4 = icmp slt i64 %3, %2
  br i1 %4, label %loop_body, label %12

loop_body:                                        ; preds = %loop_cond
  %5 = getelementptr inbounds %spp_array_string, ptr %args_local, i32 0, i32 1
  %6 = getelementptr inbounds %spp_array_string, ptr %args_local, i32 0, i32 0
  %7 = load ptr, ptr %6, align 8
  %8 = getelementptr %spp_string, ptr %7, i64 %3
  %9 = load %spp_string, ptr %8, align 8
  store %spp_string %9, ptr %a, align 8
  %10 = load %spp_string, ptr %a, align 8
  call void @spp_println(%spp_string %10)
  %11 = add i64 %3, 1
  store i64 %11, ptr %loopCount, align 4
  br label %loop_cond

12:                                               ; preds = %loop_cond
  store i64 0, ptr %0, align 4
  %13 = load i64, ptr %0, align 4
  ret i64 %13
}

