# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Aw/B/esp/v5.2.3/esp-idf/components/bootloader/subproject"
  "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader"
  "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader-prefix"
  "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader-prefix/tmp"
  "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader-prefix/src"
  "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Aw/B/esp/v5.2.3/esp-idf/examples/usb_msc_wireless_disk/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
