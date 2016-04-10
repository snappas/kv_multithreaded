CPP_FILES = server.cpp main.cpp KV_Manager.cpp Connection_Manager.cpp
HPP_FILES = server.hpp KV_Manager.hpp Connection_Manager.hpp String_Utilities.hpp
OBJ_FILES = $(patsubst %.cpp,o/%.o,$(CPP_FILES))

OUT = server
CPP_FLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -fPIC -g

all: debug

debug:
	$(CXX) -O0 -c $(CPP_FLAGS) server.cpp -o o/server.o
	$(CXX) -O0 -c $(CPP_FLAGS) main.cpp -o o/main.o
	$(CXX) -O0 -c $(CPP_FLAGS) KV_Manager.cpp -o o/KV_Manager.o
	$(CXX) -O0 -c $(CPP_FLAGS) Connection_Manager.cpp -o o/Connection_Manager.o
	$(CXX) -pg -o $(OUT) $(OBJ_FILES) -lpthread

release:
	$(CXX) -O3 -DNDEBUG -c $(CPP_FLAGS) server.cpp -o o/server.o
	$(CXX) -O3 -DNDEBUG -c $(CPP_FLAGS) main.cpp -o o/main.o
	$(CXX) -O3 -DNDEBUG -c $(CPP_FLAGS) KV_Manager.cpp -o o/KV_Manager.o
	$(CXX) -O3 -DNDEBUG -c $(CPP_FLAGS) Connection_Manager.cpp -o o/Connection_Manager.o
	$(CXX) -o $(OUT) $(OBJ_FILES) -lpthread

clean:
	rm -f $(OBJ_FILES) $(OUT) test.log server.log
