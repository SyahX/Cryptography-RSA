#ifndef RSA_H
#define RSA_H

#include "Config.h"
#include "Prime.h"
#include "Mont.h"

class RSA {
	PrimeProducer producer;
	Mont mont;

	BigInt pu;
	BigInt pr;
	BigInt N;
	BigInt phi;
	BigInt prime1;
	BigInt prime2;

	void exGcd(INT32, INT32, int&, int&);
	void getPrivateKey();
	void mul(BigInt&, BigInt&, BigInt&);

public:
	void init();
	void setPublicKey(INT32 key);
	BigInt getPublicKey();
	BigInt encode(BigInt& x);
	BigInt decode(BigInt& x);
};

#endif