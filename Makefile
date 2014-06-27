
CC=c++
CFLAGS=-c -Wall
LDFLAGS= -lmosquittopp -lPocoUtil -lPocoFoundation
OBJ=obj
BIN=bin
SOURCES=NinjaTtyDaemon.cpp MQTTClient.cpp tasks.cpp
OBJECTS=$(addprefix $(OBJ)/,$(SOURCES:.cpp=.o))

EXECUTABLE=$(BIN)/ninjatty

.PHONY: all
.PHONY: clean
.PHONY: build

all: $(SOURCES) main $(EXECUTABLE)

main: | $(OBJ)
	$(CC) $(CFLAGS) main.cpp -o $(OBJ)/main.o
	
$(EXECUTABLE): $(OBJECTS) $(OBJ)/main.o | $(BIN)
	$(CC) $(OBJECTS) $(OBJ)/main.o -o $@ $(LDFLAGS)

$(OBJECTS): $(OBJ)/%.o: %.cpp | %.h $(OBJ)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)

$(OBJ):
	-mkdir $(OBJ)

$(BIN):
	-mkdir $(BIN)
