all: test test-example

make: test, test-example

test: test.o graph.o graph_description.txt
	g++ -std=c++2a test.o graph.o -o test
	./test

test-example: test-example.o graph.o graph_description.txt
	g++ -std=c++2a test-example.o graph.o -o test-example
	./test-example

test.o: test_graph.cpp graph.cpp graph.h
	g++ -std=c++2a -c test_graph.cpp -o test.o

test-example.o: test_graph_example.cpp graph.cpp graph.h
	g++ -std=c++2a -c test_graph_example.cpp -o test-example.o

graph.o: graph.cpp graph.h
	g++ -std=c++2a -c graph.cpp

clean:
	rm -f *.o test test-example
