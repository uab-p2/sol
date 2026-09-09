BUILD_DIR := build
CMAKE_CACHE := $(BUILD_DIR)/CMakeCache.txt

.PHONY: all clean

all: $(CMAKE_CACHE)
	cmake --build $(BUILD_DIR)

$(CMAKE_CACHE): CMakeLists.txt
	cmake -S . -B $(BUILD_DIR)

clean:
	rm -rf build main test *_main *_test cmake-* out *.exe *.pdb *.ilk .vs 
