# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/cmake_build

# Include any dependencies generated for this target.
include CMakeFiles/sudoku.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sudoku.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sudoku.dir/flags.make

CMakeFiles/sudoku.dir/main.cpp.o: CMakeFiles/sudoku.dir/flags.make
CMakeFiles/sudoku.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/cmake_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sudoku.dir/main.cpp.o"
	/Applications/Xcode6-Beta5.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sudoku.dir/main.cpp.o -c /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/main.cpp

CMakeFiles/sudoku.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sudoku.dir/main.cpp.i"
	/Applications/Xcode6-Beta5.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/main.cpp > CMakeFiles/sudoku.dir/main.cpp.i

CMakeFiles/sudoku.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sudoku.dir/main.cpp.s"
	/Applications/Xcode6-Beta5.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/main.cpp -o CMakeFiles/sudoku.dir/main.cpp.s

CMakeFiles/sudoku.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/sudoku.dir/main.cpp.o.requires

CMakeFiles/sudoku.dir/main.cpp.o.provides: CMakeFiles/sudoku.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/sudoku.dir/build.make CMakeFiles/sudoku.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/sudoku.dir/main.cpp.o.provides

CMakeFiles/sudoku.dir/main.cpp.o.provides.build: CMakeFiles/sudoku.dir/main.cpp.o


# Object files for target sudoku
sudoku_OBJECTS = \
"CMakeFiles/sudoku.dir/main.cpp.o"

# External object files for target sudoku
sudoku_EXTERNAL_OBJECTS =

sudoku: CMakeFiles/sudoku.dir/main.cpp.o
sudoku: CMakeFiles/sudoku.dir/build.make
sudoku: /usr/local/lib/libopencv_videostab.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_superres.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_stitching.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_shape.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_photo.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_objdetect.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_hal.a
sudoku: /usr/local/lib/libopencv_calib3d.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_features2d.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_ml.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_highgui.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_videoio.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_imgcodecs.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_flann.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_video.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_imgproc.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_core.3.0.0.dylib
sudoku: /usr/local/lib/libopencv_hal.a
sudoku: /usr/local/share/OpenCV/3rdparty/lib/libippicv.a
sudoku: CMakeFiles/sudoku.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/cmake_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sudoku"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sudoku.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sudoku.dir/build: sudoku

.PHONY : CMakeFiles/sudoku.dir/build

CMakeFiles/sudoku.dir/requires: CMakeFiles/sudoku.dir/main.cpp.o.requires

.PHONY : CMakeFiles/sudoku.dir/requires

CMakeFiles/sudoku.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sudoku.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sudoku.dir/clean

CMakeFiles/sudoku.dir/depend:
	cd /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/cmake_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/cmake_build /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/cmake_build /Users/mjchao/Desktop/Sudoku-Solver/Sudoku-Solver/cmake_build/CMakeFiles/sudoku.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sudoku.dir/depend
