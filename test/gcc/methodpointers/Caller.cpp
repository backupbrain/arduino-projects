#include "Caller.h"

Caller::Caller() {

}
Caller::~Caller() {}

void Caller::attachCallback(void (*callback)(int)) {
	_remoteCallback = callback;
}
void Caller::triggerCallback(int param) {
	_remoteCallback(param);
}
