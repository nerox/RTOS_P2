#MAKEFILE QUE CREARÁ LA BIBLIOTECA DINÁMICA

FLAGS = -fPIC `pkg-config --libs --cflags gtk+-3.0` -I../include/
SRC1 = interfaz
SRC2 = algoritmos

All: $(SRC1).so $(SRC1).o $(SRC2).so $(SRC2).o
$(SRC1).so: $(SRC1).o
	ld -o lib$(SRC1).so $(SRC1).o -shared

$(SRC1).o: $(SRC1).c
	gcc -c $(SRC1).c $(FLAGS)

$(SRC2).so: $(SRC2).o
	ld -o lib$(SRC2).so $(SRC2).o -shared

$(SRC2).o: $(SRC2).c
	gcc -c $(SRC2).c $(FLAGS)
