#genericky makefile for projects of Michal Kotoun <xkotou04@stud.fit.vutbr.cz>

#default target + phony targets

.PHONY: all build release debug clean pack run makerun exportrun r mr er dmakerun dmr

all: release

#functions

SRC_DIR_TO_FILES        = $(wildcard $(addsuffix /*,$(shell find $(1) -type d)))
SRC_DIR_TO_C_CODEFILES   = $(filter %.c,$(call SRC_DIR_TO_FILES, $(1) ) )
SRC_DIR_TO_CXX_CODEFILES = $(filter %.cc,$(call SRC_DIR_TO_FILES, $(1) ) )
SRC_DIR_TO_OBJFILES      = $(subst $(1),$(OBJ_DIR),$(patsubst %.c, %.o, $(call SRC_DIR_TO_C_CODEFILES, $(1)) ) $(patsubst %.cc, %.o, $(call SRC_DIR_TO_CXX_CODEFILES, $(1)) ) )
SRC_DIR_TO_DEPFILES      = $(subst $(1),$(OBJ_DIR),$(patsubst %.o, %.d, $(call SRC_DIR_TO_OBJFILES, $(1)) ))

#functions end

#-----------
#config me here
#look for  dependencies  sector too

#binary to run using run targets
BINARY_RUN := tests
#binaries to make
BINARIES :=
#tests to make
TESTS_BINS := tests
#lib runners to make
RUNNER_BINS := minimal_runner
#libraries to make
LIBRARIES := smg
#-----------
# to be created before build
BIN_DIR := bin
OBJ_DIR := obj

SRC_DIR := src
SRC_TESTS_DIR := tests
SRC_RUNNER_DIR := minimal_runner

#also as include dirs
SRC_DIRS := $(shell find $(SRC_DIR) -type d)
# to be created before build
SRC_TESTS_DIRS := $(shell find $(SRC_TESTS_DIR) -type d)
OBJ_DIRS := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRC_DIRS) $(SRC_TESTS_DIRS))
#-----------
ifeq ($(OS),Windows_NT)
	SHARED_SUFFIX=.dll
	STATIC_SUFIX=.lib
	BINARY_SUFFIX=.exe
	GLOBALFLAGS :=
	SANIT_FLAGS :=
	else
	LIB_PREFIX=lib
	SHARED_SUFFIX=.so
	STATIC_SUFIX=.a
	BINARY_SUFFIX=
	GLOBALFLAGS := -fPIC
	SANIT_FLAGS := -fsanitize=address -fsanitize=leak -fsanitize=undefined
endif
#-----------
print-%  : ; @echo $* = $($*)
#dependencies
library := smg
library_OBJFILES     := $(call SRC_DIR_TO_OBJFILES, $(SRC_DIR))
library_DEPFILES     := $(call SRC_DIR_TO_DEPFILES, $(SRC_DIR))

program := tests
program_OBJFILES     := $(call SRC_DIR_TO_OBJFILES, $(SRC_TESTS_DIR))
program_DEPFILES     := $(call SRC_DIR_TO_DEPFILES, $(SRC_TESTS_DIR))

runner := minimal_runner
runner_OBJFILES     := $(call SRC_DIR_TO_OBJFILES, $(SRC_RUNNER_DIR))
runner_DEPFILES     := $(call SRC_DIR_TO_DEPFILES, $(SRC_RUNNER_DIR))
#-----------
NDEBUG = -DNDEBUG
CC = cc
INCLUDEFLAGS = $(addprefix -I,$(SRC_DIRS))
# -fPIC
# GLOBALFLAGS := -fPIC #moved to the top, OS-dependent
CFLAGS_B = $(CFLAGS) $(GLOBALFLAGS) -std=c14 -Wall -pedantic -Wextra -Wconversion $(NDEBUG) $(INCLUDEFLAGS)
CXXFLAGS_B = $(CXXFLAGS) $(GLOBALFLAGS) -std=c++14 -Wall -pedantic -Wextra -Wconversion $(NDEBUG) $(INCLUDEFLAGS)
LDFLAGS := -L"$(shell pwd)/$(BIN_DIR)"
#LDFLAGS += -pthread
#ADD GTEST_HOME to linker and include flags if needed
ifdef GTEST_HOME
	LDFLAGS += -L"$(GTEST_HOME)" -L"$(GTEST_HOME)/bin" -L"$(GTEST_HOME)/lib"
	INCLUDEFLAGS += -I"$(GTEST_HOME)/include"
endif
#-----------
#not needed if we use "shell find"
# - objectfiles are complete with paths, we reuse them for sourcefiles
#vpath %.h $(SRC_DIRS)
#vpath %.c $(SRC_DIRS)
#-----------
BINARIES_ALL =      $(addprefix $(BIN_DIR)/,$(addsuffix $(BINARY_SUFFIX),$(BINARIES)))
TESTS_BINS_ALL =    $(addprefix $(BIN_DIR)/,$(addsuffix $(BINARY_SUFFIX),$(TESTS_BINS)))
RUNNER_BINS_ALL =    $(addprefix $(BIN_DIR)/,$(addsuffix $(BINARY_SUFFIX),$(RUNNER_BINS)))
LIBRARIES_PREFIX =  $(addprefix $(LIB_PREFIX),$(LIBRARIES))
LIBRARIES_SHARED =  $(addprefix $(BIN_DIR)/,$(addsuffix $(SHARED_SUFFIX),$(LIBRARIES_PREFIX)))
LIBRARIES_STATIC =  $(addprefix $(BIN_DIR)/,$(addsuffix $(STATIC_SUFIX),$(LIBRARIES_PREFIX)))
LIBRARIES_ALL =     $(LIBRARIES_SHARED) $(LIBRARIES_STATIC)
#-----------

release: CFLAGS := $(CFLAGS_B) -o3
release: CXXFLAGS := $(CXXFLAGS_B) -o3
release: build

debug: NDEBUG :=
debug: CFLAGS := $(CFLAGS_B) -g $(SANIT_FLAGS) -Werror
debug: CXXFLAGS := $(CXXFLAGS_B) -g $(SANIT_FLAGS) -Werror
debug: build

derr: NDEBUG :=
derr: CFLAGS := $(CFLAGS_B) -Werror -g
derr: CXXFLAGS := $(CXXFLAGS_B) -Werror -g
derr: build

build: dirs $(BINARIES_ALL) $(LIBRARIES_ALL)

tests: LDFLAGS := $(LDFLAGS) -l$(library) -lgtest -lgtest_main
tests: CFLAGS := $(CFLAGS_B) $(SANIT_FLAGS) -Werror
tests: CXXFLAGS := $(CXXFLAGS_B) $(SANIT_FLAGS) -Werror
tests: build-tests
tests: run-tests

build-tests: dirs $(TESTS_BINS_ALL)

run-tests:
	@echo Running tests $(TESTS_BINS_ALL)
	export LD_LIBRARY_PATH=$(shell pwd)/$(BIN_DIR) ; $(foreach app,$(TESTS_BINS_ALL), $(app) --gtest_catch_exceptions=0 || echo $$? ; )

runner: LDFLAGS := $(LDFLAGS) -l$(library)
runner: CFLAGS := $(CFLAGS_B) $(SANIT_FLAGS) -Werror
runner: CXXFLAGS := $(CXXFLAGS_B) $(SANIT_FLAGS) -Werror
runner: build-runner
runner: run-runner

build-runner: dirs $(RUNNER_BINS_ALL)

run-runner:
	export LD_LIBRARY_PATH=$(shell pwd)/$(BIN_DIR) ; $(foreach app,$(RUNNER_BINS_ALL), $(app) || echo $$? ; )

dirs:
	@mkdir -p $(BIN_DIR) $(OBJ_DIRS)

#------------

#for all files in, binaries all, test bins all , libs all create aproperiate program:objects binding
#dependencies - screws colors in VS Code
$(BIN_DIR)/$(LIB_PREFIX)$(library)$(SHARED_SUFFIX): $(library_OBJFILES)
$(BIN_DIR)/$(LIB_PREFIX)$(library)$(STATIC_SUFIX): $(library_OBJFILES)
$(BIN_DIR)/$(program)$(BINARY_SUFFIX): $(program_OBJFILES)
$(BIN_DIR)/$(runner)$(BINARY_SUFFIX): $(runner_OBJFILES)

-include $(library_DEPFILES) $(program_DEPFILES) $(runner_DEPFILES)

#------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(INCLUDEFLAGS) -MM -MF $(OBJ_DIR)/$*.d -MT $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(CXX) $(INCLUDEFLAGS) -MM -MF $(OBJ_DIR)/$*.d -MT $@ $<

#-----------

$(OBJ_DIR)/%.o: $(SRC_TESTS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(INCLUDEFLAGS) -MM -MF $(OBJ_DIR)/$*.d -MT $@ $<

$(OBJ_DIR)/%.o: $(SRC_TESTS_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(CXX) $(INCLUDEFLAGS) -MM -MF $(OBJ_DIR)/$*.d -MT $@ $<

#-----------

$(OBJ_DIR)/%.o: $(SRC_RUNNER_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(INCLUDEFLAGS) -MM -MF $(OBJ_DIR)/$*.d -MT $@ $<

$(OBJ_DIR)/%.o: $(SRC_RUNNER_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(CXX) $(INCLUDEFLAGS) -MM -MF $(OBJ_DIR)/$*.d -MT $@ $<

#-----------

#TODO: which flags use for linking?
$(BINARIES_ALL) $(TESTS_BINS_ALL) $(RUNNER_BINS_ALL):
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

#TODO: which flags use for linking?
$(LIBRARIES_SHARED):
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS)

$(LIBRARIES_STATIC):
	ar rcs $@ $^

#-----------

mr makerun: all
	$(BIN_DIR)/$(BINARY_RUN)$(BINARY_SUFFIX) || echo $$?

dmr dmakerun: debug
	$(BIN_DIR)/$(BINARY_RUN)$(BINARY_SUFFIX) || echo $$?

r run:
	$(BIN_DIR)/$(BINARY_RUN)$(BINARY_SUFFIX) || echo $$?

er exportrun:
	export LD_LIBRARY_PATH=$(shell pwd)/$(BIN_DIR) ; ./$(BIN_DIR)/$(BINARY_RUN)$(BINARY_SUFFIX) || echo $$?

clean:
	$(RM) -rf $(BIN_DIR)/* $(OBJ_DIR)/*

cd: clean debug

ce: clean derr

#pack: ;
#	rm -f ifj2015-xkotou04.zip && zip -r ifj2015-xkotou04.zip Makefile Doxyfile *.c *.cc *.h *.hh  README README.*


#----------
#dependencies samples

#h2o_objects = main.o atoms.o utils.o
#h2o_headers = h2o.h debug_macros.h
#h2o: $(h2o_objects)
#$(h2o_objects): $(h2o_headers)

#tail: tail.o rarray.o str.o error.o
#wordcount: wordcount.c io.o print_function.o error.o $(LIB_PREFIX)htable$(SHARED_SUFFIX)
##wordcount: LDFLAGS =  -L. -lhtable
#je mozno linkovat s libhtable primo vypisem do "file listu" pokud je ve stejnem adresari
#wordcount-static: wordcount.c io.o print_function.o error.o $(LIB_PREFIX)htable$(STATIC_SUFIX)
#htable$(SHARED_SUFFIX): $(patsubst %.c,%.o,$(wildcard htab_*.c)) hash_function.o
#htable$(STATIC_SUFIX): $(patsubst %.c,%.o,$(wildcard htab_*.c)) hash_function.o
#----------
