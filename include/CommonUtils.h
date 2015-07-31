/*
 * CommonUtils.h
 *
 *  Created on: 2015年7月19日
 *      Author: weimi_000
 */

#ifndef INCLUDE_COMMONUTILS_H_
#define INCLUDE_COMMONUTILS_H_

#include <iostream>
#include <stdlib.h>

enum ValidateType {
	VALIDATE_COMMON,
	VALIDATE_ALL
};

template <typename T>
bool setString(const std::string & s, void (T::*setter)(std::string *), T & t) {
	std::string * _p = new std::string(s);

	if (_p == NULL) {
		std::cerr << "Can't allocate. new(" << s << ") failed." << std::endl;
		return false;
	} else {
		(t.*setter)(_p);
		return true;
	}
}




#endif /* INCLUDE_COMMONUTILS_H_ */
