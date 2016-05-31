all: imgdata

imgdata: 16bit.o build.o extract.o main.o preview.o Makefile
		 gcc -O 16bit.o build.o extract.o main.o preview.o -o imgdata

main.o: main.c 16bit.h build.h extract.h preview.h Makefile
		 gcc -c -O main.c

16bit.o: 16bit.c 16bit.h Makefile
		 gcc -c -O 16bit.c

build.o: build.c build.h Makefile
		 gcc -c -O build.c

extract.o: extract.c extract.h Makefile
		 gcc -c -O extract.c

preview.o: preview.c preview.h Makefile
		 gcc -c -O preview.c
		      
clean:
		 rm imgdata.o imgdata
