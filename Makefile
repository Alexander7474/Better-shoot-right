#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#
#CXX = g++
CXX = clang++

EXE = final.exe
SRC_DIR = src/
SOURCES = main.cpp $(wildcard $(SRC_DIR)engine/*.cpp) $(wildcard $(SRC_DIR)game/*.cpp)
SOURCES += $(SRC_DIR)imgui/imgui.cpp $(SRC_DIR)imgui/imgui_demo.cpp $(SRC_DIR)imgui/imgui_draw.cpp $(SRC_DIR)imgui/imgui_tables.cpp $(SRC_DIR)imgui/imgui_widgets.cpp
SOURCES += $(SRC_DIR)backends/imgui_impl_glfw.cpp $(SRC_DIR)backends/imgui_impl_opengl3.cpp
OBJS = $(SOURCES:.cpp=.o)
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

## INCLUDE FLAGS
CXXFLAGS = -I$(SRC_DIR)/imgui -I$(SRC_DIR)/backends -I/usr/include/freetype2
## COMPILER FLAGS
CXXFLAGS += -g -Wall -Wno-delete-non-virtual-dtor -Wformat -fdiagnostics-color=always 
## DEBUG FLAGS
CXXFLAGS += -DIMGUI_DEBUG

LIBS = -L./Bbop-2D -lbbop -lglfw -lGLEW -lGL -lfreetype -lLDtkLoader -lbox2d -lSDL2 -lSDL2_mixer

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(SRC_DIR)/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(SRC_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
