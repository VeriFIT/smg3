TESTS := SMGObjectTest SMGRegionTest

% : %.cxx
	$(CXX) --std=c++11 $^ -I../../src/objects -I../../src/exceptions -L../../src -lsmg -lgtest -lgtest_main -o $@ -fsanitize=leak -fsanitize=undefined -fsanitize=address

tests: export LD_LIBRARY_PATH=../../src
tests: $(TESTS)
	$(foreach test,$(TESTS), ./$(test);)

clean:
	rm -f $(TESTS)
