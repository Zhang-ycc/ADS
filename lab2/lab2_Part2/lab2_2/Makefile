cc = g++
prog = main
objects = main.o
CXXFLAGS = -std=c++11

$(prog): $(objects)
	$(cc) ${CXXFLAGS} -o $(prog) $(objects)


.PHONY:clean
clean:
	rm -rf *.o $(prog) temp
