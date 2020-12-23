#include <stdio.h>
#include <iostream>
#include "MyClass.h"
#include "Caller.h"

void callback(int param);

int main() {

	// build a MyClass,
	// attach a callback function
	MyClass* myClass = MyClass::getInstance();
	myClass->attachCallback(callback);

	myClass->triggerCallback(3);

	return 1;
}



void callback(int param) {
	printf("callback called with param: '%d'\r\n", param);
}