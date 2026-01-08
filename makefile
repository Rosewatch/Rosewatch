refresh_build:
	cmake -B build \
		-D CMAKE_C_COMPILER="/usr/bin/gcc" \
		-D CMAKE_C++_COMPILER="/usr/bin/gcc" \
		-D GRAPHICS=GRAPHICS_API_OPENGL_21 \
		-D CMAKE_BUILD_TYPE=Debug

build-bundle:
	cmake --build build
	$(MAKE) -C build install_apps

full_build: refresh_build build

run:
	build/src/shell/rosewatch

build-run: build-bundle run

debug-run: build-bundle
	gdb -ex=r --args build/src/shell/rosewatch 