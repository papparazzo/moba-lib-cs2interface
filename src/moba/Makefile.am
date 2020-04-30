AUTOMAKE_OPTIONS = subdir-objects
noinst_LIBRARIES = \
    libcs2interface.a
libcs2interface_a_SOURCES = \
    cs2reader.cpp \
    cs2utils.cpp \
    cs2writer.cpp

LDFLAGS += -L/usr/local/lib

CLEANFILES = *~

AM_CPPFLAGS = \
    -g3 -g -ggdb -O3 -Wall
