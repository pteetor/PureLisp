HDRS = consVM.h

SRC = atom.cpp build_globals.cpp cell.cpp cons.cpp eval.cpp heap.cpp interp_code.cpp read.cpp stack.cpp strings.cpp utils.cpp

consvm: $(HDRS) $(SRC) main.cpp
	g++ -g -o consvm main.cpp $(SRC)

run-tests: unit-tests
	./unit-tests --order-by=name

unit-tests: $(HDRS) $(SRC) test-harness.cpp
	g++ -o unit-tests test-harness.cpp $(SRC)

sizes: sizes.cpp consVM.h
	g++ -o sizes sizes.cpp
