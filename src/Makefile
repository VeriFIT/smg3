export CXXFLAGS := -fPIC --std=c++11 -I. -I../exceptions -I../objects
all: libsmg.so

libsmg.so: objects_o exceptions_o graphs_o
	$(CXX) -shared $(wildcard objects/*.o exceptions/*.o graphs/*.o) -o $@

objects_o:
	make -C objects

exceptions_o:
	make -C exceptions

graphs_o:
	make -C graphs

clean:
	make -C objects $@
	make -C exceptions $@
	rm -f libsmg.so
