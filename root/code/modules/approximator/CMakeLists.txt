#file(GLOB AP_HPP_HEADERS CONFIGURE_DEPENDS "include/*.hpp")

file(GLOB_RECURSE AP_H_HEADERS CONFIGURE_DEPENDS "./include/*.h")

file(GLOB_RECURSE AP_CPP_SOURCES CONFIGURE_DEPENDS "./src/*.cpp")

file(GLOB_RECURSE AP_C_SOURCES CONFIGURE_DEPENDS "./src/*.c")

message(STATUS "Module header files found: ${AP_H_HEADERS} ")
message(STATUS "Module source files found: ${AP_CPP_SOURCES} ${AP_C_SOURCES}")

add_library(approximator
	${AP_CPP_SOURCES}
	${AP_C_SOURCES}	
	${AP_H_HEADERS}
)

# Make the headers visible to other targets
target_include_directories(approximator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)