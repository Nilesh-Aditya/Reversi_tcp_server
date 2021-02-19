#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS) 
	g++ $(OBJS) -std=c++17 -w -lSDL2 -lSDL2_image -lSDL2_ttf -lboost_iostreams -lboost_system -lboost_filesystem -o $(OBJ_NAME)