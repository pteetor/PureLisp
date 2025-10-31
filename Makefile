HDRS = consVM.h

SRC = atom.cpp cell.cpp cons.cpp main.cpp read.cpp utils.cpp

consvm: $(HDRS) $(SRC)
	g++ -g -o consvm $(SRC)

sizes: sizes.cpp consVM.h
	g++ -o sizes sizes.cpp
