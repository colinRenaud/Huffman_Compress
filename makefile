################ Variables ###################
CXX = gcc
CXX_FLAGS = -Wall -ansi -pedantic -std=c99

SOURCES = src/huffman.c src/Compress.c
HEADERS = $(SOURCES:.cpp=.h)
OBJECTS = $(SOURCES:.cpp=.o)
PROGRAM = huf
OPTIMIZING = optimize

########### CIBLES ################
all: $(PROGRAM)
clean:
	rm *.o
	rm $(PROGRAM)

############## Compilation #################

$(PROGRAM): $(OBJECTS)
	@echo "No optimisation "
	$(CXX) $(CXX_FLAGS) -g  $(OBJECTS) -o $(PROGRAM)

$(OPTIMIZING): $(OBJECTS)
	@echo "optimize the programm with O3 flags"
	$(CXX) $(CXX_FLAGS) -O3 $(OBJECTS) -o $(PROGRAM)

## $@ : dependances , $< PROGRAM /  première dependance
%.o: %.cpp
	$(CXX) -c $(CXX_FLAGS) $< -o $@
