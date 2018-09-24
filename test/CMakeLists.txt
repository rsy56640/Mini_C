SET (TEST_APP main.cpp test.cpp test.h test_lgy.cpp test_lrk.cpp test_rsy.cpp test_zhsh.cpp)

ADD_DEFINITIONS (${CXX_FLAGS} -std=c++17)
SET (CXX_FLAGS -g -Wall -fPIC -D RELEASE)

ADD_LIBRARY (TEST ${TEST_APP})
ADD_EXECUTABLE (main main.cpp)