cmake_minimum_required(VERSION 3.10.0)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 1.0.${PATCH_VERSION})

project(lab VERSION ${PROJECT_VERSION})


add_executable(lab main.cpp)



target_compile_features(lab PRIVATE cxx_std_20)


if (MSVC)
    target_compile_options(lab PRIVATE
        /W4
    )

else ()
    target_compile_options(lab PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()

install(TARGETS lab RUNTIME DESTINATION bin)

if(UNIX)
    set(CPACK_GENERATOR "DEB")
else ()
    set(CPACK_GENERATOR "ZIP")
endif()


set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_func test_func)