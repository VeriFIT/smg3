export CXXFLAGS := -fPIC --std=c++11

all: libsmg.so

libsmg.so: objects_o
	$(CXX) -shared $(wildcard objects/*.o) -o $@

objects_o:
	make -C objects

clean:
	make -C objects $@
	rm -f libsmg.so
