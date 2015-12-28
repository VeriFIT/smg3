.PHONY: tests

all:
	make -C src all

tests: all
	make -C tests tests

clean:
	make -C src clean
	make -C tests clean
