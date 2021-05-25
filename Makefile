all: main.cpp
	g++ -g main.cpp -o main
clean:
	rm -f *.o main