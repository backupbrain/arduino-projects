#ifndef Caller_h
#define Caller_h

class Caller {
public:
	Caller();
	~Caller();
	

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

private:
	void (*_remoteCallback)(int);

};




#endif