Skull Atlas External Projects
=============================

This directory contains external projects for Skull Atlas.

## libigl

libigl is only included as a dependency. Since it can be run in header-only mode,
no additional compilation is needed

## niftyreg

NiftyReg is a project from UCL. There are a few missing link flags for some of
the binaries, so the submodule has been slightly customized.

By default, the library is set to build without CUDA. You can optionally set it 
to build with CUDA by setting the `NIFTYREG_CUDA` option to `ON`. If you do
this, you need to make sure you have a CUDA-compatible compiler. GCC6 and
above are not supported. Make sure that `NIFTYREG_C_COMPILER` and 
`NIFTYREG_CXX_COMPILER` have been set to CUDA-compatible compilers before
invoking your build system, because the CMake system is not capable of
detecting incompatibilities during the configure phase and will cause
a build failure.
