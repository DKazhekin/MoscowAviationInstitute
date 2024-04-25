GCC=g++
FLAGS=-std=c++17
LIB=-I/Users/deniskazhekin/DA/json/include
SOURCES=nb.cpp nb_helper.cpp
OBJ=out
INPUT_FILE=materials/train_xxl.txt
VALID_FILE=materials/valid_xxl.txt
STATS_FILE=stats.txt
OUTPUT_FILE=output.txt

.PHONY: compile learn classify clean

compile:
	$(GCC) $(FLAGS) $(LIB) -o $(OBJ) $(SOURCES)
learn:
	./$(OBJ) learn --input $(INPUT_FILE) --output $(STATS_FILE)
classify:
	./$(OBJ) classify --stats $(STATS_FILE) --input $(VALID_FILE) --output $(OUTPUT_FILE)
clean:
	rm -rf out output.txt stats.txt
