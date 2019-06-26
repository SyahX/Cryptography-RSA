#include "RSA.h"
#include <cstdio>
#include <algorithm>

using namespace std;

void RSA::init() {
	printf("Start Gen two %d bits prime ... \n", producer.bits);
	producer.getPrime();
	prime1.copy(producer.prime);
	producer.getPrime();
	prime2.copy(producer.prime);
	printf("-----\n");

	mul(prime1, prime2, N);
	mont.set(N);

	prime1.value[0] -= 1;
	prime2.value[0] -= 1;
	mul(prime1, prime2, phi);
	prime1.value[0] += 1;
	prime2.value[0] += 1;
	
	//prime1.output();
	//prime2.output();
	//N.output();
	//phi.output();
}

void RSA::setPublicKey(INT32 key) {
	pu.set(key);
	printf("Start Gen private key ... \n");
	getPrivateKey();
	pr.output();
	printf("-----\n");
}

void RSA::exGcd(INT32 a, INT32 b, int& x, int& y) {
	if (b == 0) {
		x = 1;
		y = 0;
		return;
	}
	exGcd(b, a % b, x, y);
	int t = x;
	x = y;
	y = t - a / b * y;
}

void RSA::getPrivateKey() {
	INT32 a = pu.value[0];
	pr.copy(phi);
 	INT32 b = pr.div(a);
 	int x, y;
	exGcd(a, b, x, y);
	printf("%d %d\n", x, y);
	BigInt r;
	r.set(0);
	for (int j = 30; j >= 0; --j) {
		r.shl(1);
		if ((abs(y) >> j) & 1)
			r.add(pr);
		r.mod(phi);
	}

	if (y > 0) {
		pr.copy(phi);
		pr.sub(r);
	}
	else pr.copy(r);

	r.set(abs(x));
	if (x >= 0) {
		pr.add(r);
		pr.mod(phi);
	}
	else {
		pr.add(phi);
		pr.sub(r);
		pr.mod(phi);
	}
}

void RSA::mul(BigInt& x, BigInt& y, BigInt& r) {
	r.set(0);
	for (int i = y.length - 1; i >= 0; --i) {
		for (int j = 31; j >= 0; --j) {
			r.shl(1);
			if ((y.value[i] >> j) & 1)
				r.add(x);
		}
	}
}

BigInt RSA::encode(BigInt& x) {
	BigInt r;
	mont.exp(x, pu, r);
	return r;
}

BigInt RSA::decode(BigInt& x) {
	BigInt r;
	mont.exp(x, pr, r);
	return r;
}