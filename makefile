#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

# bool flags

boost = -lboost_iostreams -lboost_system -lboost_filesystem -lpthread -lboost_thread

# normal flags

flag = -std=c++17 -w -lpthread

#SDL2 flags

SDL2 = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net

#This is the target that compiles our executable
all : $(OBJS) 
	g++ $(OBJS) $(flag) $(SDL2) $(boost) -o $(OBJ_NAME)


#DEBUGGING PURPOSE
DEBUG = practice.cpp

#specifies the name of debug file
DEBUG_NAME = practice

# created for local debugging purposes
debug: $(DEBUG)
	g++ $(DEBUG) $(flag) $(SDL2) $(boost) -o $(DEBUG_NAME)