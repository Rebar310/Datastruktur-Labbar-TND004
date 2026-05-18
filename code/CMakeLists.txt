cmake_minimum_required(VERSION 3.20.0...4.3.0)
project(TND004-Labbar VERSION 1.0.0 DESCRIPTION "TND004-Labbar" LANGUAGES CXX)
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

# ==============================================================================
# --- VCPKG-SKYDD: Ignorera textfiler vid länkning ---
# ==============================================================================
set_source_files_properties(
    lab1/test_data.txt 
    lab1/test_result.txt
    lab4/code4-part1/digraph1.txt 
    lab4/code4-part1/digraph1_test_run.txt 
    lab4/code4-part1/digraph2.txt 
    lab4/code4-part1/digraph2_test_run.txt
    lab4/code4-part2/graph1.txt 
    lab4/code4-part2/graph1_test_run.txt 
    lab4/code4-part2/graph2.txt 
    lab4/code4-part2/graph2_test_run.txt
    lab4/code4-part3/flights-data.txt 
    lab4/code4-part3/travel-execution.txt
    PROPERTIES HEADER_FILE_ONLY TRUE
)


add_executable(Lab1 lab1/lab1.cpp lab1/test_data.txt lab1/test_result.txt)
add_executable(Lab2 lab2/lab2.cpp lab2/set.cpp lab2/node.h lab2/set.h)
add_executable(Collision
        lab3/collisionsystem/include/particlesystem/collisionsystem.h 
        lab3/collisionsystem/include/particlesystem/event.h 
        lab3/collisionsystem/include/particlesystem/particle.h 
        lab3/collisionsystem/include/particlesystem/priorityqueue.h
        lab3/collisionsystem/include/particlesystem/priorityqueue-vector.h 	
        lab3/collisionsystem/include/rendering/window.h 
        lab3/collisionsystem/src/particlesystem/collisionsystem.cpp 
        lab3/collisionsystem/src/particlesystem/event.cpp
        lab3/collisionsystem/src/particlesystem/particle.cpp 
        lab3/collisionsystem/src/rendering/window.cpp
        lab3/collisionsystem/src/lab3.cpp 
    )
  
#Lab 3: del 1
add_executable(Rendering	
	lab3/detectionSystem/renderingSystem/include/readfiles.h 
    lab3/detectionSystem/renderingSystem/include/window.h  
	lab3/detectionSystem/renderingSystem/src/readfiles.cpp 
    lab3/detectionSystem/renderingSystem/src/window.cpp
	lab3/detectionSystem/renderingSystem/src/lab3-part2.cpp	# main   
)

add_executable(LinesDiscovery	
    #include/file1.h	 # ADD other header files, if needed
	lab3/detectionSystem/linesDiscoverySystem/include/find-patterns.hpp
    #src/file1.cpp		#ADD other source files, if needed
	lab3/detectionSystem/linesDiscoverySystem/src/find-patterns.cpp	# main   
)

enable_warnings(Lab1)
enable_warnings(Lab2)
enable_warnings(Collision)

# --- Laboration 3 (Collision System) ---
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED) 
find_package(glm CONFIG REQUIRED)

target_include_directories(Collision PUBLIC 
    "lab3/collisionsystem/include"
    "lab3/collisionsystem/src"
)
target_link_libraries(Collision PUBLIC glm::glm fmt::fmt glad::glad glfw)

# LÖSNING RAD 88: Gamla file(COPY) borttagen för att förhindra krasch.

target_include_directories(Rendering PUBLIC "lab3/detectionSystem/renderingSystem/include")
target_include_directories(LinesDiscovery PUBLIC "lab3/detectionSystem/linesDiscoverySystem/include")

target_compile_options(Rendering PUBLIC 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
)

target_compile_options(LinesDiscovery PUBLIC 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
)

target_link_libraries(Rendering PUBLIC glm::glm fmt::fmt glad::glad glfw)

target_compile_definitions(Rendering PRIVATE DATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/lab3/detectionSystem/data\")
target_compile_definitions(LinesDiscovery PRIVATE DATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/lab3/detectionSystem/data\")

enable_warnings(Rendering)
enable_warnings(LinesDiscovery)

# Aktivera mappar i Solution Explorer
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_target_properties(Collision PROPERTIES FOLDER "Lab3/Part1")
set_target_properties(Rendering PROPERTIES FOLDER "Lab3/Part2")
set_target_properties(LinesDiscovery PROPERTIES FOLDER "Lab3/Part2")


# ==============================================================================
# --- Laboration 4 ---
# ==============================================================================
add_executable(Lab4-part1 
    lab4/code4-part1/edge.h 
    lab4/code4-part1/digraph.h 
    lab4/code4-part1/digraph.cpp 
    lab4/code4-part1/main.cpp 
    lab4/code4-part1/digraph1.txt 
    lab4/code4-part1/digraph1_test_run.txt 
    lab4/code4-part1/digraph2.txt 
    lab4/code4-part1/digraph2_test_run.txt
)

add_executable(Lab4-part2 
    lab4/code4-part2/edge.h 
    lab4/code4-part2/dsets.h 
    lab4/code4-part2/dsets.cpp 
    lab4/code4-part2/graph.h 
    lab4/code4-part2/graph.cpp 
    lab4/code4-part2/main.cpp 
    lab4/code4-part2/graph1.txt 
    lab4/code4-part2/graph1_test_run.txt 
    lab4/code4-part2/graph2.txt 
    lab4/code4-part2/graph2_test_run.txt
)

# AUTOMATISK FILSKAPARE: Skapar main.cpp automatiskt om mappen för part 3 är tom
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lab4/code4-part3/main.cpp")
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/lab4/code4-part3/main.cpp"
"#include <iostream>\n\nint main() {\n    std::cout << \"Hej från Lab 4 Del 3!\\n\";\n    return 0;\n}\n"
    )
endif()

add_executable(Lab4-part3
    lab4/code4-part3/flights-data.txt
    lab4/code4-part3/travel-execution.txt
    lab4/code4-part3/main.cpp
)

enable_warnings(Lab4-part1)
enable_warnings(Lab4-part2)
enable_warnings(Lab4-part3)

# LÖSNING RAD 162: Gamla file(COPY) borttagen för att förhindra krasch.

set_target_properties(Lab4-part1 PROPERTIES FOLDER "Lab4")
set_target_properties(Lab4-part2 PROPERTIES FOLDER "Lab4")
set_target_properties(Lab4-part3 PROPERTIES FOLDER "Lab4")