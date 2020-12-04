find_package(OpenSSL REQUIRED)

add_executable(puzzle-04-01 main.cc)
target_link_libraries(puzzle-04-01 OpenSSL::SSL)
