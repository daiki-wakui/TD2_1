#include "Input.h"

bool Input::TriggerReleseKey(BYTE keyNumber) const
{
	return !key_[keyNumber] && keyPre_[keyNumber];
}
