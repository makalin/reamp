.PHONY: build clean run install-deps

BUILD_DIR = build
CMAKE_BUILD_TYPE = Release

build:
	@echo "Building ReAmp..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) ..
	@cd $(BUILD_DIR) && cmake --build . -j$(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
	@echo "Build complete! Binary: $(BUILD_DIR)/bin/reamp"

build-debug:
	@$(MAKE) build CMAKE_BUILD_TYPE=Debug

clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete"

run: build
	@$(BUILD_DIR)/bin/reamp

install-deps:
	@echo "Installing dependencies..."
	@if command -v brew >/dev/null 2>&1; then \
		brew install sdl2 sdl2_image lua cmake; \
	elif command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update && sudo apt-get install -y libsdl2-dev libsdl2-image-dev liblua5.3-dev cmake build-essential; \
	elif command -v pacman >/dev/null 2>&1; then \
		sudo pacman -S sdl2 sdl2_image lua cmake base-devel; \
	else \
		echo "Please install SDL2, SDL2_image, Lua, and CMake manually for your system"; \
	fi

help:
	@echo "ReAmp Build System"
	@echo ""
	@echo "Targets:"
	@echo "  build         - Build ReAmp (Release)"
	@echo "  build-debug   - Build ReAmp (Debug)"
	@echo "  clean         - Remove build directory"
	@echo "  run           - Build and run ReAmp"
	@echo "  install-deps  - Install required dependencies"
	@echo "  help          - Show this help message"

