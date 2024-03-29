FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -std=c++11 -O2
COMPILLER=g++

all: start

start: matrix.o main.o
	$(COMPILLER) $(FLAGS) -o nm-lab1 matrix.o main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

matrix.o: matrix.cpp
	$(COMPILLER) -c $(FLAGS) matrix.cpp

clean:
	@-rm -f *.o *.gch *.dat nm-lab1
	@echo "Clean success"
