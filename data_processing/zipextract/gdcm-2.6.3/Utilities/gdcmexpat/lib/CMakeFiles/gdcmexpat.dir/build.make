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
CMAKE_SOURCE_DIR = /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3

# Include any dependencies generated for this target.
include Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/depend.make

# Include the progress variables for this target.
include Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/progress.make

# Include the compile flags for this target's objects.
include Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/flags.make

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/flags.make
Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o: Utilities/gdcmexpat/lib/xmlparse.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gdcmexpat.dir/xmlparse.c.o   -c /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmlparse.c

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gdcmexpat.dir/xmlparse.c.i"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -E /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmlparse.c > CMakeFiles/gdcmexpat.dir/xmlparse.c.i

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gdcmexpat.dir/xmlparse.c.s"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -S /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmlparse.c -o CMakeFiles/gdcmexpat.dir/xmlparse.c.s

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.requires:

.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.requires

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.provides: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.requires
	$(MAKE) -f Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/build.make Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.provides.build
.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.provides

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.provides.build: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o


Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/flags.make
Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o: Utilities/gdcmexpat/lib/xmltok.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gdcmexpat.dir/xmltok.c.o   -c /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmltok.c

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gdcmexpat.dir/xmltok.c.i"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -E /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmltok.c > CMakeFiles/gdcmexpat.dir/xmltok.c.i

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gdcmexpat.dir/xmltok.c.s"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -S /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmltok.c -o CMakeFiles/gdcmexpat.dir/xmltok.c.s

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.requires:

.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.requires

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.provides: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.requires
	$(MAKE) -f Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/build.make Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.provides.build
.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.provides

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.provides.build: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o


Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/flags.make
Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o: Utilities/gdcmexpat/lib/xmlrole.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gdcmexpat.dir/xmlrole.c.o   -c /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmlrole.c

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gdcmexpat.dir/xmlrole.c.i"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -E /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmlrole.c > CMakeFiles/gdcmexpat.dir/xmlrole.c.i

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gdcmexpat.dir/xmlrole.c.s"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && /usr/bin/cc  $(C_DEFINES) -DHAVE_EXPAT_CONFIG_H $(C_INCLUDES) $(C_FLAGS) -S /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/xmlrole.c -o CMakeFiles/gdcmexpat.dir/xmlrole.c.s

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.requires:

.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.requires

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.provides: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.requires
	$(MAKE) -f Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/build.make Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.provides.build
.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.provides

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.provides.build: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o


# Object files for target gdcmexpat
gdcmexpat_OBJECTS = \
"CMakeFiles/gdcmexpat.dir/xmlparse.c.o" \
"CMakeFiles/gdcmexpat.dir/xmltok.c.o" \
"CMakeFiles/gdcmexpat.dir/xmlrole.c.o"

# External object files for target gdcmexpat
gdcmexpat_EXTERNAL_OBJECTS =

bin/libgdcmexpat.a: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o
bin/libgdcmexpat.a: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o
bin/libgdcmexpat.a: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o
bin/libgdcmexpat.a: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/build.make
bin/libgdcmexpat.a: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library ../../../bin/libgdcmexpat.a"
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && $(CMAKE_COMMAND) -P CMakeFiles/gdcmexpat.dir/cmake_clean_target.cmake
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gdcmexpat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/build: bin/libgdcmexpat.a

.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/build

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/requires: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlparse.c.o.requires
Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/requires: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmltok.c.o.requires
Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/requires: Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/xmlrole.c.o.requires

.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/requires

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/clean:
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib && $(CMAKE_COMMAND) -P CMakeFiles/gdcmexpat.dir/cmake_clean.cmake
.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/clean

Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/depend:
	cd /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3 /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3 /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib /home/chipbuster/CVC/skull-atlas/data_processing/gdcm-2.6.3/Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Utilities/gdcmexpat/lib/CMakeFiles/gdcmexpat.dir/depend

