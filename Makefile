OBJS=\
	ut.o \
	ut_minmaxheap.o \
	ut_bounded_priority_queue.o
OBJ_OPT_FILES=$(patsubst %.o,obj/opt/%.o,$(OBJS))
OBJ_DBG_FILES=$(patsubst %.o,obj/dbg/%.o,$(OBJS))

DEP_FILES=$(patsubst %.o,dep/%.d,$(OBJS))

INCLUDE_DIRS=-Iinclude -I/opt/boost
LIBS=-L/opt/boost/stage/lib -lboost_unit_test_framework -lrt
DEFINE=-DBOOST_TEST_DYN_LINK

ifeq ($(TOOLS),intel)
CXX=icpc
DEP=icpc
LINK=icpc
DEPFLAGS=-std=c++0x
CXXFLAGS_DBG=-O0 -g -std=c++0x -Wall -Wsign-compare -Wcheck -wd279
CXXFLAGS_OPT=-O2 -xHost -std=c++0x -Wall -Wsign-compare -Wcheck -wd279
else
ifeq ($(TOOLS),clang)
CXX=clang++
DEP=clang++
LINK=clang++
DEPFLAGS=-std=c++0x
CXXFLAGS_DBG=-O0 -g -std=c++0x -Wall
CXXFLAGS_OPT=-O2 -march=native -Wall
else
CXX=g++
DEP=g++
LINK=g++
DEPFLAGS=-std=c++0x
CXXFLAGS_DBG=-O0 -g -std=c++0x -Wall -Wsign-compare
CXXFLAGS_OPT=-O2 -march=native -std=c++0x -Wall -Wsign-compare
endif
endif

all: bin/sway_test_opt bin/sway_test_dbg

opt: bin/sway_test_opt

dbg: bin/sway_test_dbg

bin/sway_test_opt: $(OBJ_OPT_FILES)
	$(LINK) $(OBJ_OPT_FILES) $(LIBS) -o $@

bin/sway_test_dbg: $(OBJ_DBG_FILES)
	$(LINK) $(OBJ_DBG_FILES) $(LIBS) -o $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP_FILES)
endif

dep/%.d: src/%.cpp include/*.h
	$(DEP) $(DEPFLAGS) \
	-MM $(patsubst dep/%.d,src/%.cpp,$@) \
	-MT $(patsubst dep/%.d,obj/opt/%.o,$@) \
	-MT $(patsubst dep/%.d,obj/dbg/%.o,$@) \
	$(INCLUDE_DIRS) > $@

obj/dbg/%.o: src/%.cpp
	$(CXX) $(DEFINE) $(INCLUDE_DIRS) -c $(CXXFLAGS_DBG) $< -o $@

obj/opt/%.o: src/%.cpp
	$(CXX) $(DEFINE) $(INCLUDE_DIRS) -c $(CXXFLAGS_OPT) $< -o $@

clean:
	rm -f bin/*
	rm -f dep/*.d
	rm -f obj/opt/*.o
	rm -f obj/dbg/*.o

.PHONY: clean
