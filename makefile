test: main.o Mont.o Mont.h Config.h
	g++ main.o Mont.o -o test -O2 -Wall
AES.o: Mont.cpp Mont.h
	g++ -c Mont.cpp -o Mont.o -O2 -Wall
main.o: main.cpp Mont.h
	g++ -c main.cpp -o main.o -O2 -Wall
clean:
	rm *.o test


