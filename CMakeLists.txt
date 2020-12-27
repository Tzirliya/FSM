cmake_minimum_required(VERSION 3.17)
project(SemesterProject C)

set(CMAKE_C_STANDARD 11)

add_executable(SemesterProject main.c)
add_executable(Test cmake-build-debug/Testing/Tests/tests.c)

enable_testing()

add_test(test1 SemesterProject FSMDefinitionFiles/test1.fsm FSMInputFiles/test1.inputs)
set_tests_properties(test1 PROPERTIES WILL_FAIL FALSE)

add_test(test2 SemesterProject FSMDefinitionFiles/test2.fsm FSMInputFiles/test1.inputs)
set_tests_properties(test2 PROPERTIES WILL_FAIL TRUE)

add_test(test3 SemesterProject FSMDefinitionFiles/test1.fsm FSMInputFiles/test2.inputs)
set_tests_properties(test3 PROPERTIES WILL_FAIL TRUE)