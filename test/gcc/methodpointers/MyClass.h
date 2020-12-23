#ifndef MyClass_h
#define MyClass_h

#include "Caller.h"

class MyClass {
public:
	static MyClass* getInstance();

	/**
	 * attach a callback to be called when
	 * triggerCallback is called
	 *
	 * @param callback[in] the callback to call
	 */
	void attachCallback(void (*callback)(int));

	/** 
	 * This trigger triggers a local callback
	 * which in turn triggers the remote callback, 
	 * defined by attachCallback()
	 *
	 * @param param[in] the value to pass through the callback system
	 */
	void triggerCallback(int param);

	/**
	 * this callback is triggered by caller
	 *
	 * @param param[in] the parameter to pass
	 */
	static void localCallback(int);

private:
	MyClass();
	virtual ~MyClass();
    //MyClass(const MyClass&)=delete; 
    MyClass& operator=(const MyClass&)=delete;

	void (*_remoteCallback)(int);

	Caller* _caller;

};




#endif