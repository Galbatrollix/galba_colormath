SHELL = cmd

APP_NAME = program

C_FLAGS = -O3 -g0 -Wpedantic -pedantic-errors  -Wall -Wno-unused-variable -Wno-unused-function -Wno-format
C_COMPILER = gcc

 
C_SOURCE_FOLDER = src

C_SRC_FILES = $(wildcard $(C_SOURCE_FOLDER)/*.c)
C_SRC_FILES += main.c
C_HEADERS =  $(wildcard $(C_SOURCE_FOLDER)/*.h)
C_OBJ_FILES = $(patsubst %.c, %.o, $(C_SRC_FILES))

# adding Makefile itself to the rule prerequisites will make sure everything is recompiled if makefile content gets changed
$(APP_NAME).exe: $(C_OBJ_FILES) $(C_HEADERS) Makefile
	$(C_COMPILER) $(C_FLAGS) $(C_OBJ_FILES) -o $(APP_NAME).exe


clean:
	del /Q /F $(APP_NAME).exe
	del /Q /F $(C_SOURCE_FOLDER)\\*.o

run: $(APP_NAME).exe
	$(APP_NAME).exe

# target is any .o file and its prerequisite is a .c file
# -c means that gcc should compile but NOT link the files
# https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean
# $< means <name of fist prerequisite> here %.c
# $@ means <name of target> here %.o
%.o: %.c Makefile
	$(C_COMPILER) $(C_FLAGS) -c $< -o $@