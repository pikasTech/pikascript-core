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
include src/mimimemory/CMakeFiles/mimimemory.dir/depend.make

# Include the progress variables for this target.
include src/mimimemory/CMakeFiles/mimimemory.dir/progress.make

# Include the compile flags for this target's objects.
include src/mimimemory/CMakeFiles/mimimemory.dir/flags.make

src/mimimemory/CMakeFiles/mimimemory.dir/src/VM_memory.c.o: src/mimimemory/CMakeFiles/mimimemory.dir/flags.make
src/mimimemory/CMakeFiles/mimimemory.dir/src/VM_memory.c.o: ../src/mimimemory/src/VM_memory.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/mimimemory/CMakeFiles/mimimemory.dir/src/VM_memory.c.o"
	cd /root/dev/mimilib/build/src/mimimemory && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mimimemory.dir/src/VM_memory.c.o   -c /root/dev/mimilib/src/mimimemory/src/VM_memory.c

src/mimimemory/CMakeFiles/mimimemory.dir/src/VM_memory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mimimemory.dir/src/VM_memory.c.i"
	cd /root/dev/mimilib/build/src/mimimemory && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/dev/mimilib/src/mimimemory/src/VM_memory.c > CMakeFiles/mimimemory.dir/src/VM_memory.c.i

src/mimimemory/CMakeFiles/mimimemory.dir/src/VM_memory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mimimemory.dir/src/VM_memory.c.s"
	cd /root/dev/mimilib/build/src/mimimemory && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/dev/mimilib/src/mimimemory/src/VM_memory.c -o CMakeFiles/mimimemory.dir/src/VM_memory.c.s

# Object files for target mimimemory
mimimemory_OBJECTS = \
"CMakeFiles/mimimemory.dir/src/VM_memory.c.o"

# External object files for target mimimemory
mimimemory_EXTERNAL_OBJECTS =

../lib/libmimimemory.a: src/mimimemory/CMakeFiles/mimimemory.dir/src/VM_memory.c.o
../lib/libmimimemory.a: src/mimimemory/CMakeFiles/mimimemory.dir/build.make
../lib/libmimimemory.a: src/mimimemory/CMakeFiles/mimimemory.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ../../../lib/libmimimemory.a"
	cd /root/dev/mimilib/build/src/mimimemory && $(CMAKE_COMMAND) -P CMakeFiles/mimimemory.dir/cmake_clean_target.cmake
	cd /root/dev/mimilib/build/src/mimimemory && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mimimemory.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/mimimemory/CMakeFiles/mimimemory.dir/build: ../lib/libmimimemory.a

.PHONY : src/mimimemory/CMakeFiles/mimimemory.dir/build

src/mimimemory/CMakeFiles/mimimemory.dir/clean:
	cd /root/dev/mimilib/build/src/mimimemory && $(CMAKE_COMMAND) -P CMakeFiles/mimimemory.dir/cmake_clean.cmake
.PHONY : src/mimimemory/CMakeFiles/mimimemory.dir/clean

src/mimimemory/CMakeFiles/mimimemory.dir/depend:
	cd /root/dev/mimilib/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/dev/mimilib /root/dev/mimilib/src/mimimemory /root/dev/mimilib/build /root/dev/mimilib/build/src/mimimemory /root/dev/mimilib/build/src/mimimemory/CMakeFiles/mimimemory.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/mimimemory/CMakeFiles/mimimemory.dir/depend
