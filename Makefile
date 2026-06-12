SRCS := $(wildcard ./src/*.cpp ./src/*.c)

all: game

game: $(SRCS)
	mkdir -p build/
	cp -r fonts/ textures/ maps/ build/
	g++ $(SRCS) -Iinclude -o build/chaser -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

run:
	cd build && ./chaser

runarm:
	cd build/arm && ./chaser

arm: $(SRCS)
	mkdir -p build/arm
	cp -r fonts/ textures/ maps/ build/arm
	aarch64-linux-gnu-g++ $(SRCS) -Iinclude -o build/chaser -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -rf build/

.PHONY: all game clean arm
