#include "Prime.h"
#include <cstdio>

using namespace std;

void PrimeProducer::getPrime() {
	printf("Start Gen big Prime ... \n");
	int cnt = 0;
	while (true) {
		printf("\ttry %d times ...\n", ++cnt);
		prime.random(bits);
		if (!simpleTest()) continue;
		if (hardTest()) break;
		//break;
	}
	printf("After %d times, we find a prime.\n", cnt);
}

bool PrimeProducer::simpleTest() {
	return true;
}

int PrimeProducer::prepare() {
	d.copy(prime);
	--d.value[0];
	d_hat.copy(d);

	int cnt = 0;
	for (int i = 0; i < d.length; ++i)
		if (d.value[i] == 0) ++cnt;
		else break;

	d.length -= cnt;
	for (int i = 0; i < d.length; ++i)
		d.value[i] = d.value[i + cnt];

	int tmp = 0;
	while (!(d.value[0] & 1)) {
		++tmp;
		d.value[0] >>= 1;
	}
	if (tmp == 0) return cnt * base;

	d.value[0] += (d.value[1] << (32 - tmp));
	d.value[d.length] = 0;
	for (int i = 1; i < d.length; ++i) {
		d.value[i] = (d.value[i] >> tmp) + (d.value[i + 1] << (32 - tmp));
	}
	return cnt * 32 + tmp;
}

bool PrimeProducer::hardTest() {
	int cnt = prepare();

	two.length = 1;
	two.value[0] = 2;
	mont.set(prime);
	for (int i = 0; i < TEST_TIMES; ++i)
		if (!MillerRabin(cnt))
			return false;
	return true;
}

bool PrimeProducer::MillerRabin(int cnt) {
	//printf("%d\n", cnt);
	a.random(bits - 32);

	BigInt r, tmp;
	//mont.exp(a, d_hat, r);
	//r.output();
	mont.exp(a, d, r);
	if (r.equals(d_hat)) return true;
	if (r.equalsOne()) return true;

	for (int i = 0; i < cnt; ++i) {
		//printf("Y\n");
		tmp.copy(r);
		mont.exp(tmp, two, r);
		if (r.equals(d_hat)) return true;
	}
	//r.output();
	return false;
}