cmake_minimum_required(VERSION 3.25.1)

project(MyAllocator VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

add_executable(my_allocator_03 main03.cpp)
add_executable(my_allocator main.cpp)

target_include_directories(
    my_allocator_03
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(
    my_allocator
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(my_allocator_03 PROPERTIES
    CXX_STANDARD 98
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(my_allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

install(
    TARGETS my_allocator_03
    COMPONENT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
)

install(
    TARGETS my_allocator
    COMPONENT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR}) 
SET(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR}) 
SET(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH}) 
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_PACKAGE_CONTACT "my")
INCLUDE(CPack)

