APP_NAME = Breakout
BUILD_DIR = ./bin
C_FILES = ./src/*.c ./src/*.cpp
CFLAGS = -Wall -g -std=c++17

APP_DEFINES:=
APP_INCLUDES:= -I/usr/local/include -L/usr/local/lib -lglfw -lassimp -lfreetype -framework GLUT -framework OpenGL -Wl,-rpath,/usr/local/lib

build:
	clang++ $(CFLAGS) $(C_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_INCLUDES)
