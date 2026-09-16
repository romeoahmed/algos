cmake_minimum_required(VERSION 4.2)
project(algos LANGUAGES CXX)

enable_testing()
include(FetchContent)

FetchContent_Declare(
    ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG f923e6fe4b7542d75e0c4ee54ad0af6a5382a87c # v2.3.1
    GIT_SUBMODULES ""
    SYSTEM
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(
    rapidcheck
    GIT_REPOSITORY https://github.com/emil-e/rapidcheck.git
    GIT_TAG 6e8dadfdafa3a74eabb52ead87f8787f72eccd0b
    GIT_SUBMODULES ""
    SYSTEM
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ut rapidcheck)

# Supply RapidCheck's missing headers; its MSVC /O2 conflicts with /RTC.
set_target_properties(
    rapidcheck
    PROPERTIES SKIP_LINTING YES MSVC_RUNTIME_CHECKS ""
)
target_compile_options(
    rapidcheck
    PRIVATE
        "$<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:/FIalgorithm;/FIiterator>"
        "$<$<NOT:$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:SHELL:-include algorithm -include iterator>"
)

option(ALGOS_SANITIZE "Enable sanitizers in Debug" ON)

if(ALGOS_SANITIZE AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    # MSVC requires matching ASan container annotations in linked objects.
    target_compile_options(
        rapidcheck
        PRIVATE "$<$<CONFIG:Debug>:/fsanitize=address>"
    )
endif()

foreach(
    name
    IN
    ITEMS
        merge_sort
        quickselect
        rotated_search
        majority
        expressions
        longest_substring
        search_tree
        a_star
)
    add_executable(
        ${name}
        src/${name}/solution.hpp
        src/${name}/solution.cpp
        src/${name}/test.cpp
    )
    target_link_libraries(${name} PRIVATE Boost::ut rapidcheck)
    target_compile_features(${name} PRIVATE cxx_std_23)
    set_target_properties(
        ${name}
        PROPERTIES
            CXX_EXTENSIONS NO
            CXX_SCAN_FOR_MODULES NO
            COMPILE_WARNING_AS_ERROR YES
    )
    add_test(NAME ${name} COMMAND ${name})

    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        target_compile_options(
            ${name}
            PRIVATE /W4 /permissive- /utf-8 /Zc:__cplusplus
        )
        if(ALGOS_SANITIZE)
            set_property(TARGET ${name} PROPERTY MSVC_RUNTIME_CHECKS "")
            target_compile_options(
                ${name}
                PRIVATE "$<$<CONFIG:Debug>:/fsanitize=address>"
            )
            target_link_options(
                ${name}
                PRIVATE "$<$<CONFIG:Debug>:/INCREMENTAL:NO>"
            )
            # Locate the ASan runtime beside the compiler for Debug tests.
            set_property(
                TEST ${name}
                PROPERTY
                    ENVIRONMENT_MODIFICATION
                        "$<$<CONFIG:Debug>:PATH=path_list_prepend:$<PATH:GET_PARENT_PATH,${CMAKE_CXX_COMPILER}>>"
            )
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
        target_compile_options(
            ${name}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                -Wshadow
                -Wformat=2
        )
        if(ALGOS_SANITIZE)
            target_compile_options(
                ${name}
                PRIVATE
                    "$<$<CONFIG:Debug>:-fsanitize=address,undefined;-fno-sanitize-recover=all;-fno-omit-frame-pointer>"
            )
            target_link_options(
                ${name}
                PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address,undefined>"
            )
        endif()
    endif()
endforeach()
