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
# Vi försöker hitta biblioteken. Om de saknas hoppar vi över Lab 3 så länge.
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED) # Se till att det står glfw3 här
find_package(glm CONFIG REQUIRED)

   
target_include_directories(Collision PUBLIC 
    "lab3/collisionsystem/include"
    "lab3/collisionsystem/src"
)
target_link_libraries(Collision PUBLIC glm::glm fmt::fmt glad::glad glfw)

    
file(COPY lab3/collisionsystem/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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

# External libraries
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(Rendering PUBLIC glm::glm fmt::fmt glad::glad glfw)

target_compile_definitions(Rendering PRIVATE DATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/lab3/detectionSystem/data\")
target_compile_definitions(LinesDiscovery PRIVATE DATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/lab3/detectionSystem/data\")

enable_warnings(Rendering)
enable_warnings(LinesDiscovery)

# Aktivera mappar i Solution Explorer
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Lägg Lab3 (den första vi gjorde) i en undermapp som heter Part1
set_target_properties(Collision PROPERTIES FOLDER "Lab3/Part1")

# Lägg Rendering och LinesDiscovery i en undermapp som heter Part2
set_target_properties(Rendering PROPERTIES FOLDER "Lab3/Part2")
set_target_properties(LinesDiscovery PROPERTIES FOLDER "Lab3/Part2")