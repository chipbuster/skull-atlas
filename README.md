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
for the Dell Medical School.

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

### Components

The files in `file_converters` provide methods for converting between different
file formats. They should have individual README files (probably short) to
describe how you use them. This structure is expected to remain stable.

The files in `data_processing` provide methods for processing data (e.g.
meshing, extracting features, or registration). The structure of this folder is
not entirely clear.

The files in `cgal_dummy` are just to make sure that the build system (TravisCI)
and CGAL are working as intended. The tests are pulled
from [the latest CGAL introduction](http://doc.cgal.org/latest/Manual/introduction.html).
This directory will probably be removed later if it is determined that it is no
longer needed.
