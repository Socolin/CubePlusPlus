#include "NetworkEncryption.h"
#include <cryptopp/files.h>
#include <cryptopp/integer.h>
#include <sstream>

#include "Logging/Logger.h"

namespace Network
{

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
        LOG_ERROR << "Error while generating RSA";
    }

    char *pBuffer = new char[rStr.length()];
    memcpy(pBuffer,rStr.c_str(),rStr.length());

    LOG_DEBUG << "rStr.length():" << rStr.length() << std::endl;
    certificate = std::make_pair(pBuffer,(short)rStr.length());
}

NetworkEncryption::~NetworkEncryption()
{
    delete privateKey;
    delete publicKey;
    privateKey = nullptr;
    publicKey = nullptr;
    delete[] certificate.first;
    certificate.first = nullptr;
}

} /* namespace Network */
