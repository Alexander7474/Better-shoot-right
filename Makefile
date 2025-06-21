CC = g++
CFLAGS = -std=c++11 -Wall -I/usr/include/freetype2 -g
LIBS = -L./Bbop-2D -lbbop -lglfw -lGLEW -lGL -lfreetype -lLDtkLoader 

SRCS = main.cpp src/engine/gameCharacter.cpp src/game/bot.cpp src/game/trooper.cpp src/game/neutre.cpp  src/game/player.cpp src/game/game.cpp src/engine/member.cpp src/engine/gun.cpp src/engine/bullet.cpp src/engine/crossair.cpp
OBJS = $(SRCS:.cpp=.o)

GREEN = \033[0;32m
CYAN = \033[0;36m
PURPLE = \033[0;35m
NC = \033[0m

all: final.exe

final.exe: $(OBJS)
	@(cd Bbop-2D && make -j && make lib)
	@echo -e "$(PURPLE)Linking compiled files $(NC)"
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo -e "$(GREEN)Linking done ! $(NC)"

%.o: %.cpp
	@echo -e "$(GREEN)Compiling $(CYAN)[$@] $(NC)"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo -e "$(PURPLE)Deleting all objects files and final$(NC)"
	@rm -f final.exe $(OBJS)
	@(cd Bbop-2D && make clean)

map:
	@make
	@$(CC) $(CFLAGS) -c liveMap.cpp
	@$(CC) $(CFLAGS) liveMap.o $(LIBS) -o map.exe
	@rm liveMap.o

