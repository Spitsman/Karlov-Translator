#pragma once
#include <iostream>
#include <stdio.h>
//using namespace std;


const int N = 1000; /* integer size */
const int LL = 10;/*int_max*/

class Iterator{
private:
	int*p;
public:
	Iterator(){
		p = 0;
	}
	Iterator(int * a){
		p = a;
	}
	Iterator & operator ++ (){
		p++;
		return *this;
	}
	Iterator & operator -- (){
		p--;
		return *this;
	}
	int operator * (){
		return *p;
	}
	bool operator != (Iterator & reference){
		return p != reference.p;
	}
	bool operator == (Iterator & reference){
		return p == reference.p;
	}
};

class integer {
private:
	int *a;
public:
	integer();
	integer(const integer&);
	integer(int);
	~integer();
	int& operator[] (int) const;
	int length() const;
	void LevelUp();
	bool operator > (const integer&) const;
	bool operator < (const integer&) const;
	bool operator == (const integer&) const;
	bool operator != (const integer&) const;
	bool operator >= (const integer&) const;
	bool operator <= (const integer&) const;
	friend std::istream& operator >> (std::istream&, integer&);
	friend std::ostream& operator << (std::ostream&, integer&);
	integer  operator + (const integer&) const;
	integer  operator * (const integer&) const;
	integer  operator - (const integer&) const;
	integer  operator / (const integer&) const;
	integer  operator % (const integer&) const;
	integer& operator +=(const integer&);
	integer& operator = (const integer&);
	integer& operator -=(const integer&);
	integer& operator *=(const integer&);
	integer& operator /=(const integer&);
	Iterator begin();
	Iterator end();
	int ConvertToInt()
	{
		int number = 0;
		int len = pow(10,length());
		for (Iterator i = end(); i != begin(); i--)
		{
			number += (*i)*len;
			len /= 10;
		}
		number += *begin();
		return number;
	}
};


Iterator integer::begin(){ return Iterator(a); }

Iterator integer::end(){
	Iterator i(a + N - 1);
	while (*i == 0)	i--;
	return i++;
}

integer::integer() {
	a = new int[N];
	for (int i = 0; i < N; ++i)
		a[i] = 0;
}

integer::integer(const integer& second) {
	a = new int[N];
	for (int i = 0; i < N; ++i)
		a[i] = second[i];
}

integer::integer(int noun) {
	a = new int[N];
	long long int ten = 1;
	for (int i = 0; i < LL; ++i) {
		a[i] = (noun / ten) % 10;
		ten = ten * 10;
	}
	for (int i = LL; i < N; ++i)
		a[i] = 0;
}

integer::~integer() {
	delete[]a;
}

int integer::length() const {
	int i = N - 1;
	while (a[i] == 0 && i > 0) i--;
	return i + 1;
}

int& integer::operator[](int i) const {
	return a[i];
}

bool integer::operator == (const integer& second) const {
	for (int i = 0; i < N; ++i)
	if (a[i] != second[i]) return false;
	return true;
}

bool integer::operator != (const integer& second) const {
	return !(*this == second);
}

bool integer::operator >(const integer& second) const {
	for (int i = N - 1; i >= 0; --i) {
		if (a[i] > second[i]) return true;
		if (a[i] < second[i]) return false;
	}
	return false;
}

bool integer::operator < (const integer& second) const{
	for (int i = N - 1; i >= 0; --i) {
		if (a[i] > second[i]) return false;
		if (a[i] < second[i]) return true;
	}
	return false;
}

bool integer::operator >= (const integer& second) const {
	return *this > second || *this == second;
}

bool integer::operator <= (const integer& second) const{
	return *this < second || *this == second;
}

std::ostream& operator << (std::ostream& out, integer& first) {
	int i = N - 1;
	while (i >= 0 && first[i] == 0)
		i--;
	if (i == -1) std::cout << 0;
	else
	for (int j = i; j >= 0; --j)
		out << first[j];
	return out;
}

std::istream& operator >> (std::istream& sin, integer& first) {
	char c;
	char buff[N];
	int i = 0;
	while (c = getchar()) {
		if (c == ' ' || c == '\0' || c == '\n' || c == '\t')
			break;
		buff[i] = c;
		i++;
	}
	bool b = false;
	for (int j = 0; j < i; ++j) {
		if (buff[i - j - 1] == '-')
			b = true;
		else
			first[j] = (int)buff[i - j - 1] - (int)'0';
	}
	if (b == true)
		first[first.length() - 1] *= -1;
	return sin;
}

