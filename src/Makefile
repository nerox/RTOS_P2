#Compila Programa fuente

CC = gcc
#CFLAGS1 = -pg -fopenmp
CFLAGS1 = -o
CFLAGS2 = `pkg-config --libs --cflags gtk+-3.0`
Archivo = project2_scheduling_rtos
PATHBIN = ../
Format = .c
INCS1 =
INCS2 =
INCS3 = -I../include/
LIBS =  -lm
PATHLIB = -L../lib/
CFLAGS_LIB = -Wl,-rpath=$(dir $(mkfile_path))../lib
MY_LIB1 = libinterfaz.so
MY_LIB2 = libalgoritmos.so
My_LIBS = -linterfaz -lalgoritmos
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
All:  binario

binario: $(Archivo).c
	$(CC) $(CFLAGS1) $(PATHBIN)$(Archivo) $(Archivo)$(Format) $(CFLAGS2) $(INCS1) $(INCS2) $(INCS3) $(LIBS) $(PATHLIB) $(CFLAGS_LIB) $(My_LIBS)
