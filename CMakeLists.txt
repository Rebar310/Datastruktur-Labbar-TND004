cmake_minimum_required(VERSION 3.20.0...4.3.0)
project(TND004-Labbar VERSION 1.0.0 DESCRIPTION "TND004 Labbar" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(enable_warnings target)
    target_compile_options(${target} PUBLIC 
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4                 # Enable the highest warning level
            /w44388             # Enable 'signed/unsigned mismatch' '(off by default)
            /we4715             # Turn 'not all control paths return a value' into a compile error
            /permissive-        # Stick to the standard
			/fsanitize=address  # Enable the Address Sanatizer, helps finding bugs at runtime
            >
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
    )
endfunction()

add_executable(Lab1 lab1/lab1.cpp lab1/test_data.txt lab1/test_result.txt)
add_executable(Lab2 lab2/lab2.cpp lab2/set.cpp lab2/node.h lab2/set.h)

enable_warnings(Lab1)
enable_warnings(Lab2)