
all: main

main: main.o
	$(CXX) -o $@ $^

