COMPILER = g++

FLAGS = -g -fopenmp -O3 -std=c++11


SAMPLE = ./sample
UNITTEST = ./Test
BIN = ./bin
SRC_SAMPLE = $(wildcard $(SAMPLE)/*.cpp)
SAMPLE_TARGET = $(SRC_SAMPLE:$(SAMPLE)%=$(BIN)%)
SRC_UNITTEST = $(wildcard $(UNITTEST)/*.cpp)
UNITTEST_TARGET = $(SRC_UNITTEST:$(UNITTEST)%=$(BIN)%)

BatchLayout: $(UNITTEST_TARGET:.cpp=)

$(BIN)/%: $(SAMPLE)/%.cpp
	mkdir -p $(BIN)
	mkdir -p datasets/output
	$(COMPILER) $(FLAGS) $(INCLUDE) -o $@ $^ -DCPP ${TOCOMPILE} ${LIBS}

$(BIN)/%: $(UNITTEST)/%.cpp
	mkdir -p $(BIN)
	mkdir -p datasets/output
	$(COMPILER) $(FLAGS) $(INCLUDE) -o $@ $^ -DCPP ${TOCOMPILE} ${LIBS}
		
clean:
	rm -rf ./bin/*
