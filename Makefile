CC = g++
CFLAGS = -std=c++11 -Wall -I/usr/include/freetype2 -g
LIBS = -L./Bbop-Library -lbbop -lglfw3 -lGLEW -lGL -lfreetype -lLDtkLoader -lIrrKlang

SRCS = main.cpp src/engine/gameCharacter.cpp src/game/player.cpp src/game/game.cpp src/engine/member.cpp src/engine/gun.cpp src/engine/bullet.cpp
OBJS = $(SRCS:.cpp=.o)

GREEN = \033[0;32m
CYAN = \033[0;36m
PURPLE = \033[0;35m
NC = \033[0m

all: final.exe

final.exe: $(OBJS)
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
