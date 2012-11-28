/*
 * NetworkException.h
 *
 *  Created on: 21 nov. 2012
 *      Author: bertrand
 */

#ifndef NETWORKEXCEPTION_H_
#define NETWORKEXCEPTION_H_

#include <exception>
#include <string>

namespace Network
{

class NetworkException: public std::exception
{
public:
	NetworkException();
	NetworkException(const char* message);

	virtual const char* what() const throw ()
	{
		return message.c_str();
	}
protected:
	std::string message;
};

} /* namespace Network */
#endif /* NETWORKEXCEPTION_H_ */
