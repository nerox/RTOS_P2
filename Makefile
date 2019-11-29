#Compila Programa fuente

CC = gcc
CFILES = project2_scheduling_rtos.c interfaz.c algoritmos.c
CFLAGS1 = -Wall -fPIC `pkg-config --libs --cflags gtk+-3.0`
CFLAGS2 = -o
Archivo = project2_scheduling_rtos
PATHBIN =
Format = .c
INCS1 =
INCS2 =
LIBS =  -lm
PATHLIB =
CFLAGS_LIB =
MY_LIB =

All: binario

binario: $(Archivo).c
	$(CC) $(CFILES) $(INCS1) $(LIBS) $(CFLAGS1) $(CFLAGS2) $(Archivo)


#gcc Scheduler_Controller.c Quick_Sort.c Deployer.c Scheduler.c Lottery.c lectura_json.c  Round_Robin.c project1_scheduling_rtos.c interfaz.c -lm -ljson-c -fPIC `pkg-config --libs --cflags gtk+-3.0` -o
