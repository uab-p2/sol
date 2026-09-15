# Build makefile for a quest solution. Packed into the coderunner too, where
# gradescope's run_autograder uses it to build the submission it grades.
# Run `luna gradescope pack <solution>` to (re)generate that coderunner.
BUILD_DIR := build
CMAKE_CACHE := $(BUILD_DIR)/CMakeCache.txt

.PHONY: all test clean

all: $(CMAKE_CACHE)
	cmake --build $(BUILD_DIR) --parallel

$(CMAKE_CACHE): CMakeLists.txt
	cmake -S . -B $(BUILD_DIR)

test: all

clean:
	rm -rf build main test *_main *_test cmake-* out *.exe *.pdb *.ilk .vs results.json coderunner.zip
