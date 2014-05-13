EXEDIR := scripts
OBJDIR := bin
SRCDIR := src
INCDIR := inc
MAKEDIR := bin

CXX := $(shell root-config --cxx)
EXTRA_WARNINGS := -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Winit-self -Winvalid-pch -Wlong-long -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wpointer-arith -Wredundant-decls -Wstack-protector -Wswitch-default -Wswitch-enum -Wundef -Wunused -Wvariadic-macros -Wwrite-strings -Wabi -Wctor-dtor-privacy -Wnon-virtual-dtor -Wstrict-null-sentinel -Wsign-promo -Wsign-compare #-Wunsafe-loop-optimizations -Wfloat-equal -Wsign-conversion -Wunreachable-code
CXXFLAGS := -isystem $(shell root-config --incdir) -Wall -Wextra -pedantic -Werror -Wshadow -Woverloaded-virtual -Wold-style-cast $(EXTRA_WARNINGS) $(shell root-config --cflags) -O2 -I $(INCDIR)
LD := $(shell root-config --ld)
LDFLAGS := $(shell root-config --ldflags)
LDLIBS := $(shell root-config --libs) -lMinuit

vpath %.cpp $(SRCDIR)
vpath %.hpp $(INCDIR)
vpath %.o $(OBJDIR)
vpath %.exe $(EXEDIR)
vpath %.d $(MAKEDIR)

# Add new executables to this list
all: make_reduced_tree.exe

# List any object files your executable oneed to be linked with
$(EXEDIR)/make_reduced_tree.exe: make_reduced_tree.o reduced_tree_maker.o timer.o weights.o in_json_2012.o math.o pu_constants.o event_number.o weights.o cfa.o event_handler.o

-include $(addsuffix .d,$(addprefix $(MAKEDIR)/,$(notdir $(basename $(wildcard $(SRCDIR)/*.cpp)))))
-include $(MAKEDIR)/cfa.d

$(MAKEDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MG -MF $@ $< 
	sed -i'' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# This is a bit ugly. Shouldn't need the dependency explicitly.
$(EXEDIR)/%.exe: $(OBJDIR)/%.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# cfa.cpp and cfa.hpp need special treatment. Probably cleaner ways to do this.
$(SRCDIR)/cfa.cpp $(INCDIR)/cfa.hpp: dummy_cfa.all
.SECONDARY: dummy_cfa.all
dummy_cfa.all: $(EXEDIR)/generate_cfa_class.exe example_root_file.root
	./$< $(word 2,$^)
.PRECIOUS: generate_cfa_class.o

.DELETE_ON_ERROR:

.PHONY: clean

clean:
	-rm -rf $(EXEDIR)/*.exe $(OBJDIR)/*.o $(MAKEDIR)/*.d $(SRCDIR)/cfa.cpp $(INCDIR)/cfa.hpp *.exe *.o *.d *.out
	./scripts/remove_backups.sh
