
CC=c++
CFLAGS=-c -Wall
LDFLAGS= -lmosquittopp -lPocoUtil -lPocoFoundation
OBJ=obj
BIN=bin
SOURCES=main.cpp NinjaTtyDaemon.cpp MQTTClient.cpp tasks.cpp
OBJECTS=$(addprefix $(OBJ)/,$(SOURCES:.cpp=.o))

EXECUTABLE=$(BIN)/ninjatty

.PHONY: all
.PHONY: clean
.PHONY: build

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) | $(BIN)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS): $(OBJ)/%.o: %.cpp | $(OBJ)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)

$(OBJ):
	-mkdir $(OBJ)

$(BIN):
	-mkdir $(BIN)
