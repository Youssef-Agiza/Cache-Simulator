SRC_DIR=./src
INC_DIR= ./headers
CC=g++
CCFLAGS= -c -I$(INC_DIR)/
OBJECTS= ./Random.obj ./SetAssociativeCache.obj ./cacheSim.obj ./Utilities.obj


all: cacheSim.exe outputDir

cacheSim.exe: $(OBJECTS)
	$(CC) $(OBJECTS) -o cacheSim.exe


Random.obj: $(SRC_DIR)/Random.cpp $(INC_DIR)/Random.h
	$(CC) $(SRC_DIR)/Random.cpp $(CCFLAGS) -o Random.obj


SetAssociativeCache.obj: $(SRC_DIR)/SetAssociativeCache.cpp $(INC_DIR)/SetAssociativeCache.h $(INC_DIR)/PCH.h
	$(CC) $(SRC_DIR)/SetAssociativeCache.cpp $(CCFLAGS) -o SetAssociativeCache.obj


cacheSim.obj: $(SRC_DIR)/cacheSim.cpp $(INC_DIR)/PCH.h $(INC_DIR)/SetAssociativeCache.h $(INC_DIR)/Random.h 
	$(CC) $(SRC_DIR)/cacheSim.cpp $(CCFLAGS) -o cacheSim.obj

Utilities.obj: $(SRC_DIR)/Utilities.cpp $(INC_DIR)/Utilities.h $(INC_DIR)/PCH.h $(INC_DIR)/SetAssociativeCache.h
	$(CC) $(SRC_DIR)/Utilities.cpp $(CCFLAGS) -o Utilities.obj

plotOutputs: 
	for file in ./outputs/Exp#1*; do \
		python3 plotter/plotter.py 1 $$file;\
	done
	for file in ./outputs/Exp#2*; do \
		python3 plotter/plotter.py 2 $$file;\
	done

outputDir:
		mkdir -p outputs plots
 
clean:
	rm $(OBJECTS) 
