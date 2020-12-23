
#ifndef CIRCULAR_ARRAY_H_
#define CIRCULAR_ARRAY_H_

#include <Arduino.h>

#include <Vector.h>

template<typename Data>
class CircularArray {
 public:
  CircularArray(int count) : max_size_(count) {
  }

  void push_back(const Data& d) {
    while (data_.size() >= max_size_) {
      data_.erase(data_.begin());
    }
    data_.push_back(d);
  }

  void pop_back() {
    data_.pop_back();
  }

  void clear() {
    while (0 != size()) {
      pop_back();
    }
  }

  Vector<Data>& data() { return data_; }
  const Vector<Data>& data() const { return data_; }
  size_t size() const { return data_.size(); }

  bool is_full() const { return size() == max_size_; }


  Data const &operator[](size_t idx) const { return data_[idx]; }; // Const getter
  Data &operator[](size_t idx) { return data_[idx]; }; // Changeable getter
  Data& back() { return data_[size() - 1]; }
  const Data& back() const { return data_[size() - 1]; }

 private:
  Vector<Data> data_;
  size_t max_size_;
};


void TestCircularArray() {

  Serial.println("Testing TestCircularArray!");

  CircularArray<int> cyc_int_array(4);

  Serial.print("cyc_int_array.MyArraySize()");
  Serial.println(cyc_int_array.data().size());

  cyc_int_array.push_back(4);
  cyc_int_array.push_back(3);
  cyc_int_array.push_back(2);
  cyc_int_array.push_back(1);
  cyc_int_array.push_back(1);
  cyc_int_array.push_back(1);

  Serial.print("cyc_int_array.MyArraySize()");
  Serial.println(cyc_int_array.size());

  // Serial.begin(9600);
  for (int i = 0; i < cyc_int_array.data().size(); ++i) {
    Serial.print("cyc_int_array.At(");
    Serial.print(i);
    Serial.print("): ");
    Serial.println(cyc_int_array[i]);
  }

}

#endif  // CIRCULAR_ARRAY_H_