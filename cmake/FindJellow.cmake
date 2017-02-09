# Try to find Jellow: headers and libraries.
#
#  JELLOW_FOUND               True if libgsasl got found
#  JELLOW_INCLUDE_DIRS        Location of libgsasl headers 
#  JELLOW_LIBRARIES           List of libaries to use libgsasl 
#
# Copyright (c) 2017 Nikita Belov <zodiac.nv@gmail.com>

if(WIN32)
	find_path(JELLOW_INCLUDE_DIRS jreen/jreen.h $ENV{INCLUDE})
	find_library(JELLOW_LIBRARIES libjreen-qt5 $ENV{LIB})
else()
	find_path(JELLOW_INCLUDE_DIRS jreen/jreen.h PATH_SUFFIXES jreen-qt5)
	find_library(JELLOW_LIBRARIES jreen-qt5)
endif()

if(JELLOW_INCLUDE_DIRS AND JELLOW_LIBRARIES)
	set(JELLOW_FOUND true)
endif()

