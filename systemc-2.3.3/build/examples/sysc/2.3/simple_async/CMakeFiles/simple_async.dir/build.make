# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build"

# Include any dependencies generated for this target.
include examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/depend.make

# Include the progress variables for this target.
include examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/progress.make

# Include the compile flags for this target's objects.
include examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/flags.make

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o: examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/flags.make
examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o: ../examples/sysc/2.3/simple_async/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.3/simple_async" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple_async.dir/main.cpp.o -c "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/2.3/simple_async/main.cpp"

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple_async.dir/main.cpp.i"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.3/simple_async" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/2.3/simple_async/main.cpp" > CMakeFiles/simple_async.dir/main.cpp.i

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple_async.dir/main.cpp.s"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.3/simple_async" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/2.3/simple_async/main.cpp" -o CMakeFiles/simple_async.dir/main.cpp.s

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.requires:

.PHONY : examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.requires

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.provides: examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.requires
	$(MAKE) -f examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/build.make examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.provides.build
.PHONY : examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.provides

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.provides.build: examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o


# Object files for target simple_async
simple_async_OBJECTS = \
"CMakeFiles/simple_async.dir/main.cpp.o"

# External object files for target simple_async
simple_async_EXTERNAL_OBJECTS =

examples/sysc/2.3/simple_async/simple_async: examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o
examples/sysc/2.3/simple_async/simple_async: examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/build.make
examples/sysc/2.3/simple_async/simple_async: src/libsystemc.so.2.3.3
examples/sysc/2.3/simple_async/simple_async: examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simple_async"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.3/simple_async" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple_async.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/build: examples/sysc/2.3/simple_async/simple_async

.PHONY : examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/build

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/requires: examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/main.cpp.o.requires

.PHONY : examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/requires

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/clean:
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.3/simple_async" && $(CMAKE_COMMAND) -P CMakeFiles/simple_async.dir/cmake_clean.cmake
.PHONY : examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/clean

examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/depend:
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/2.3/simple_async" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.3/simple_async" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : examples/sysc/2.3/simple_async/CMakeFiles/simple_async.dir/depend

