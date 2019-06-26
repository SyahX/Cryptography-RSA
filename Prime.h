#ifndef PRIME_H
#define PRIME_H

#include "Mont.h"

class PrimeProducer {
private:
	const static int TEST_TIMES = 10;

	BigInt d;
	BigInt d_hat;
	BigInt a;
	BigInt two;
	Mont mont;

public:
	const static int bits = 1024;
	BigInt prime;

	void getPrime();
	bool simpleTest();
	int prepare();
	bool hardTest();
	bool MillerRabin(int);
};

#endif