cmake_minimum_required(VERSION 3.8)
project(rottweiler)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_subdirectory(spdlog)
add_subdirectory(sockpp)
add_subdirectory(libserial)

add_library(rottweiler_lib RotArgs.cpp RotArgs.h RotCoord.cpp RotCoord.h GreenHeron.cpp GreenHeron.h RotCmd.cpp RotCmd.h Rotor.h DummyRotor.cpp DummyRotor.h RottweilerServer.cpp RottweilerServer.h RottweilerClient.cpp RottweilerClient.h RotErrs.h)
add_executable(rottweiler main.cpp)
set_target_properties(rottweiler PROPERTIES OUTPUT_NAME "rottweilerd")
target_link_libraries(rottweiler_lib spdlog::spdlog sockpp_lib serial_lib)
target_link_libraries(rottweiler rottweiler_lib)

enable_testing()
add_executable(rottweiler-test tests/main.cpp tests/test_rotcmd.cpp tests/tests.h)
target_link_libraries(rottweiler-test rottweiler_lib)
add_test(NAME rottweiler_tests COMMAND "rottweilerd --test")

add_compile_options(-g)