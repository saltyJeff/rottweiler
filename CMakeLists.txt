cmake_minimum_required(VERSION 3.8)
project(rottweiler)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_subdirectory(spdlog)
add_subdirectory(sockpp)
add_subdirectory(libserial)

add_executable(rottweiler main.cpp RotArgs.cpp RotArgs.h RotCoord.cpp RotCoord.h GreenHeron.cpp GreenHeron.h RotCmd.cpp RotCmd.h tests/test_rotcmd.cpp tests/tests.h Rotor.h DummyRotor.cpp DummyRotor.h RottweilerServer.cpp RottweilerServer.h RottweilerClient.cpp RottweilerClient.h)
set_target_properties(rottweiler PROPERTIES OUTPUT_NAME "rottweilerd")
target_link_libraries(rottweiler spdlog::spdlog sockpp_lib serial_lib)
add_compile_options(-g)

enable_testing()
add_test(NAME rottweiler_tests COMMAND "rottweilerd --test")