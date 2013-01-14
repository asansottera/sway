# Copyright (c) 2011, Andrea Sansottera
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#  - Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#  - Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

OBJS=\
	ut.o \
	ut_minmaxheap.o \
	ut_bounded_priority_queue.o\
	ut_priority_dqueue.o\
	ut_configuration.o
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
DEPFLAGS=
CXXFLAGS_DBG=-O0 -g -Wall -Wsign-compare -wd279
CXXFLAGS_OPT=-O2 -xHost -Wall -Wsign-compare -wd279
else
ifeq ($(TOOLS),clang)
CXX=clang++
DEP=clang++
LINK=clang++
DEPFLAGS=
CXXFLAGS_DBG=-O0 -g -Wall -Wno-unused-variable
CXXFLAGS_OPT=-O2 -march=native -Wall -Wno-unused-variable
else
CXX=g++
DEP=g++
LINK=g++
DEPFLAGS=
CXXFLAGS_DBG=-O0 -g -Wall -Wsign-compare
CXXFLAGS_OPT=-O2 -march=native -Wall -Wsign-compare
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

dep/%.d: src/%.cpp include/**/*.hpp
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
