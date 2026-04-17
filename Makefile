CXX = g++
CXXFLAGS = -pedantic-errors -Wall -Wextra -std=c++11 -Iinclude
TARGET = loopbreak
OBJS = main.o game.o map.o player.o save.o utils.o item.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp include/game.h
	$(CXX) $(CXXFLAGS) -c main.cpp

game.o: src/game.cpp include/game.h include/save.h include/utils.h
	$(CXX) $(CXXFLAGS) -c src/game.cpp -o game.o

map.o: src/map.cpp include/map.h include/room.h include/utils.h
	$(CXX) $(CXXFLAGS) -c src/map.cpp -o map.o

player.o: src/player.cpp include/player.h
	$(CXX) $(CXXFLAGS) -c src/player.cpp -o player.o

save.o: src/save.cpp include/save.h include/game.h include/utils.h
	$(CXX) $(CXXFLAGS) -c src/save.cpp -o save.o

utils.o: src/utils.cpp include/utils.h
	$(CXX) $(CXXFLAGS) -c src/utils.cpp -o utils.o

item.o: src/item.cpp include/item.h
	$(CXX) $(CXXFLAGS) -c src/item.cpp -o item.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJS) savegame.txt
