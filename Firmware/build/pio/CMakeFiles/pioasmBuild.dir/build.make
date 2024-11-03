# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chris/Code/IGG_-64x64M/Firmware

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/Code/IGG_-64x64M/Firmware/build

# Utility rule file for pioasmBuild.

# Include any custom commands dependencies for this target.
include pio/CMakeFiles/pioasmBuild.dir/compiler_depend.make

# Include the progress variables for this target.
include pio/CMakeFiles/pioasmBuild.dir/progress.make

pio/CMakeFiles/pioasmBuild: pio/CMakeFiles/pioasmBuild-complete

pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install
pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-mkdir
pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-download
pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-update
pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-patch
pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-configure
pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build
pio/CMakeFiles/pioasmBuild-complete: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E make_directory /home/chris/Code/IGG_-64x64M/Firmware/build/pio/CMakeFiles
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E touch /home/chris/Code/IGG_-64x64M/Firmware/build/pio/CMakeFiles/pioasmBuild-complete
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E touch /home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-done

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install:
.PHONY : pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build:
.PHONY : pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install:
.PHONY : pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pioasm && $(MAKE)

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-configure: pio/pioasm/tmp/pioasmBuild-cfgcmd.txt
pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-configure: pio/pioasm/tmp/pioasmBuild-cache-Release.cmake
pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-configure: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pioasm && /usr/bin/cmake --no-warn-unused-cli -DCMAKE_MAKE_PROGRAM:FILEPATH=/usr/bin/make -DPIOASM_FLAT_INSTALL=1 -DCMAKE_INSTALL_PREFIX=/home/chris/Code/IGG_-64x64M/Firmware/build/pioasm-install -DCMAKE_RULE_MESSAGES=OFF -DCMAKE_INSTALL_MESSAGE=NEVER "-GUnix Makefiles" -C/home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/tmp/pioasmBuild-cache-Release.cmake -S /home/chris/Code/pico-sdk/tools/pioasm -B /home/chris/Code/IGG_-64x64M/Firmware/build/pioasm
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pioasm && /usr/bin/cmake -E touch /home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-configure

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-download: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-source_dirinfo.txt
pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-download: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E echo_append
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E touch /home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-download

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Performing install step for 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pioasm && $(MAKE) install

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build:
.PHONY : pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -Dcfgdir= -P /home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/tmp/pioasmBuild-mkdirs.cmake
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E touch /home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-mkdir

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-patch: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-patch-info.txt
pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-patch: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E echo_append
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E touch /home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-patch

pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-update: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-update-info.txt
pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-update: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/chris/Code/IGG_-64x64M/Firmware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No update step for 'pioasmBuild'"
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E echo_append
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && /usr/bin/cmake -E touch /home/chris/Code/IGG_-64x64M/Firmware/build/pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-update

pioasmBuild: pio/CMakeFiles/pioasmBuild
pioasmBuild: pio/CMakeFiles/pioasmBuild-complete
pioasmBuild: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-build
pioasmBuild: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-configure
pioasmBuild: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-download
pioasmBuild: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-install
pioasmBuild: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-mkdir
pioasmBuild: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-patch
pioasmBuild: pio/pioasm/src/pioasmBuild-stamp/pioasmBuild-update
pioasmBuild: pio/CMakeFiles/pioasmBuild.dir/build.make
.PHONY : pioasmBuild

# Rule to build all files generated by this target.
pio/CMakeFiles/pioasmBuild.dir/build: pioasmBuild
.PHONY : pio/CMakeFiles/pioasmBuild.dir/build

pio/CMakeFiles/pioasmBuild.dir/clean:
	cd /home/chris/Code/IGG_-64x64M/Firmware/build/pio && $(CMAKE_COMMAND) -P CMakeFiles/pioasmBuild.dir/cmake_clean.cmake
.PHONY : pio/CMakeFiles/pioasmBuild.dir/clean

pio/CMakeFiles/pioasmBuild.dir/depend:
	cd /home/chris/Code/IGG_-64x64M/Firmware/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/Code/IGG_-64x64M/Firmware /home/chris/Code/IGG_-64x64M/Firmware/pio /home/chris/Code/IGG_-64x64M/Firmware/build /home/chris/Code/IGG_-64x64M/Firmware/build/pio /home/chris/Code/IGG_-64x64M/Firmware/build/pio/CMakeFiles/pioasmBuild.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : pio/CMakeFiles/pioasmBuild.dir/depend
