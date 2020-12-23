#include "MyClass.h"


MyClass::MyClass() {
	_caller = new Caller();
	_caller->attachCallback(localCallback);
}
MyClass::~MyClass() { 
	delete _caller;
}

MyClass* MyClass::getInstance() {
	static MyClass* instance = new MyClass();
	return instance;
}


void MyClass::attachCallback(void (*callback)(int)) {
	_remoteCallback = callback;
}

void MyClass::triggerCallback(int param) {
	_caller->triggerCallback(param);
}


void MyClass::localCallback(int param) {
	MyClass* instance = MyClass::getInstance();
	instance->_remoteCallback(param);
}
