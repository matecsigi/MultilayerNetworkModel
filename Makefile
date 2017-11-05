SRCDIR := src
BUILDDIR := build
GENERATE-SOURCES := build/GenerateMain.o
SIMULATE-SOURCES := build/SimulateMain.o
TEST-SOURCES := build/test_Network_addEdge.o

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g  -Wall
#LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
LIB := -lboost_unit_test_framework
INC := -I include

# $(TARGET): $(OBJECTS)
# 	@echo " Linking..."
# 	@echo " g++ $^ -o $(TARGET) $(LIB)"; g++ $^ -o $(TARGET) $(LIB)

all: bin/generate bin/simulate bin/test

bin/generate: $(filter-out $(SIMULATE-SOURCES) $(TEST-SOURCES),$(OBJECTS))
	@echo " g++ $(filter-out $(SIMULATE-SOURCES) $(TEST-SOURCES),$(OBJECTS)) -o bin/generate $(LIB)"; g++ $(filter-out $(SIMULATE-SOURCES) $(TEST-SOURCES),$(OBJECTS)) -o bin/generate $(LIB)

bin/simulate: $(filter-out $(GENERATE-SOURCES) $(TEST-SOURCES), $(OBJECTS))
	@echo " g++ $(filter-out $(GENERATE-SOURCES) $(TEST-SOURCES), $(OBJECTS)) -o bin/simulate $(LIB)"; g++ $(filter-out $(GENERATE-SOURCES) $(TEST-SOURCES),$(OBJECTS)) -o bin/simulate $(LIB)

bin/test: $(filter-out $(GENERATE-SOURCES) $(SIMULATE-SOURCES), $(OBJECTS))
	@echo " g++ $(filter-out $(GENERATE-SOURCES) $(SIMULATE-SOURCES), $(OBJECTS))-o bin/test $(LIB)"; g++ $(filter-out $(GENERATE-SOURCES) $(SIMULATE-SOURCES), $(OBJECTS)) -o bin/test $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " g++ $(CFLAGS) $(INC) -c -o $@ $<"; g++ $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) bin/generate bin/simulate bin/test"; $(RM) -r $(BUILDDIR) bin/generate bin/simulate bin/test

.PHONY: clean
