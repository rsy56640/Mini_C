SET (SRC_APP lexer.cpp lexer.h miniC_exception.h)

ADD_DEFINITIONS (${CXX_FLAGS} -std=c++17)
SET (CXX_FLAGS -g -Wall -fPIC)

ADD_LIBRARY (SRC ${SRC_APP})