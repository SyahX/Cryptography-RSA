#include "Mont.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

bool operator < (BigInt& x, BigInt& y) {
	x.normal(); 
	y.normal();
	if (x.length < y.length) return true;
	if (x.length > y.length) return false;

	for (int i = 0; i < x.length; ++i) {
		if (x.value[i] < y.value[i]) return true;
		if (x.value[i] > y.value[i]) return false;
	}
	return false;
}

bool operator == (BigInt& x, BigInt& y) {
	x.normal(); 
	y.normal();
	if (x.length != y.length) return false;

	for (int i = 0; i < x.length; ++i) {
		if (x.value[i] != y.value[i]) return false;
	}
	return true;
}

BigInt::BigInt() {
	value = new INT32[MAXL];
}

BigInt::~BigInt() {
	delete[] value;
}

void BigInt::set(INT32 x) {
	length = 1;
	value[0] = x;
}

void BigInt::set(const char* st, int b) {
	length = 1;
	value[0] = 0;
	int len = strlen(st);

	BigInt temp;
	for (int i = 0; i < len; ++i) {
		this->mul(b);
		if (b == 256) temp.set(st[i]);
		else {
			if (st[i] >= '0' && st[i] <= '9') temp.set(st[i] - '0');
			if (st[i] >= 'A' && st[i] <= 'Z') temp.set(st[i] - 'A' + 10);
			if (st[i] >= 'a' && st[i] <= 'z') temp.set(st[i] - 'a' + 10);
		}
		this->add(temp);
	}
}

void BigInt::copy(const BigInt& N) {
	length = N.length;
	for (int i = 0; i < length; ++i)
		value[i] = N.value[i];
}

void BigInt::mod(const BigInt& N) {
	if (length < N.length) return;

	if (length == N.length) {
		for (int i = length - 1; i >= 0; --i)
			if (value[i] < N.value[i])
				return;
			else if (value[i] > N.value[i])
				break;
	}

	INT32 flag = 0;
	for (int i = 0; i < N.length; ++i) {
		INT32 n1 = value[i] < N.value[i];
		value[i] -= N.value[i];
		INT32 n2 = value[i] < flag;
		value[i] -= flag;
		flag = n1 + n2;
	}
	length = N.length;
}

INT32 BigInt::mod(INT32 p) {
	ULL tmp = 0;
	for (int i = length - 1; i >= 0; --i) {
		tmp = (tmp << 32) + (ULL)value[i];
		tmp = tmp % p;
	}
	return tmp;
}

void BigInt::mul(const BigInt& x, INT32 y) {
	length = x.length;
	ULL tmp = 0;
	for (int i = 0; i < length; ++i) {
		tmp = tmp + (ULL)x.value[i] * y;
		value[i] = (INT32)tmp;
		tmp >>= 32;
	}
	if (tmp > 0) {
		value[length] = (INT32)tmp;
		++length;
	}
}

void BigInt::mul(INT32 y) {
	ULL tmp = 0;
	for (int i = 0; i < length; ++i) {
		tmp = tmp + (ULL)value[i] * y;
		value[i] = (INT32)tmp;
		tmp >>= 32;
	}
	if (tmp > 0) {
		value[length] = (INT32)tmp;
		++length;
	}
}

INT32 BigInt::div(INT32 y) {
	ULL tmp = 0;
	for (int i = length - 1; i >= 0; --i) {
		tmp = (tmp << 32) + (ULL)value[i];
		value[i] = tmp / y;
		tmp = tmp % y;
	}
	return tmp;
}

void BigInt::add(BigInt& x) {
	int l = max(length, x.length);
	for (int i = length; i < l; ++i)
		value[i] = 0;
	for (int i = x.length; i < l; ++i)
		x.value[i] = 0;
	length = l;

	ULL tmp = 0;
	for (int i = 0; i < length; ++i) {
		tmp = tmp + (ULL)value[i] + x.value[i];
		value[i] = (INT32)tmp;
		tmp >>= 32;
	}

	if (tmp > 0) {
		value[length] = (INT32)tmp;
		++length;
	}
}

void BigInt::sub(BigInt& x) {
	for (int i = x.length; i < length; ++i)
		x.value[i] = 0;
	INT32 flag = 0;
	for (int i = 0; i < length; ++i) {
		INT32 n1 = value[i] < x.value[i];
		value[i] -= x.value[i];
		INT32 n2 = value[i] < flag;
		value[i] -= flag;
		flag = n1 + n2;
	}
	while (length > 1 && value[length - 1] == 0) --length;
}

