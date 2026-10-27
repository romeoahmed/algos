cmake_minimum_required(VERSION 4.4)
project(algos LANGUAGES CXX)

enable_testing()
include(FetchContent)

option(ALGOS_SANITIZE "Enable sanitizers in Debug" ON)

# These predicates are evaluated in both target and test properties.
set(msvc_debug
    "$<AND:$<CONFIG:Debug>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>>"
)
set(gnu_debug
    "$<AND:$<CONFIG:Debug>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>>"
)

FetchContent_Declare(
    ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG 89d751b051ea250597b58353f2df6230bca04730
    GIT_SUBMODULES ""
    SYSTEM
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(
    rapidcheck
    GIT_REPOSITORY https://github.com/emil-e/rapidcheck.git
    GIT_TAG 2c3c4365aca21ef4e612768fdc95b1ce8b39a651
    GIT_SUBMODULES ""
    SYSTEM
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ut rapidcheck)

# RapidCheck compiles Random.cpp with MSVC /O2, which conflicts with /RTC.
set_target_properties(
    rapidcheck
    PROPERTIES SKIP_LINTING YES MSVC_RUNTIME_CHECKS ""
)

if(ALGOS_SANITIZE)
    # MSVC requires matching ASan container annotations in linked objects.
    target_compile_options(
        rapidcheck
        PRIVATE "$<${msvc_debug}:/fsanitize=address>"
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
        tromino_tiling
        karatsuba
        jump_game
        erase_overlap
        burst_balloons
        partition_labels
        forward_chaining
        bayesian_inference
        climbing_stairs
        house_robber
        unique_paths
        min_cost_stairs
        max_subarray
        stock_profit
        longest_increasing
        rain_water
        knapsack
        longest_common
        edit_distance
        obstacle_paths
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
            EXPORT_COMPILE_COMMANDS YES
    )
    target_compile_options(
        ${name}
        PRIVATE
            "$<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:/W4;/permissive-;/utf-8;/Zc:__cplusplus>"
            "$<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:-Wall;-Wextra;-Wpedantic;-Wconversion;-Wsign-conversion;-Wshadow;-Wformat=2>"
    )
    add_test(NAME ${name} COMMAND ${name})

    if(ALGOS_SANITIZE)
        # MSVC /RTC is incompatible with ASan.
        set_property(TARGET ${name} PROPERTY MSVC_RUNTIME_CHECKS "")
        target_compile_options(
            ${name}
            PRIVATE
                "$<${msvc_debug}:/fsanitize=address>"
                "$<${gnu_debug}:-fsanitize=address,undefined;-fno-sanitize-recover=all;-fno-omit-frame-pointer>"
        )
        target_link_options(
            ${name}
            PRIVATE
                "$<${msvc_debug}:/INCREMENTAL:NO>"
                "$<${gnu_debug}:-fsanitize=address,undefined>"
        )
        # Locate the MSVC ASan runtime beside the compiler for Debug tests.
        set_property(
            TEST ${name}
            PROPERTY
                ENVIRONMENT_MODIFICATION
                    "$<${msvc_debug}:PATH=path_list_prepend:$<PATH:GET_PARENT_PATH,${CMAKE_CXX_COMPILER}>>"
        )
    endif()
endforeach()
