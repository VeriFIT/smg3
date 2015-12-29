export CXXFLAGS := -fPIC --std=c++11 -I. -I../exceptions -I../objects -I../types
all: libsmg.so

libsmg.so: objects_o exceptions_o graphs_o types_o
	$(CXX) -shared $(wildcard objects/*.o exceptions/*.o graphs/*.o types/*.o) -o $@

objects_o:
	make -C objects

exceptions_o:
	make -C exceptions

graphs_o:
	make -C graphs

types_o:
	make -C types

clean:
	make -C objects $@
	make -C exceptions $@
	make -C types $@
	rm -f libsmg.so
