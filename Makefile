CFLAGS = -g -Wno-unused-parameter

all : chainemain

chainemain : ChaineMain.o Chaine.o SVGwriter.o
	gcc -lm -o chainemain $(CFLAGS) ChaineMain.o Chaine.o SVGwriter.o -lm

ChaineMain.o : ChaineMain.c Chaine.h SVGwriter.h
	gcc $(CFLAGS) -c ChaineMain.c

SVGwriter.o : SVGwriter.c SVGwriter.h
	gcc $(CFLAGS) -c SVGwriter.c
	
Chaine.o : Chaine.c SVGwriter.h
	gcc $(CFLAGS)  -c Chaine.c

