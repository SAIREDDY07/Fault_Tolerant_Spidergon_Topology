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
include examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/depend.make

# Include the progress variables for this target.
include examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/progress.make

# Include the compile flags for this target's objects.
include examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/flags.make

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o: examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/flags.make
examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o: ../examples/sysc/simple_fifo/simple_fifo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/simple_fifo" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o -c "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/simple_fifo/simple_fifo.cpp"

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple_fifo.dir/simple_fifo.cpp.i"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/simple_fifo" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/simple_fifo/simple_fifo.cpp" > CMakeFiles/simple_fifo.dir/simple_fifo.cpp.i

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple_fifo.dir/simple_fifo.cpp.s"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/simple_fifo" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/simple_fifo/simple_fifo.cpp" -o CMakeFiles/simple_fifo.dir/simple_fifo.cpp.s

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.requires:

.PHONY : examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.requires

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.provides: examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.requires
	$(MAKE) -f examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/build.make examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.provides.build
.PHONY : examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.provides

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.provides.build: examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o


# Object files for target simple_fifo
simple_fifo_OBJECTS = \
"CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o"

# External object files for target simple_fifo
simple_fifo_EXTERNAL_OBJECTS =

examples/sysc/simple_fifo/simple_fifo: examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o
examples/sysc/simple_fifo/simple_fifo: examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/build.make
examples/sysc/simple_fifo/simple_fifo: src/libsystemc.so.2.3.3
examples/sysc/simple_fifo/simple_fifo: examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simple_fifo"
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/simple_fifo" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple_fifo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/build: examples/sysc/simple_fifo/simple_fifo

.PHONY : examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/build

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/requires: examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/simple_fifo.cpp.o.requires

.PHONY : examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/requires

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/clean:
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/simple_fifo" && $(CMAKE_COMMAND) -P CMakeFiles/simple_fifo.dir/cmake_clean.cmake
.PHONY : examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/clean

examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/depend:
	cd "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/simple_fifo" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/simple_fifo" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : examples/sysc/simple_fifo/CMakeFiles/simple_fifo.dir/depend

