# TODO 1: Set the minimum required version of CMake to be 3.10

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB HPP_HEADERS CONFIGURE_DEPENDS "include/*.hpp")
message(STATUS "HPP files found: ${HPP_HEADERS}")

file(GLOB H_HEADERS CONFIGURE_DEPENDS "include/*.h")
message(STATUS "H files found: ${H_HEADERS}")

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "source/*.cpp")
message(STATUS "CPP files found: ${CPP_SOURCES}")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_AUTOMOC ON)

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${HPP_HEADERS} ${H_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        Qt6::Core
        Qt6::Widgets
        approximator
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)

# TODO 7: Set the project version number as 1.0 in the above project command

# TODO 6: Set the variable CMAKE_CXX_STANDARD to 11
#         and the variable CMAKE_CXX_STANDARD_REQUIRED to True

# TODO 8: Use configure_file to configure and copy TutorialConfig.h.in to
#         TutorialConfig.h

# TODO 3: Add an executable called Tutorial to the project
# Hint: Be sure to specify the source file as tutorial.cxx

# TODO 9: Use target_include_directories to include ${PROJECT_BINARY_DIR}
