SRCS := $(wildcard ./src/*.cpp ./src/*.c)
OS := $(shell $(CC) -dM -E - < /dev/null)

ifneq ($(findstring __APPLE__, $(OS)),)
	PLATFORM := macOS
else ifneq ($(findstring _WIN32, $(OS)),)
	PLATFORM := Windows
	mingw := no
	ifneq ($(findstring __MINGW, $(OS)),)
		mingw := yes
	endif
else ifneq ($(findstring __linux__, $(OS)),)
	PLATFORM := Linux
endif

all: game

game: $(SRCS)
	mkdir -p build/
ifeq ($(PLATFORM),macOS)
	cp -r fonts/ build/fonts
	cp -r textures/ build/textures
	cp -r maps/ build/maps
else
	cp -r fonts/ textures/ maps/ build/
endif

ifeq ($(PLATFORM),Linux)
	g++ $(SRCS) -Iinclude -o build/chaser -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(PLATFORM),macOS)
	clang++ $(SRCS) -Iinclude $$(pkg-config --libs --cflags raylib) -o build/chaser
else ifeq ($(PLATFORM),Windows)
	g++ $(SRCS) -o build/chaser.exe -Iinclude -Llib -lraylib -lgdi32 -lwinmm -lopengl32
endif

run:
ifeq ($(PLATFORM),Windows)
ifneq ($(findstring yes, $(mingw)))
	cd build && ./chaser.exe
else
	cd build && .\chaser.exe
endif
else
	cd build && ./chaser
endif

runarm:
	cd build/arm && ./chaser

arm: $(SRCS)
	mkdir -p build/arm
	cp -r fonts/ textures/ maps/ build/arm
	aarch64-linux-gnu-g++ $(SRCS) -Iinclude -o build/arm/chaser -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -rf build/

.PHONY: all game clean arm
