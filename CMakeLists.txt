cmake_minimum_required(VERSION 3.2)

project(join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

include_directories(${PROJECT_SOURCE_DIR})

configure_file(version.h.in version.h)

add_library(join_server_common
        common/communications.h
        common/interface.h
        common/tcp/client_session.cpp
        common/tcp/server.cpp
        common/tcp/client.cpp
        )
target_link_libraries(join_server_common -lboost_system)
target_link_libraries(join_server_common -lboost_thread)

add_library(join_server_data_base
        data_base/data_base.cpp
        data_base/table.cpp
        data_base/result.cpp
        )

add_executable(join_server
        join_server/main.cpp
        join_server/join_server.cpp
        )
target_link_libraries(join_server
        join_server_common
        join_server_data_base
        )

add_executable(join_client
        join_client/main.cpp
        )
target_link_libraries(join_client
        join_server_common
        -pthread
        )

set_target_properties(join_server_common join_server_data_base join_server join_client PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )

install(TARGETS join_server_common join_server_data_base join_server join_client
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT artyom.sorokoumov@gmail.com)

include(CPack)
