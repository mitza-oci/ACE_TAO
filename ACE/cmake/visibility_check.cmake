
set(LIB_CXX_VISIBILITY_PRESET default)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.2)
        set(LIB_CXX_VISIBILITY_PRESET hidden)
    endif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.2)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
        set(LIB_CXX_VISIBILITY_PRESET hidden)
    endif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        set(LIB_CXX_VISIBILITY_PRESET hidden)
    endif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
endif()

if (NOT "${LIB_CXX_VISIBILITY_PRESET}" STREQUAL "hidden")
    add_definitions("-DACE_HAS_CUSTOM_EXPORT_MACROS=0")
endif (NOT "${LIB_CXX_VISIBILITY_PRESET}" STREQUAL "hidden")