void BigInt::normal() {
	while (length > 1 && value[length - 1] == 0) --length;
}

void BigInt::shl(int tmp) {
	value[length] = (value[length - 1] >> (32 - tmp));
	for (int i = length - 1; i >= 0; --i) {
		value[i] = (value[i] << tmp) + (value[i - 1] >> (32 - tmp));
	}
	if (value[length] > 0) ++length;
}

void BigInt::shr(int tmp) {
	value[length] = 0;
	for (int i = 0; i < length; ++i) {
		value[i] = (value[i] >> tmp) + (value[i + 1] << (32 - tmp));
	}
	if (length > 1 && value[length - 1] == 0) --length;
}

bool BigInt::equals(const BigInt& N) {
	if (length != N.length) return false;
	for (int i = 0; i < length; ++i)
		if (value[i] != N.value[i]) return false;
	return true;
}

bool BigInt::equalsOne() {
	if (value[0] != 1) return false;
	for (int i = 1; i < length; ++i)
		if (value[i] != 0) return false;
	return true;
}

void BigInt::random(int bits) {
	length = bits / base;
	for (int i = 0; i < length; ++i) {
		value[i] = 0;
		for (int j = 0; j < base; ++j) {
			value[i] = (value[i] << 1) + (rand() & 1);
		}
	}
	value[0] |= 1;
	value[length - 1] |= ((INT32)1 << 31);
}

void BigInt::binary(INT32 x) {
	for (int i = 31; i >= 0; --i) {
		printf("%d", ((x >> i) & 1));
	}
}

void BigInt::output() {
	for (int i = length - 1; i >= 0; --i)
		printf("%08X", value[i]);
	printf("\n");
}

INT32 Mont::getOmega() {
	INT32 t = 1;
	for (int i = 0; i < base - 1; ++i)
		t = t * t * mod.value[0];
	t = -t;
	return t;
}

void Mont::getSquareRho(BigInt& r) {
	r.length = mod.length;
	for (int i = 1; i < r.length; ++i)
		r.value[i] = 0;
	r.value[0] = 1;

	for (int i = 0; i < 2 * mod.length * base; ++i) {
		INT32 flag = 0;
		for (int j = 0; j < r.length; ++j) {
			INT32 next = ((r.value[j] >> 31) & 1);
			r.value[j] <<= 1;
			r.value[j] += flag;
			flag = next;
		}
		r.value[r.length] = flag;
		r.length += flag;
		r.mod(mod);
	}
}

void Mont::set(const BigInt& num) {
	mod.length = num.length;
	for (int i = 0; i < num.length; ++i)
		mod.value[i] = num.value[i];
	omega = getOmega();
	getSquareRho(rho2);
}

void Mont::mul(BigInt& x, BigInt& y, BigInt& r) {
	for (int i = x.length; i < mod.length; ++i) x.value[i] = 0;
	for (int i = y.length; i < mod.length; ++i) y.value[i] = 0;

	r.length = mod.length;
	for (int i = 0; i < r.length; ++i)
		r.value[i] = 0;

	BigInt temp;
	for (int i = 0; i < mod.length; ++i) {
		INT32 u = (r.value[0] + y.value[i] * x.value[0]) * omega;
		temp.mul(x, y.value[i]);
		r.add(temp);
		temp.mul(mod, u);
		r.add(temp);
		for (int j = 0; j < r.length - 1; ++j)
			r.value[j] = r.value[j + 1];
		--r.length;
		
	}
	r.mod(mod);
}

void Mont::exp(BigInt& x, BigInt& y, BigInt& t) {
	for (int i = x.length; i < mod.length; ++i) x.value[i] = 0;
	for (int i = y.length; i < mod.length; ++i) y.value[i] = 0;

	BigInt t_hat, x_hat;
	t_hat.length = mod.length;
	for (int i = 1; i < t_hat.length; ++i)
		t_hat.value[i] = 0;
	t_hat.value[0] = 1;

	mul(t_hat, rho2, t);
	mul(x, rho2, x_hat);

	for (int i = y.length - 1; i >= 0; --i) {
		for (int j = 31; j >= 0; --j) {
			t_hat.copy(t);
			mul(t_hat, t_hat, t);
			if ((y.value[i] >> j) & 1) {
				t_hat.copy(t);
				mul(t_hat, x_hat, t);
			}
		}
	}

	t_hat.length = mod.length;
	for (int i = 1; i < t_hat.length; ++i)
		t_hat.value[i] = 0;
	t_hat.value[0] = 1;
	x_hat.copy(t);
	mul(x_hat, t_hat, t);
}