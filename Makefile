HDRS = consVM.h

SRC = atom.cpp build_globals.cpp cell.cpp cons.cpp eval.cpp main.cpp read.cpp stack.cpp utils.cpp

consvm: $(HDRS) $(SRC)
	g++ -g -o consvm $(SRC)

sizes: sizes.cpp consVM.h
	g++ -o sizes sizes.cpp
