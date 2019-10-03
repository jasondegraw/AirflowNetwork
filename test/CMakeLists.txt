project(tests)

set(srcs ../src/fast.cpp)

add_executable(airflownetwork_tests catch.hpp ${srcs} airflowelement_tests.cpp transport_tests.cpp eigen_transport_tests.cpp)
include_directories(../src)
