TESTS := SMGEdgePointsToTest SMGValueTest SMGEdgeHasValueTest

% : %.cc
	$(CXX) --std=c++11 $^ -I../../src/graphs -I../../src/objects -I../../src/exceptions -I../../src/types -L../../src -lsmg -lgtest -lgtest_main -o $@ -fsanitize=address -fsanitize=leak -fsanitize=undefined -fno-sanitize-recover=leak,undefined,address

tests: export LD_LIBRARY_PATH=../../src
tests: $(TESTS)
	$(foreach test,$(TESTS), ./$(test);)

clean:
	rm -f $(TESTS)
