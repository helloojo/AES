#ifndef GALOISFIELD_H
#define GALOISFIELD_H

class GaloisField {
 public:
  GaloisField();
  GaloisField(const char);
  GaloisField(const short);
  GaloisField(const int);
  GaloisField(const long);
  GaloisField(const long long);
  GaloisField(const unsigned char);
  GaloisField(const unsigned short);
  GaloisField(const unsigned int);
  GaloisField(const unsigned long);
  GaloisField(const unsigned long long);
  GaloisField(const GaloisField&);
  GaloisField& operator=(const GaloisField&);
  template <typename T>
  GaloisField& operator=(const T&);

  const GaloisField operator+(const GaloisField&) const;
  const GaloisField operator-(const GaloisField&) const;
  const GaloisField operator*(const GaloisField&) const;
  const GaloisField operator/(const GaloisField&) const;
  template <typename T>
  const GaloisField operator+(const T&) const;
  template <typename T>
  const GaloisField operator-(const T&) const;
  template <typename T>
  const GaloisField operator*(const T&) const;
  template <typename T>
  const GaloisField operator/(const T&) const;

  template <typename T>
  friend const GaloisField operator+(const T& first, const GaloisField& second);
  template <typename T>
  friend const GaloisField operator-(const T& first, const GaloisField& second);
  template <typename T>
  friend const GaloisField operator*(const T& first, const GaloisField& second);
  template <typename T>
  friend const GaloisField operator/(const T& first, const GaloisField& second);

  bool operator==(const GaloisField&) const;
  bool operator!=(const GaloisField&) const;

  GaloisField& operator+=(const GaloisField&);
  GaloisField& operator-=(const GaloisField&);
  GaloisField& operator*=(const GaloisField&);
  GaloisField& operator/=(const GaloisField&);

  template <typename T>
  GaloisField& operator+=(const T&);
  template <typename T>
  GaloisField& operator-=(const T&);
  template <typename T>
  GaloisField& operator*=(const T&);
  template <typename T>
  GaloisField& operator/=(const T&);

  static unsigned int inverse(unsigned int num) {
    return extended_euclide_inverse(num);
  }
  unsigned int number() const;

 private:
  const static unsigned int irr_poly = 0x11b;
  unsigned int number_;
  unsigned static int extended_euclide_inverse(unsigned int);
  unsigned static int add(unsigned int, unsigned int);
  unsigned static int sub(unsigned int, unsigned int);
  unsigned static int divide(unsigned int, unsigned int);
  unsigned static int multi(unsigned int, unsigned int);
  bool static is_carry(unsigned int);
  unsigned static int degree(unsigned int);
};

template <typename T>
GaloisField& GaloisField::operator=(const T& other) {
  number_ = other;
  return (*this);
}

template <typename T>
const GaloisField GaloisField::operator+(const T& other) const {
  return GaloisField(add(number_, other));
}

template <typename T>
const GaloisField GaloisField::operator-(const T& other) const {
  return GaloisField(sub(number_, other));
}

template <typename T>
const GaloisField GaloisField::operator*(const T& other) const {
  return GaloisField(multi(number_, other));
}

template <typename T>
const GaloisField GaloisField::operator/(const T& other) const {
  return GaloisField(divide(number_, other));
}

template <typename T>
GaloisField& GaloisField::operator+=(const T& other) {
  number_ = add(number_, other);
  return (*this);
}

template <typename T>
GaloisField& GaloisField::operator-=(const T& other) {
  number_ = sub(number_, other);
  return (*this);
}

template <typename T>
GaloisField& GaloisField::operator*=(const T& other) {
  number_ = multi(number_, other);
  return (*this);
}

template <typename T>
GaloisField& GaloisField::operator/=(const T& other) {
  number_ = divide(number_, other);
  return (*this);
}

// friend method

template <typename T>
const GaloisField operator+(const T& first, const GaloisField& second) {
  return first + second;
}

template <typename T>
const GaloisField operator-(const T& first, const GaloisField& second) {
  return first - second;
}

template <typename T>
const GaloisField operator*(const T& first, const GaloisField& second) {
  return first * second;
}

template <typename T>
const GaloisField operator/(const T& first, const GaloisField& second) {
  return first / second;
}

#endif