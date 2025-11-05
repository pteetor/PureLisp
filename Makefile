HDRS = consVM.h

SRC = atom.cpp build_globals.cpp cell.cpp cons.cpp eval.cpp read.cpp stack.cpp utils.cpp

consvm: $(HDRS) $(SRC) main.cpp
	g++ -g -o consvm main.cpp $(SRC)

run-tests: unit-tests
	./unit-tests --order-by=name

unit-tests: $(HDRS) $(SRC) test-harness.cpp
	g++ -o unit-tests test-harness.cpp $(SRC)

sizes: sizes.cpp consVM.h
	g++ -o sizes sizes.cpp
