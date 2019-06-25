test: main.o Mont.o Mont.h Prime.o Prime.h Config.h
	g++ main.o Mont.o Prime.o -o test -O2 -Wall
Mont.o: Mont.cpp Mont.h
	g++ -c Mont.cpp -o Mont.o -O2 -Wall
Prime.o: Prime.cpp Prime.h
	g++ -c Prime.cpp -o Prime.o -O2 -Wall
main.o: main.cpp Mont.h Prime.h
	g++ -c main.cpp -o main.o -O2 -Wall
clean:
	rm *.o test


