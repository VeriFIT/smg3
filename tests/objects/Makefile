TESTS := SMGObjectTest

% : %.cxx
	$(CXX) --std=c++11 $^ -I../../src/objects/ -L../../src -lsmg -lgtest -lgtest_main -o $@

tests: export LD_LIBRARY_PATH=../../src
tests: $(TESTS)
	$(foreach test,$(TESTS), ./$(test))

clean:
	rm -f $(TESTS)
