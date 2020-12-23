#ifndef VECTOR_H_
#define VECTOR_H_

#include <Arduino.h>
#include "NewDeleteExt.h"

// Minimal class to replace std::vector
template<typename Data>
class Vector {
 public:
  Vector() : d_size(0), d_capacity(0), d_data(0) {}; // Default constructor
  explicit Vector(int sz) : d_size(0), d_capacity(0), d_data(0) {
    d_size = sz;
    d_capacity = sz;
    d_data = static_cast<Data*>(malloc(sz * sizeof(Data)));
    for (int i = 0; i < d_size; ++i) {
      InitElement(&d_data[i]);
    }
  }
  Vector(Vector const &other)
      : d_size(other.d_size),
        d_capacity(other.d_capacity),
        d_data(0) {
    d_data = static_cast<Data*>(malloc(d_capacity * sizeof(Data)));
    for (int i = 0; i < other.d_size; ++i) {
      d_data[i] = other.d_data[i];
    }
  }; // Copy constuctor
  ~Vector() {
    clear();
    free(d_data);
  }; // Destructor
  Vector &operator=(Vector const &other) {
    CopyFrom(other);
    return *this;
  } // Needed for memory management

  void clear() {
    while (d_size) { pop_back(); }
  }

  void pop_back() {
    if (d_size) {
      DestroyElement(&back());
      --d_size;
    }
  }

  void DestroyElement(Data* d) {
    d->~Data();
  }

  void InitElement(Data* d) {
    void* raw_data = d;
    new(raw_data) Data();
  }

  // Adds new value. If needed, allocates more space
  void push_back(const Data& x) {
    if (d_capacity == d_size)
      EnsureCapacity();
    d_data[d_size++] = x;
  };

  void erase(const Data& d) {
    for (int i = 0; i < d_size; ++i) {
      if (d == d_data[i]) {
        erase(&d_data[i]);
      }
    }
  }

  typedef Data* iterator;
  typedef const Data* const_iterator;

  void erase(iterator pos) {
    if (pos >= end())
      return;
    DestroyElement(pos);
    InitElement(pos);

    iterator next = pos;
    ++next;
    while (next != end()) {
      *pos = *next;
      ++pos;
      ++next;
    }
    DestroyElement(&back());
    --d_size;
  }

  size_t size() const { return d_size; }; // Size getter
  bool empty() const { return d_size == 0; }
  const_iterator end() const {
    if (!d_size) {
      return NULL;
    }
    return &back() + 1;
  }

  iterator begin() {
    if (!d_size) {
      return NULL;
    }
    return &d_data[0];
  }

  Data const &operator[](size_t idx) const { return d_data[idx]; }; // Const getter
  Data &operator[](size_t idx) { return d_data[idx]; }; // Changeable getter
  Data& back() { return d_data[d_size - 1]; }
  const Data& back() const { return d_data[d_size - 1]; }
 private:
  void EnsureCapacity() {
    d_capacity = d_capacity ? d_capacity * 2 : 1;
    Data* newdata = (Data*) malloc(sizeof(Data) * d_capacity);
    for (int i = 0; i < d_size; ++i) {
      InitElement(&newdata[i]);
    }
    for (int i = 0; i < d_size; ++i) {
      newdata[i] = d_data[i];
      DestroyElement(&d_data[i]);
    }
    free(d_data);
    d_data = newdata;
  } // Allocates double the old space

  void CopyFrom(const Vector& other) {
    delete[] d_data;
    d_data = NULL;
    d_size = other.d_size;
    d_capacity = d_size;
    if (!d_size) {
      return;
    }
    d_data = new Data[d_size];
    for (int i = 0; i < d_size; ++i) {
      d_data[i] = other.d_data[i];
    }
  }

  size_t d_size; // Stores no. of actually stored objects
  size_t d_capacity; // Stores allocated capacity
  Data *d_data; // Stores data
};

#endif  // VECTOR_H_
