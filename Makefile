
CC = g++
SOURCE_FILES = src/*.cpp

.PHONY: all
all:
	$(CC) -c $(SOURCE_FILES) -IC:/SFML-2.6.1/include -Iinclude
	$(CC) *.o -o main -LC:/SFML-2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: clean
clean:
	rm main.exe