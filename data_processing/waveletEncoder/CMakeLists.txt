cmake_minimum_required (VERSION 2.8)
project (waveletEncoder)


add_executable(wavletEnc wave.cpp waveAnthony.cpp block.cpp zeroblocktable.cpp zeroblocktableassist.cpp zerobit.cpp error.cpp mainassist.cpp encode.cpp main.cpp scale.cpp wavedata.cpp)

install(TARGETS wavletEnc DESTINATION bin)
