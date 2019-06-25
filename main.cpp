#include <cstdio>
#include <ctime>
#include <cstdlib>

#include "Config.h"
#include "Prime.h"
#include "Mont.h"

void TESTA() {
	BigInt a;
	a.random(1024);
	a.output();
	BigInt b;
	b.copy(a);
	b.value[0] -= 1;
	a.mod(b);
	a.output();

	INT32 x = 5;
	INT32 y = 3;
	BigInt::binary(x - y); printf("\n");

	ULL t = 1823475619283571243;
	printf("%016llX\n", t);
	printf("%016X\n", (INT32)t);
}

void showTime(clock_t start) {
	printf("%.3fms\n", (clock() - start) / 1000.0);
}

void TESTB() {
	srand(1);
	BigInt a;
	a.random(512);
	BigInt b;
	b.random(1024);
	BigInt N;
	N.random(1024);

	a.output();
	N.output();

	Mont mont;
	mont.set(N);

	BigInt r;
	clock_t start = clock();
	for (int i = 0; i < 100; ++i) {
		mont.exp(a, N, r);
	}
	showTime(start);
	r.output();
}

void TESTC() {
	PrimeProducer procuder;
	clock_t start = clock();
	procuder.getPrime();
	showTime(start);
	procuder.prime.output();
}


int main(int argc, char const *argv[])
{	
	//TESTA();
	//TESTB();
	TESTC();
	
	return 0;
}
