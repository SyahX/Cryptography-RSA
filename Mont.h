#ifndef MONT_H
#define MONT_H

#include "Config.h"

struct BigInt {
	int length;
	INT32* value;

	BigInt();
	~BigInt();

	void set(INT32);
	void copy(const BigInt&);
	void mod(const BigInt&);
	INT32 mod(INT32);
	void mul(const BigInt&, INT32);
	INT32 div(INT32);
	void add(BigInt&);
	void sub(BigInt&);
	void shl(int p);
	void shr(int p);

	void normal();

	bool equals(const BigInt&);
	bool equalsOne();

	void random(int bits);
	void output();

	static void binary(INT32 x);
};

bool operator < (BigInt& x, BigInt& y);
bool operator == (BigInt& x, BigInt& y);

class Mont {
	BigInt mod;
	BigInt rho;
	BigInt rho2;
	INT32 omega;
	
	INT32 getOmega();

public:
	void set(const BigInt& num);
	void mul(BigInt&, BigInt&, BigInt&);
	void exp(BigInt&, BigInt&, BigInt&);
	void getSquareRho(BigInt& r);
};

#endif