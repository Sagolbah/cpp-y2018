# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/sagolbah/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4505.18/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/sagolbah/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4505.18/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sagolbah/SPbITMO/C++/homeworks/hw4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sagolbah/SPbITMO/C++/homeworks/hw4

# Include any dependencies generated for this target.
include CMakeFiles/huffman_testing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/huffman_testing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/huffman_testing.dir/flags.make

CMakeFiles/huffman_testing.dir/huffman_testing.cpp.o: CMakeFiles/huffman_testing.dir/flags.make
CMakeFiles/huffman_testing.dir/huffman_testing.cpp.o: huffman_testing.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sagolbah/SPbITMO/C++/homeworks/hw4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/huffman_testing.dir/huffman_testing.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_testing.dir/huffman_testing.cpp.o -c /home/sagolbah/SPbITMO/C++/homeworks/hw4/huffman_testing.cpp

CMakeFiles/huffman_testing.dir/huffman_testing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_testing.dir/huffman_testing.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sagolbah/SPbITMO/C++/homeworks/hw4/huffman_testing.cpp > CMakeFiles/huffman_testing.dir/huffman_testing.cpp.i

CMakeFiles/huffman_testing.dir/huffman_testing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_testing.dir/huffman_testing.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sagolbah/SPbITMO/C++/homeworks/hw4/huffman_testing.cpp -o CMakeFiles/huffman_testing.dir/huffman_testing.cpp.s

CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.o: CMakeFiles/huffman_testing.dir/flags.make
CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.o: gtest/gtest-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sagolbah/SPbITMO/C++/homeworks/hw4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.o -c /home/sagolbah/SPbITMO/C++/homeworks/hw4/gtest/gtest-all.cc

CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sagolbah/SPbITMO/C++/homeworks/hw4/gtest/gtest-all.cc > CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.i

CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sagolbah/SPbITMO/C++/homeworks/hw4/gtest/gtest-all.cc -o CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.s

CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.o: CMakeFiles/huffman_testing.dir/flags.make
CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.o: gtest/gtest_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sagolbah/SPbITMO/C++/homeworks/hw4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.o -c /home/sagolbah/SPbITMO/C++/homeworks/hw4/gtest/gtest_main.cc

CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sagolbah/SPbITMO/C++/homeworks/hw4/gtest/gtest_main.cc > CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.i

CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sagolbah/SPbITMO/C++/homeworks/hw4/gtest/gtest_main.cc -o CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.s

# Object files for target huffman_testing
huffman_testing_OBJECTS = \
"CMakeFiles/huffman_testing.dir/huffman_testing.cpp.o" \
"CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.o" \
"CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.o"

# External object files for target huffman_testing
huffman_testing_EXTERNAL_OBJECTS =

huffman_testing: CMakeFiles/huffman_testing.dir/huffman_testing.cpp.o
huffman_testing: CMakeFiles/huffman_testing.dir/gtest/gtest-all.cc.o
huffman_testing: CMakeFiles/huffman_testing.dir/gtest/gtest_main.cc.o
huffman_testing: CMakeFiles/huffman_testing.dir/build.make
huffman_testing: CMakeFiles/huffman_testing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sagolbah/SPbITMO/C++/homeworks/hw4/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable huffman_testing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/huffman_testing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/huffman_testing.dir/build: huffman_testing

.PHONY : CMakeFiles/huffman_testing.dir/build

CMakeFiles/huffman_testing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/huffman_testing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/huffman_testing.dir/clean

CMakeFiles/huffman_testing.dir/depend:
	cd /home/sagolbah/SPbITMO/C++/homeworks/hw4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sagolbah/SPbITMO/C++/homeworks/hw4 /home/sagolbah/SPbITMO/C++/homeworks/hw4 /home/sagolbah/SPbITMO/C++/homeworks/hw4 /home/sagolbah/SPbITMO/C++/homeworks/hw4 /home/sagolbah/SPbITMO/C++/homeworks/hw4/CMakeFiles/huffman_testing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/huffman_testing.dir/depend
