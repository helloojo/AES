#include <GaloisField.h>

static void swap(int& a, int& b) {
  int c = b;
  b = a;
  a = c;
}

// public method
GaloisField::GaloisField() : number_(0) {
}

GaloisField::GaloisField(const char num) : number_(num) {
}

GaloisField::GaloisField(const short num) : number_(num) {
}

GaloisField::GaloisField(const int num) : number_(num) {
}

GaloisField::GaloisField(const long num) : number_(num) {
}

GaloisField::GaloisField(const long long num) : number_(num) {
}

GaloisField::GaloisField(const unsigned char num) : number_(num) {
}

GaloisField::GaloisField(const unsigned short num) : number_(num) {
}

GaloisField::GaloisField(const unsigned int num) : number_(num) {
}

GaloisField::GaloisField(const unsigned long num) : number_(num) {
}

GaloisField::GaloisField(const unsigned long long num) : number_(num) {
}

GaloisField::GaloisField(const GaloisField& other) : number_(other.number()) {
}

GaloisField& GaloisField::operator=(const GaloisField& other) {
  number_ = other.number();
  return (*this);
}

const GaloisField GaloisField::operator+(const GaloisField& other) const {
  return GaloisField(add(number_, other.number()));
}

const GaloisField GaloisField::operator-(const GaloisField& other) const {
  return GaloisField(sub(number_, other.number()));
}

const GaloisField GaloisField::operator*(const GaloisField& other) const {
  return GaloisField(multi(number_, other.number()));
}

const GaloisField GaloisField::operator/(const GaloisField& other) const {
  return GaloisField(divide(number_, other.number()));
}

bool GaloisField::operator==(const GaloisField& other) const {
  return number_ == other.number();
}

bool GaloisField::operator!=(const GaloisField& other) const {
  return !((*this) == other);
}

GaloisField& GaloisField::operator+=(const GaloisField& other) {
  number_ = add(number_, other.number());
  return (*this);
}

GaloisField& GaloisField::operator-=(const GaloisField& other) {
  number_ = sub(number_, other.number());
  return (*this);
}

GaloisField& GaloisField::operator*=(const GaloisField& other) {
  number_ = multi(number_, other.number());
  return (*this);
}

GaloisField& GaloisField::operator/=(const GaloisField& other) {
  number_ = divide(number_, other.number());
  return (*this);
}

unsigned int GaloisField::number() const {
  return number_;
}
// end of public method

// private method
unsigned int GaloisField::extended_euclide_inverse(unsigned int a) {
  int a1 = 1, a2 = 0, a3 = a;
  int b1 = 0, b2 = 1, b3 = irr_poly;
  while (a3) {
    int q = degree(a3) - degree(b3);
    if (q < 0) {
      swap(a1, b1);
      swap(a2, b2);
      swap(a3, b3);
      q = -q;
    }
    a1 = add(a1, (b1 << q));
    a2 = add(a2, (b2 << q));
    a3 = add(a3, (b3 << q));
  }
  return b1;
}

unsigned int GaloisField::add(unsigned int a, unsigned int b) {
  return a ^ b;
}

unsigned int GaloisField::sub(unsigned int a, unsigned int b) {
  return a ^ b;
}

unsigned int GaloisField::multi(unsigned int a, unsigned int b) {
  unsigned int result = 0;
  while (a && b) {
    if (b & 0x01) {  // b의 가장 우측 비트가 1일때
      result ^= a;   //결과와 XOR
    }
    if (a & 0x80) {  // a의 가장 좌측비트가 8번째 비트일때
      a = (a << 1) ^ irr_poly;  // left shift후 Irrpoly와 XOR해서 차수 내려줌
    } else {
      a <<= 1;  // left shift
    }
    b >>= 1;  // b right shift
  }
  return result;
}

unsigned int GaloisField::divide(unsigned int a, unsigned int b) {
  return multi(a, extended_euclide_inverse(b));
}

bool GaloisField::is_carry(unsigned int a) {
  return a & 0x100;
}

unsigned int GaloisField::degree(unsigned int a) {
  for (int i = 31; i >= 0; i--) {
    if (a & (1u << i)) {
      return i;
    }
  }
  return 0;
}
// end of private method
