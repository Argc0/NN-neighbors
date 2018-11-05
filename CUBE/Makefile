all:
	g++ --std=c++11 -Wall -g -o cube main_cube.cpp cube.cpp hash.cpp functions.cpp functions_cube.cpp -lm

# clean house
clean:
	rm -f main_cube.o cube.o hash.o functions.o functions_cube.o cube

# do a bit of accounting
count:
	wc main_cube.cpp cube.cpp functions.cpp functions_cube.cpp hash.cpp
