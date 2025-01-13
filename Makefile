CC = g++
CFLAGS = -std=c++11 -Wall -I/usr/include/freetype2 -DDEBUG
LIBS = -L./Bbop-Library -lbbop -lglfw3 -lGLEW -lGL -lfreetype -lLDtkLoader 

SRCS = main.cpp gameCharacter.cpp player.cpp game.cpp member.cpp
OBJS = $(SRCS:.cpp=.o)

GREEN = \033[0;32m
CYAN = \033[0;36m
PURPLE = \033[0;35m
NC = \033[0m

all: final

final: $(OBJS)
	@(cd Bbop-Library && make -j && make lib)
	@echo -e "$(PURPLE)Linking compiled files $(NC)"
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo -e "$(GREEN)Linking done ! $(NC)"

%.o: %.cpp
	@echo -e "$(GREEN)Compiling $(CYAN)[$@] $(NC)"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo -e "$(PURPLE)Deleting all objects files and final$(NC)"
	@rm -f final $(OBJS)
	@(cd Bbop-Library && make clean)
