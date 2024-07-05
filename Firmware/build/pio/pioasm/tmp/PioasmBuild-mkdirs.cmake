# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/chris/Code/pico-sdk/tools/pioasm"
  "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pioasm"
  "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pio/pioasm"
  "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pio/pioasm/tmp"
  "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pio/pioasm/src/PioasmBuild-stamp"
  "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pio/pioasm/src"
  "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pio/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pio/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/chris/Code/Pierre-IGGDriver/Firmware/build/pio/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
