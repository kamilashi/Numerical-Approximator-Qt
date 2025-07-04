file(GLOB HPP_HEADERS CONFIGURE_DEPENDS "./include/*.hpp")

file(GLOB H_HEADERS CONFIGURE_DEPENDS "./include/*.h")

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "./src/*.cpp")

message(STATUS "application header files found: ${HPP_HEADERS} ${H_HEADERS}")
message(STATUS "application source files found: ${CPP_SOURCES}")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_AUTOMOC ON)

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${HPP_HEADERS} ${H_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ./include)

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
