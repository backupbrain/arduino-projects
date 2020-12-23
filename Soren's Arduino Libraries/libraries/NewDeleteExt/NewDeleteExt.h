
#ifndef NEW_DELETE_EXT_H_
#define NEW_DELETE_EXT_H_

#include <Arduino.h>

void* operator new(size_t, void* ptr) { return ptr; }

#endif  // NEW_DELETE_EXT_H_
