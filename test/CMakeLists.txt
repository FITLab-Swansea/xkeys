######################################################
# CMake Build Script for X-Keys Test Program
#
# The code may be used by anyone for any purpose,
# and can serve as a starting point for developing
# applications using the X-Keys libary.
#
# Alan Ott
# Signal 11 Software
# 2011-08-11
######################################################

project(piehidtest)

cmake_minimum_required(VERSION 2.8.0)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# Source (c) files
SET(SRCS
	test.c
)

INCLUDE_DIRECTORIES(
	${pie_SOURCE_DIR}/piehid
)

LINK_DIRECTORIES(
	${pie_BINARY_DIR}
	/usr/local/lib
)

ADD_DEFINITIONS()

ADD_EXECUTABLE(piehidtest ${SRCS})

TARGET_LINK_LIBRARIES(
	piehidtest # executable name
	piehid     # lib to link to
)

INSTALL(TARGETS piehidtest
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include )


