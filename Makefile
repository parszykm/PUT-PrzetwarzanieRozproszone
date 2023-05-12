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
	mpirun -oversubscribe -H lab-sec-18:4,lab-sec-19:4 -np 8 ./main
run-normal: main Makefile tags
	mpirun -oversubscribe -np 8 ./main
run-debug: main Makefile tags
	# mpirun -oversubscribe -np 4 xterm -hold -e gdb ./main
	mpirun -oversubscribe -H lab-sec-20:3,lab-sec-21:2 -np 5 xterm -hold -e ./main
run-big: main Makefile tags
	mpirun -oversubscribe -np 16 ./main

# valgrind
