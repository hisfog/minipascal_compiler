# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/hisfog/byyl_sy/minipascal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hisfog/byyl_sy/minipascal/build

# Include any dependencies generated for this target.
include CMakeFiles/mainfunc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mainfunc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mainfunc.dir/flags.make

CMakeFiles/mainfunc.dir/src/main.cpp.o: CMakeFiles/mainfunc.dir/flags.make
CMakeFiles/mainfunc.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hisfog/byyl_sy/minipascal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mainfunc.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mainfunc.dir/src/main.cpp.o -c /home/hisfog/byyl_sy/minipascal/src/main.cpp

CMakeFiles/mainfunc.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mainfunc.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hisfog/byyl_sy/minipascal/src/main.cpp > CMakeFiles/mainfunc.dir/src/main.cpp.i

CMakeFiles/mainfunc.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mainfunc.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hisfog/byyl_sy/minipascal/src/main.cpp -o CMakeFiles/mainfunc.dir/src/main.cpp.s

CMakeFiles/mainfunc.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/mainfunc.dir/src/main.cpp.o.requires

CMakeFiles/mainfunc.dir/src/main.cpp.o.provides: CMakeFiles/mainfunc.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/mainfunc.dir/build.make CMakeFiles/mainfunc.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/mainfunc.dir/src/main.cpp.o.provides

CMakeFiles/mainfunc.dir/src/main.cpp.o.provides.build: CMakeFiles/mainfunc.dir/src/main.cpp.o


# Object files for target mainfunc
mainfunc_OBJECTS = \
"CMakeFiles/mainfunc.dir/src/main.cpp.o"

# External object files for target mainfunc
mainfunc_EXTERNAL_OBJECTS =

mainfunc: CMakeFiles/mainfunc.dir/src/main.cpp.o
mainfunc: CMakeFiles/mainfunc.dir/build.make
mainfunc: /usr/lib/x86_64-linux-gnu/libboost_system.so
mainfunc: /usr/lib/x86_64-linux-gnu/libboost_thread.so
mainfunc: /usr/lib/x86_64-linux-gnu/libboost_regex.so
mainfunc: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
mainfunc: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
mainfunc: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
mainfunc: /usr/lib/x86_64-linux-gnu/libpthread.so
mainfunc: libboolExpression.a
mainfunc: libbasic.a
mainfunc: libAST.a
mainfunc: CMakeFiles/mainfunc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hisfog/byyl_sy/minipascal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mainfunc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mainfunc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mainfunc.dir/build: mainfunc

.PHONY : CMakeFiles/mainfunc.dir/build

CMakeFiles/mainfunc.dir/requires: CMakeFiles/mainfunc.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/mainfunc.dir/requires

CMakeFiles/mainfunc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mainfunc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mainfunc.dir/clean

CMakeFiles/mainfunc.dir/depend:
	cd /home/hisfog/byyl_sy/minipascal/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hisfog/byyl_sy/minipascal /home/hisfog/byyl_sy/minipascal /home/hisfog/byyl_sy/minipascal/build /home/hisfog/byyl_sy/minipascal/build /home/hisfog/byyl_sy/minipascal/build/CMakeFiles/mainfunc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mainfunc.dir/depend

