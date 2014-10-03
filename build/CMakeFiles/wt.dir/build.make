# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/irina/projects/lughos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/irina/projects/lughos/build

# Utility rule file for wt.

# Include the progress variables for this target.
include CMakeFiles/wt.dir/progress.make

CMakeFiles/wt: CMakeFiles/wt-complete

CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-install
CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-mkdir
CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-download
CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-update
CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-patch
CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-configure
CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-build
CMakeFiles/wt-complete: ../externalLibraries/src/wt-stamp/wt-install
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Completed 'wt'"
	/usr/bin/cmake -E make_directory /home/irina/projects/lughos/build/CMakeFiles
	/usr/bin/cmake -E touch /home/irina/projects/lughos/build/CMakeFiles/wt-complete
	/usr/bin/cmake -E touch /home/irina/projects/lughos/externalLibraries/src/wt-stamp/wt-done

../externalLibraries/src/wt-stamp/wt-install: ../externalLibraries/src/wt-stamp/wt-build
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing install step for 'wt'"
	cd /home/irina/projects/lughos/externalLibraries/src/wt-build && $(MAKE) install
	cd /home/irina/projects/lughos/externalLibraries/src/wt-build && /usr/bin/cmake -E touch /home/irina/projects/lughos/externalLibraries/src/wt-stamp/wt-install

../externalLibraries/src/wt-stamp/wt-mkdir:
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Creating directories for 'wt'"
	/usr/bin/cmake -E make_directory /home/irina/projects/lughos/externalLibraries/src/wt
	/usr/bin/cmake -E make_directory /home/irina/projects/lughos/externalLibraries/src/wt-build
	/usr/bin/cmake -E make_directory /home/irina/projects/lughos/externalLibraries
	/usr/bin/cmake -E make_directory /home/irina/projects/lughos/externalLibraries/tmp
	/usr/bin/cmake -E make_directory /home/irina/projects/lughos/externalLibraries/src/wt-stamp
	/usr/bin/cmake -E make_directory /home/irina/projects/lughos/externalLibraries/src
	/usr/bin/cmake -E touch /home/irina/projects/lughos/externalLibraries/src/wt-stamp/wt-mkdir

../externalLibraries/src/wt-stamp/wt-download: ../externalLibraries/src/wt-stamp/wt-gitinfo.txt
../externalLibraries/src/wt-stamp/wt-download: ../externalLibraries/src/wt-stamp/wt-mkdir
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing download step (git clone) for 'wt'"
	cd /home/irina/projects/lughos/externalLibraries/src && /usr/bin/cmake -P /home/irina/projects/lughos/externalLibraries/tmp/wt-gitclone.cmake
	cd /home/irina/projects/lughos/externalLibraries/src && /usr/bin/cmake -E touch /home/irina/projects/lughos/externalLibraries/src/wt-stamp/wt-download

../externalLibraries/src/wt-stamp/wt-update: ../externalLibraries/src/wt-stamp/wt-download
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing update step for 'wt'"
	cd /home/irina/projects/lughos/externalLibraries/src/wt && /usr/bin/cmake -P /home/irina/projects/lughos/externalLibraries/tmp/wt-gitupdate.cmake

../externalLibraries/src/wt-stamp/wt-patch: ../externalLibraries/src/wt-stamp/wt-download
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "No patch step for 'wt'"
	/usr/bin/cmake -E touch /home/irina/projects/lughos/externalLibraries/src/wt-stamp/wt-patch

../externalLibraries/src/wt-stamp/wt-configure: ../externalLibraries/tmp/wt-cfgcmd.txt
../externalLibraries/src/wt-stamp/wt-configure: ../externalLibraries/src/wt-stamp/wt-update
../externalLibraries/src/wt-stamp/wt-configure: ../externalLibraries/src/wt-stamp/wt-patch
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing configure step for 'wt'"
	cd /home/irina/projects/lughos/externalLibraries/src/wt-build && /usr/bin/cmake -DCMAKE_TOOLCHAIN_FILE:FILEPATH= -DCMAKE_INSTALL_PREFIX:PATH=/home/irina/projects/lughos/externalLibraries -DCONFIGDIR:STRING=/home/irina/projects/lughos/externalLibraries/etc -DCONNECTOR_FCGI:BOOL=OFF -DBoost_USE_STATIC_LIBS=ON -DBOOST_LIBRARYDIR:PATH=/lib -DBOOST_ROOT:PATH= -DHARU_PREFIX:STRING=/home/irina/projects/lughos/externalLibraries -DCMAKE_FIND_LIBRARY_SUFFIXES:STRING=\".a .dll.a\" "-GUnix Makefiles" /home/irina/projects/lughos/externalLibraries/src/wt
	cd /home/irina/projects/lughos/externalLibraries/src/wt-build && /usr/bin/cmake -E touch /home/irina/projects/lughos/externalLibraries/src/wt-stamp/wt-configure

../externalLibraries/src/wt-stamp/wt-build: ../externalLibraries/src/wt-stamp/wt-configure
	$(CMAKE_COMMAND) -E cmake_progress_report /home/irina/projects/lughos/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing build step for 'wt'"
	cd /home/irina/projects/lughos/externalLibraries/src/wt-build && $(MAKE)
	cd /home/irina/projects/lughos/externalLibraries/src/wt-build && /usr/bin/cmake -E touch /home/irina/projects/lughos/externalLibraries/src/wt-stamp/wt-build

wt: CMakeFiles/wt
wt: CMakeFiles/wt-complete
wt: ../externalLibraries/src/wt-stamp/wt-install
wt: ../externalLibraries/src/wt-stamp/wt-mkdir
wt: ../externalLibraries/src/wt-stamp/wt-download
wt: ../externalLibraries/src/wt-stamp/wt-update
wt: ../externalLibraries/src/wt-stamp/wt-patch
wt: ../externalLibraries/src/wt-stamp/wt-configure
wt: ../externalLibraries/src/wt-stamp/wt-build
wt: CMakeFiles/wt.dir/build.make
.PHONY : wt

# Rule to build all files generated by this target.
CMakeFiles/wt.dir/build: wt
.PHONY : CMakeFiles/wt.dir/build

CMakeFiles/wt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wt.dir/clean

CMakeFiles/wt.dir/depend:
	cd /home/irina/projects/lughos/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/irina/projects/lughos /home/irina/projects/lughos /home/irina/projects/lughos/build /home/irina/projects/lughos/build /home/irina/projects/lughos/build/CMakeFiles/wt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wt.dir/depend

