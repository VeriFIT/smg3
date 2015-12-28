TESTS := SMGEdgePointsToTest SMGValueTest

% : %.cc
	$(CXX) --std=c++11 $^ -I../../src/graphs -I../../src/objects -I../../src/exceptions -L../../src -lsmg -lgtest -lgtest_main -o $@

tests: export LD_LIBRARY_PATH=../../src
tests: $(TESTS)
	$(foreach test,$(TESTS), ./$(test);)

clean:
	rm -f $(TESTS)
