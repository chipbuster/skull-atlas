## CMakeLists for external projects

include(ExternalProject)

option(BUILD_NIFTYREG "Build the NiftyReg registration tools" ON)
if(BUILD_NIFTYREG)

    # NiftyReg CUDA support is a bit finicky with compiler support
    # Disable it by default and only enable if you know what you're doing
    option(NIFTYREG_CUDA "Build NiftyReg with CUDA Support" OFF)
    set(NIFTYREG_C_COMPILER ${CMAKE_C_COMPILER} CACHE STRING "C compiler to use for NiftyReg")
    set(NIFTYREG_CXX_COMPILER ${CMAKE_CXX_COMPILER} CACHE STRING "C++ compiler to use for NiftyReg")
    if(NIFTYREG_CUDA)
        set(NIFTYREG_CUDA_FLAG "ON" CACHE STRING "Value to pass to CMake for USE_CUDA")
        mark_as_advanced(NIFTYREG_CXX_COMPILER NIFTYREG_C_COMPILER)
    else(NIFTYREG_CUDA)
        set(NIFTYREG_CUDA_FLAG "OFF" CACHE STRING "Value to pass to CMake for USE_CUDA")
    endif(NIFTYREG_CUDA)
    mark_as_advanced(NIFTYREG_CUDA_FLAG)

    # Since we may need to configure different compilers for NiftyReg than for
    # the rest of the project, we add it as an external project. If CUDA
    # is not enabled, this is fairly irrelevant...but it seems useful.
    ExternalProject_Add(NiftyReg
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/niftyreg/nifty_reg/
        CMAKE_COMMAND cmake
        CMAKE_ARGS "-DCMAKE_C_COMPILER=${NIFTYREG_C_COMPILER}" "-DCMAKE_CXX_COMPILER=${NIFTYREG_CXX_COMPILER}" "-DUSE_CUDA=${NIFTYREG_CUDA_FLAG}"
        BUILD_COMMAND make
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    )

endif(BUILD_NIFTYREG)
