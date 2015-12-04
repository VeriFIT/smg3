export CXXFLAGS := -fPIC --std=c++11 -I. -I../exceptions

all: libsmg.so

libsmg.so: objects_o exceptions_o
	$(CXX) -shared $(wildcard objects/*.o exceptions/*.o) -o $@

objects_o:
	make -C objects

exceptions_o:
	make -C exceptions

clean:
	make -C objects $@
	make -C exceptions $@
	rm -f libsmg.so
