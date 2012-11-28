/*
 * NetworkEncryption.cpp
 *
 *  Created on: 26 nov. 2012
 *      Author: bertrand
 */

#include "NetworkEncryption.h"
#include <cryptopp/files.h>
#include <cryptopp/integer.h>
#include <sstream>

namespace Network
{

NetworkEncryption* NetworkEncryption::instance = NULL;

NetworkEncryption* NetworkEncryption::GetInstance()
{
	if (instance == NULL)
		instance = new NetworkEncryption();
	return instance;
}

NetworkEncryption::NetworkEncryption()
{
	std::stringstream ss;

	privateKey = new CryptoPP::RSA::PrivateKey();
	CryptoPP::Integer i(65537);
	privateKey->Initialize(autoSeed,1024,i);
	publicKey = new CryptoPP::RSA::PublicKey(*privateKey);
	CryptoPP::FileSink fileSink(ss);
	publicKey->Save(fileSink);


	std::string rStr = ss.str();
	if (rStr.length() == 0)
	{
		std::cerr << "Error while generating RSA";
	}

	char *pBuffer = new char[rStr.length()];
	memcpy(pBuffer,rStr.c_str(),rStr.length());

	std::cout << "rStr.length():" << rStr.length() << std::endl;
	certificate = std::make_pair(pBuffer,(short)rStr.length());
}

NetworkEncryption::~NetworkEncryption()
{

}

} /* namespace Network */
