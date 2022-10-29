myprog: firstScan.o secondScan.o macro.o firstAndSecond.o global.h main.o
	gcc -g -Wall -pedantic firstScan.o secondScan.o macro.o firstAndSecond.o main.o -o myprog
macro.o: macro.c macro.h firstAndSecond.h
	gcc -c -Wall -pedantic macro.c -o macro.o
firstAndSecond.o: firstAndSecond.c firstAndSecond.h global.h
	gcc -c -Wall -pedantic firstAndSecond.c -o firstAndSecond.o
firstScan.o: firstScan.c firstScan.h firstAndSecond.h
	gcc -c -Wall -pedantic firstScan.c -o firstScan.o
secondScan.o: secondScan.c secondScan.h firstAndSecond.h
	gcc -c -Wall -pedantic secondScan.c -o secondScan.o
main.o: main.c main.h macro.h firstScan.h secondScan.h
	gcc -c -Wall -pedantic main.c -o main.o
