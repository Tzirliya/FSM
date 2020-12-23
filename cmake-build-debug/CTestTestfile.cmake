# CMake generated Testfile for 
# Source directory: /home/tplotkin/ComputerSystems/SemesterProject
# Build directory: /home/tplotkin/ComputerSystems/SemesterProject/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test1 "SemesterProject" "FSMDefinitionFiles/test1.fsm" "FSMInputFiles/test1.inputs")
set_tests_properties(test1 PROPERTIES  WILL_FAIL "FALSE" _BACKTRACE_TRIPLES "/home/tplotkin/ComputerSystems/SemesterProject/CMakeLists.txt;11;add_test;/home/tplotkin/ComputerSystems/SemesterProject/CMakeLists.txt;0;")
add_test(test2 "SemesterProject" "FSMDefinitionFiles/test1.fsm" "FSMInputFiles/test1.inputs")
set_tests_properties(test2 PROPERTIES  WILL_FAIL "FALSE" _BACKTRACE_TRIPLES "/home/tplotkin/ComputerSystems/SemesterProject/CMakeLists.txt;14;add_test;/home/tplotkin/ComputerSystems/SemesterProject/CMakeLists.txt;0;")
