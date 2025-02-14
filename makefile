#uncomment the below if using mingw
# SHELL = cmd

APP_NAME = program

C_FLAGS = -O3 -g0 -Wpedantic -pedantic-errors  -Wall  -Wno-unused-variable -Wno-unused-function

# IF additional checker flags
C_FLAGS += -Wstrict-overflow=5 -Wwrite-strings -Wconversion -Wunused-variable -Wunused-function -Wformat 

# IF linux:
C_FLAGS += -fsanitize=address,undefined -fsanitize-undefined-trap-on-error -Wconversion -fanalyzer -lm

C_COMPILER = gcc

 
C_SOURCE_FOLDER = src
C_INCLUDES_DIR = includes
TEST_FOLDER = tests

# adding header directories
C_FLAGS += -iquote $(C_INCLUDES_DIR) -iquote $(TEST_FOLDER)

C_SRC_FILES = $(wildcard $(C_SOURCE_FOLDER)/*.c)
C_SRC_FILES += $(wildcard $(TEST_FOLDER)/*.c)
C_SRC_FILES += main.c
C_HEADERS =  $(wildcard $(C_INCLUDES_DIR)/*.h)
C_OBJ_FILES = $(patsubst %.c, %.o, $(C_SRC_FILES))

# adding Makefile itself to the rule prerequisites will make sure everything is recompiled if makefile content gets changed
$(APP_NAME).exe: $(C_OBJ_FILES) $(C_HEADERS) Makefile
	$(C_COMPILER) $(C_FLAGS) $(C_OBJ_FILES) -o $(APP_NAME).exe

# for mingw
# clean:
# 	del /Q /F $(APP_NAME).exe
# 	del /Q /F $(C_SOURCE_FOLDER)\\*.o
# 	del /Q /F $(TEST_FOLDER)\\*.o
# 	del /Q /F main.o

# run: $(APP_NAME).exe
# 	$(APP_NAME).exe


%.o: %.c Makefile
	$(C_COMPILER) $(C_FLAGS) -c $< -o $@