integer integer::operator + (const integer& second) const{
	integer c, one = *this, two = second;
	int sum;

	if (*this > 0 && second < 0) {
		two[two.length() - 1] *= -1;
		return one - two;
	}
	if (*this < 0 && second > 0) {
		one[one.length() - 1] *= -1;
		return two - one;
	}

	bool b = false;
	if (*this < 0 && second < 0) {
		one[one.length() - 1] *= -1;
		two[two.length() - 1] *= -1;
		b = true;
	}

	for (int i = 0; i < N - 1; ++i) {
		sum = one[i] + two[i] + c[i];
		c[i] = sum % 10;
		c[i + 1] = sum / 10;
	}
	if (b == true)
		c[c.length() - 1] *= -1;
	return c;
}

integer integer::operator * (const integer& second) const {
	integer c, one = *this, two = second;
	bool check = false;

	if (*this < 0 && second > 0) {
		one[one.length() - 1] *= -1;
		check = true;
	}
	if (second < 0 && *this > 0) {
		two[two.length() - 1] *= -1;
		check = true;
	}
	if (*this < 0 && second < 0) {
		one[one.length() - 1] *= -1;
		two[two.length() - 1] *= -1;
	}

	for (int i = 0; i < N / 2; ++i)
	for (int j = 0; j < N / 2 - 1; ++j)
		c[i + j] = c[i + j] + one[i] * two[j];

	for (int i = 0; i < N - 1; ++i) {
		c[i + 1] = c[i + 1] + c[i] / 10;
		c[i] = c[i] % 10;
	}

	if (check == true)
		c[c.length() - 1] *= -1;
	return c;
}

integer& integer::operator = (const integer& second) {
	if (this == &second) return *this;
	for (int i = 0; i < N; ++i)
		a[i] = second[i];
	return *this;
}

integer integer::operator - (const integer& second) const {
	integer c;
	bool choose = false;

	if (*this < 0 && second < 0) {
		integer tempfirst = *this;
		integer tempsecond = second;
		tempfirst[length() - 1] *= -1;
		tempsecond[second.length() - 1] *= -1;
		integer Return = tempfirst + tempsecond;
		Return[Return.length() - 1] *= -1;
		return Return;
	}

	if (second < 0 && *this > 0) {
		integer temp = second;
		temp[temp.length() - 1] *= -1;
		return temp + *this;
	}
	if (second > 0 && *this < 0) {
		integer temp = *this;
		temp[temp.length() - 1] *= -1;
		return temp + *this;
	}

	integer one = *this, two = second;
	if (*this < second) {
		std::swap(one, two);
		choose = true;
	}

	for (int i = 0; i < N - 1; ++i) {
		c[i] = c[i] + 10 + one[i] - two[i];
		c[i + 1] = (c[i] >= 10) ? 0 : -1;
		c[i] = c[i] % 10;
	}

	int i = N - 1;
	while (c[i] != 0) {
		c[i] = 0;
		i--;
	}

	if (choose == true)
		c[c.length() - 1] *= -1;
	return c;
}

integer& integer::operator -= (const integer& second) {
	integer a = *this - second;
	return a;
}

void integer::LevelUp()
{
	for (int i = length(); i >= 1; i--)
		a[i] = a[i - 1];
}

integer integer::operator / (const integer& second) const
{
	integer res;
	integer curValue;
	integer one = *this, two = second;
	bool check = false;

	if (*this < 0 && second > 0) {
		one[one.length() - 1] *= -1;
		check = true;
	}
	if (*this > 0 && second < 0) {
		two[two.length() - 1] *= -1;
		check = true;
	}

	for (int i = one.length() - 1; i >= 0; i--)
	{
		curValue.LevelUp(); // * osn
		curValue[0] = one.a[i];

		int x = 0;
		int l = 0, r = 10;
		while (l <= r)
		{
			int m = (l + r) >> 1;
			integer cur = two * m;
			if (cur <= curValue)
			{
				x = m;
				l = m + 1;
			}
			else
				r = m - 1;
		}
		res[i] = x;
		curValue = curValue - two * x;
	}

	int pos = length();
	while (pos >= 0 && !res[pos])
		pos--;
	if (check == true)
		res[res.length() - 1] *= -1;
	return res;
}

integer& integer::operator /= (const integer& second) {
	integer a = *this / second;
	return a;
}

integer integer::operator % (const integer& second) const {
	return *this - (*this / second) * second;
}

