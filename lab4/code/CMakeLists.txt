cmake_minimum_required(VERSION 3.20.0...4.3.0)
project(TND004-Lab-4 VERSION 1.0.0 DESCRIPTION "TND004 Lab 4" LANGUAGES CXX)
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

add_executable(Lab4-part1 code4-part1/edge.h code4-part1/digraph.h code4-part1/digraph.cpp code4-part1/main.cpp 
                     code4-part1/digraph1.txt code4-part1/digraph1_test_run.txt code4-part1/digraph2.txt code4-part1/digraph2_test_run.txt)
add_executable(Lab4-part2 code4-part2/edge.h code4-part2/dsets.h code4-part2/dsets.cpp 
                     code4-part2/graph.h code4-part2/graph.cpp code4-part2/main.cpp 
					 code4-part2/graph1.txt code4-part2/graph1_test_run.txt code4-part2/graph2.txt code4-part2/graph2_test_run.txt)
add_executable(Lab4-part3
    code4-part3/flights-data.txt code4-part3/travel-execution.txt code4-part3/part3main.cpp
)

enable_warnings(Lab4-part1)
enable_warnings(Lab4-part2)
enable_warnings(Lab4-part3)