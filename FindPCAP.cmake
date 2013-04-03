# FindPCAP.cmake
#
# Simple CMake find module for searching libpcap. If the main pcap header can be
# identified, as well as the shared library, this module will assume that the
# library has been found.

FIND_PATH(
  PCAP_INCLUDE_DIR
  pcap/pcap.h
)

FIND_LIBRARY(
  PCAP_LIBRARY
  pcap
)

SET(
  PCAP_INCLUDE_DIRS
  ${PCAP_INCLUDE_DIR}
)

SET(
  PCAP_LIBRARIES
  ${PCAP_LIBRARY}
)

# Some notifications

INCLUDE( FindPackageHandleStandardArgs )

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  PCAP
  DEFAULT_MSG
  PCAP_LIBRARY
  PCAP_INCLUDE_DIR )

MARK_AS_ADVANCED(
  PCAP_INCLUDE_DIR
  PCAP_LIBRARY
)
