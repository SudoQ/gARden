CXX = g++
CXXFLAGS = -Wall -std=c++11
LDLIBS = -lglfw -lGL -lGLU -DGLEW_STATIC -lGLEW -lfreeimage

OBJ_DIR = bin
LIB_DIR = -L/usr/lib
INC_DIR = -I/usr/include

SOURCE = main.cpp
OBJECTS = ${SOURCE:%.cpp=$(OBJ_DIR)/%.o}
EXECUTABLE = main.exe

all: init $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIB_DIR) -o $@ $(OBJECTS) $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

init:
	@mkdir -p "$(OBJ_DIR)"

clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)
