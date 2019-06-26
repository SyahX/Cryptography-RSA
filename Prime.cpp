#include "Prime.h"
#include <cstdio>
#include <ctime>

using namespace std;

void PrimeProducer::getPrime() {
	printf("\t[Producer] Start Gen big Prime ... \n");
	
	clock_t start = clock();
	int cnt = 0;
	while (++cnt) {
		prime.random(bits);
		if (!simpleTest()) continue;
		if (hardTest()) break;
	}

	printf("\t[Producer] After %d times, we find a prime, ", cnt);
	printf("use %.3fms\n", (clock() - start) / 1000.0);
}

bool PrimeProducer::simpleTest() {
	for (int i = 0; i < 1000; ++i)
		if (prime.mod(pList[i]) == 0)
			return false;
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

	d.value[d.length] = 0;
	d.value[0] += (d.value[1] << (32 - tmp));
	for (int i = 1; i < d.length; ++i) {
		d.value[i] = (d.value[i] >> tmp) + (d.value[i + 1] << (32 - tmp));
	}
	while (d.value[d.length - 1] == 0) --d.length;

	return cnt * 32 + tmp;
}

bool PrimeProducer::hardTest() {
	int cnt = prepare();

	two.set(2);
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