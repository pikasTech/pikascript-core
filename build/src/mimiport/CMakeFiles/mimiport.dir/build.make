# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/dev/mimilib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/dev/mimilib/build

# Include any dependencies generated for this target.
include src/mimiport/CMakeFiles/mimiport.dir/depend.make

# Include the progress variables for this target.
include src/mimiport/CMakeFiles/mimiport.dir/progress.make

# Include the compile flags for this target's objects.
include src/mimiport/CMakeFiles/mimiport.dir/flags.make

src/mimiport/CMakeFiles/mimiport.dir/src/PORT_bh1750.c.o: src/mimiport/CMakeFiles/mimiport.dir/flags.make
src/mimiport/CMakeFiles/mimiport.dir/src/PORT_bh1750.c.o: ../src/mimiport/src/PORT_bh1750.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/mimiport/CMakeFiles/mimiport.dir/src/PORT_bh1750.c.o"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mimiport.dir/src/PORT_bh1750.c.o   -c /root/dev/mimilib/src/mimiport/src/PORT_bh1750.c

src/mimiport/CMakeFiles/mimiport.dir/src/PORT_bh1750.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mimiport.dir/src/PORT_bh1750.c.i"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/dev/mimilib/src/mimiport/src/PORT_bh1750.c > CMakeFiles/mimiport.dir/src/PORT_bh1750.c.i

src/mimiport/CMakeFiles/mimiport.dir/src/PORT_bh1750.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mimiport.dir/src/PORT_bh1750.c.s"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/dev/mimilib/src/mimiport/src/PORT_bh1750.c -o CMakeFiles/mimiport.dir/src/PORT_bh1750.c.s

src/mimiport/CMakeFiles/mimiport.dir/src/PORT_com.c.o: src/mimiport/CMakeFiles/mimiport.dir/flags.make
src/mimiport/CMakeFiles/mimiport.dir/src/PORT_com.c.o: ../src/mimiport/src/PORT_com.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/mimiport/CMakeFiles/mimiport.dir/src/PORT_com.c.o"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mimiport.dir/src/PORT_com.c.o   -c /root/dev/mimilib/src/mimiport/src/PORT_com.c

src/mimiport/CMakeFiles/mimiport.dir/src/PORT_com.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mimiport.dir/src/PORT_com.c.i"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/dev/mimilib/src/mimiport/src/PORT_com.c > CMakeFiles/mimiport.dir/src/PORT_com.c.i

src/mimiport/CMakeFiles/mimiport.dir/src/PORT_com.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mimiport.dir/src/PORT_com.c.s"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/dev/mimilib/src/mimiport/src/PORT_com.c -o CMakeFiles/mimiport.dir/src/PORT_com.c.s

src/mimiport/CMakeFiles/mimiport.dir/src/mimiPort_market.c.o: src/mimiport/CMakeFiles/mimiport.dir/flags.make
src/mimiport/CMakeFiles/mimiport.dir/src/mimiPort_market.c.o: ../src/mimiport/src/mimiPort_market.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/mimiport/CMakeFiles/mimiport.dir/src/mimiPort_market.c.o"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mimiport.dir/src/mimiPort_market.c.o   -c /root/dev/mimilib/src/mimiport/src/mimiPort_market.c

src/mimiport/CMakeFiles/mimiport.dir/src/mimiPort_market.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mimiport.dir/src/mimiPort_market.c.i"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/dev/mimilib/src/mimiport/src/mimiPort_market.c > CMakeFiles/mimiport.dir/src/mimiPort_market.c.i

src/mimiport/CMakeFiles/mimiport.dir/src/mimiPort_market.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mimiport.dir/src/mimiPort_market.c.s"
	cd /root/dev/mimilib/build/src/mimiport && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/dev/mimilib/src/mimiport/src/mimiPort_market.c -o CMakeFiles/mimiport.dir/src/mimiPort_market.c.s

# Object files for target mimiport
mimiport_OBJECTS = \
"CMakeFiles/mimiport.dir/src/PORT_bh1750.c.o" \
"CMakeFiles/mimiport.dir/src/PORT_com.c.o" \
"CMakeFiles/mimiport.dir/src/mimiPort_market.c.o"

# External object files for target mimiport
mimiport_EXTERNAL_OBJECTS =

../lib/libmimiport.a: src/mimiport/CMakeFiles/mimiport.dir/src/PORT_bh1750.c.o
../lib/libmimiport.a: src/mimiport/CMakeFiles/mimiport.dir/src/PORT_com.c.o
../lib/libmimiport.a: src/mimiport/CMakeFiles/mimiport.dir/src/mimiPort_market.c.o
../lib/libmimiport.a: src/mimiport/CMakeFiles/mimiport.dir/build.make
../lib/libmimiport.a: src/mimiport/CMakeFiles/mimiport.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library ../../../lib/libmimiport.a"
	cd /root/dev/mimilib/build/src/mimiport && $(CMAKE_COMMAND) -P CMakeFiles/mimiport.dir/cmake_clean_target.cmake
	cd /root/dev/mimilib/build/src/mimiport && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mimiport.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/mimiport/CMakeFiles/mimiport.dir/build: ../lib/libmimiport.a

.PHONY : src/mimiport/CMakeFiles/mimiport.dir/build

src/mimiport/CMakeFiles/mimiport.dir/clean:
	cd /root/dev/mimilib/build/src/mimiport && $(CMAKE_COMMAND) -P CMakeFiles/mimiport.dir/cmake_clean.cmake
.PHONY : src/mimiport/CMakeFiles/mimiport.dir/clean

src/mimiport/CMakeFiles/mimiport.dir/depend:
	cd /root/dev/mimilib/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/dev/mimilib /root/dev/mimilib/src/mimiport /root/dev/mimilib/build /root/dev/mimilib/build/src/mimiport /root/dev/mimilib/build/src/mimiport/CMakeFiles/mimiport.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/mimiport/CMakeFiles/mimiport.dir/depend
