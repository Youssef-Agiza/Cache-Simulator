SRC_DIR=./src
INC_DIR= ./headers
CC=g++
CCFLAGS= -c -I$(INC_DIR)/
OBJECTS= ./Random.obj ./SetAssociativeCache.obj ./cacheSim.obj ./Utilities.obj

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

clean:
	rm $(OBJECTS)
