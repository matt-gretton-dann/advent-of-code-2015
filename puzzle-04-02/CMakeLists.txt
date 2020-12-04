find_package(OpenSSL REQUIRED)

add_executable(puzzle-04-02 main.cc)
target_link_libraries(puzzle-04-02 OpenSSL::SSL)
