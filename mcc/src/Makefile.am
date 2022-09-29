## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = mcc

mcc_SOURCES = \
	cryptog.c \
	cryptog.h \
	server.c \
	signature.c \
	signature.h \
	SocketProject.c \
	SocketProject.h

mcc_LDFLAGS = 

mcc_LDADD = 

gcc -o foo signature.c cryptog.c server.c SocketProject.c   -I /usr/local/include/pbc -L /usr/local/lib -lgmp  -Wl,-rpath /usr/local/lib -l pbc -lpthread


