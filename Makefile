.PHONY: tests

all:
	make -C src all

tests:
	make -C tests tests

clean:
	make -C src clean
	make -C tests clean
