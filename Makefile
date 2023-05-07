SOURCES=$(wildcard *.cpp)
HEADERS=$(SOURCES:.cpp=.h)
#FLAGS=-DDEBUG -g
FLAGS=-g

all: main tags

main: $(SOURCES) $(HEADERS) Makefile
	mpic++ $(SOURCES) $(FLAGS) -o main

clear: clean

clean:
	rm main a.out

tags: ${SOURCES} ${HEADERS}
	ctags -R .

run: main Makefile tags
	mpirun -oversubscribe -np 8 ./main

run-debug: main Makefile tags
	mpirun -oversubscribe -np 4 xterm -hold -e gdb ./main
run-big: main Makefile tags
	mpirun -oversubscribe -np 16 ./main

# valgrind
