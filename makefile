refresh-build:
	cmake -B build \
		-D CMAKE_C_COMPILER="/usr/bin/gcc" \
		-D CMAKE_C++_COMPILER="/usr/bin/gcc" \
		-D GRAPHICS=GRAPHICS_API_OPENGL_21 \
		-D CMAKE_BUILD_TYPE=Debug

build-bundle:
	cmake --build build
	$(MAKE) -C build install_apps

full-build: refresh-build build-bundle

run:
	build/src/shell/rosewatch

build-run: build-bundle run

debug-run: build-bundle
	gdb -ex=r --args build/src/shell/rosewatch 

remote-sync:
	rsync -avz . frida@rosewatch$(ARGS):/var/rosewatch --exclude .git --exclude build --exclude apps

remote-build: remote-sync
	ssh frida@rosewatch$(ARGS) -o RemoteCommand="/var/rosewatch/build.sh"

remote-kill:
	ssh frida@rosewatch$(ARGS) -o RemoteCommand="/var/rosewatch/kill.sh"

remote-run: remote-kill
	ssh frida@rosewatch$(ARGS) -o RemoteCommand="/var/rosewatch/build/bin/lvglsim $(ARGS)"