Skull Atlas 2016
================

![Travis Status](https://travis-ci.com/chipbuster/skull-atlas.svg?token=JKmHgBdX7axXPFzjazNA)

```
         _,.-------.,_
     ,;~'             '~;,
   ,;                     ;,
  ;                         ;
 ,'                         ',
,;                           ;,
; ;      .           .      ; ;
| ;   ______       ______   ; |
|  `/~"     ~" . "~     "~\'  |
|  ~  ,-~~~^~, | ,~^~~~-,  ~  |
 |   |        }:{        |   |
 |   l       / | \       !   |
 .~  (__,.--" .^. "--.,__)  ~.
 |     ---;' / | \ `;---     |
  \__.       \/^\/       .__/
   V| \                 / |V
    | |T~\___!___!___/~T| |
    | |`IIII_I_I_I_IIII'| |
    |  \,III I I I III,/  |
     \   `~~~~~~~~~~'    /
       \   .       .   /  
         \.    ^    ./
           ^~~~^~~~^
```

This repository is meant to store the prototype Skull Atlas as created by CVC
for the Dell Medical School. It could theoretically run on all UNIXes, but due
to difference between GNU and BSD tools, is only working on Linux.

### How to acquire this source code

This repository is hosted on GitHub and has submodule components. Thus, the
correct way to clone this repository is

```
git clone --recursive https://github.com/chipbuster/skull-atlas.git
```

Not cloning recursively may lead to errors unless certain parts of the build
are disabled.

### Prerequisites

The list of prerequisite software for this project is incredibly large and
only grows with time. Some optional pieces become mandatory due to structural
changes (and vice versa), so this README might not always be up to date. There
are three general families of requirements: C++ build tools and libraries,
Python and its libraries, and R and its libraries.

#### Required Software
##### C++ and Friends

You will need:

  * CMake (v3.2.0 or greater)
  * A C++ compiler (currently only tested on g++ and clang++)
  * A Fortran compiler (currently only tested with gfortran)
  * UNIX build tools (automake, autoconf, make, m4, etc.)
  * Boost (1.62.0)
  * Qt4
  * libgmp
  * libmpfr
  * [freeglut3](http://freeglut.sourceforge.net/)
  * [CGAL](http://www.cgal.org/) (v4.8.x)
  * Eigen3
  
If you're on Ubuntu, you can find install commands for all of these packages
in the `.travis.yml` (just copy-paste the instructions one at a time from the
section `pre_install`)

Note: Most of these libraries are available in the repos of most mainstream 
linux distributions. freeglut can be tricky to find, but it usually exists.

[LibIGL](https://github.com/libigl/libigl) is also required, but it is
already provided in this source code (as a git submodule), so you do not
need to download/install it unless you want to.

CGAL 4.9 has a bug in it that stalls our pipeline, so it is not recommended
as of right now. 4.9.1 might fix this issue.

##### Python and Friends

You will need:
  
  * python3 (the code may run under python2, but it is not guaranteed)
  * numpy
  * [PyDICOM](http://www.pydicom.org/)
  * [niBabel](http://nipy.org/nibabel/)
  * [rpy2](https://rpy2.bitbucket.io/)

If you are using Python3 version 3.3 or older, you will also need to install
the backported enum package [`enum34`](https://pypi.python.org/pypi/enum34)
  
##### R and Friends

You will need:
  
  * R
  * [RNiftyReg](https://cran.r-project.org/web/packages/RNiftyReg/index.html)

Libraries can be installed in R with the command `install.packages(<pkgname>)`, e.g.
`install.packages('RNiftyReg')`

#### Optional Software

In order to build/run some of the older, deprecated programs in this repo, you
will additionally need:

  * sqlite3
  * matlab
  * an OpenGL-enabled web browser

### Building

This repository has a recursive CMake structure: the top-level contains a
`CMakeLists.txt` which simply defines a few environment variables and paths,
then adds subdirectories. Each subdirectory has its own CMakeLists.txt, and so
on and so forth, until everything is built. You should be able to build the
software in here with a regular CVC cmake workflow.

In the CMake build directory, there should be a folder named `bin/` that collects
all of the executables that the project generates, including scripts.

**Important note: the repo currently assumes that you have all of the necessary
prereqs, e.g. (CGAL, MPFR, GMP, Matlab) installed on your computer. There is no
dependency building yet.**

### Testing

This project is integrated with TravisCI. If you push code that does not build,
it will whine at you with an email. The software used by Travis is fairly
similar to what's found on the CVC linux machines (gcc version off by 3 minor,
cmake version off by 1 minor) so it had better build normally.

Build tests are triggered by pushing to the repo. If you don't want your push to
trigger a buildtest (e.g. you're just updating a readme) you can add `[ci skip]`
to your commit message. Please do not use this to knowingly commit broken code
or I will hunt you down and use a sharp object on you.

Runtime tests can be added by using the `add_test` command
[from CMake](https://cmake.org/cmake/help/v2.8.10/cmake.html#command:add_test)

### Running

Work is ongoing on a global pipeline driver.

### Components

This section needs to be redone after the shifting around.

### Notes on TravisCI

Currently, TravisCI downloads and builds its own copy of CGAL, due to the
version in Ubuntu being goddamn ancient. It still uses Ubuntu's system copies of
Boost/MPFR/GMP for dependencies of CGAL.
