#ifndef NETWORKENCRYPTION_H_
#define NETWORKENCRYPTION_H_

#include "Util/Singleton.h"

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

namespace Network
{

class NetworkEncryption : public Util::Singleton<NetworkEncryption>
{
    friend Util::Singleton<NetworkEncryption>;
public:
    virtual ~NetworkEncryption();

    const std::pair<char*, short>& GetCertificate() const
    {
        return certificate;
    }

    const CryptoPP::RSA::PublicKey* getPublicKey() const
    {
        return publicKey;
    }

    const CryptoPP::RSA::PrivateKey* getPrivateKey() const
    {
        return privateKey;
    }

    CryptoPP::AutoSeededRandomPool& getAutoSeed()
    {
        return autoSeed;
    }

private:
    NetworkEncryption();

    CryptoPP::AutoSeededRandomPool autoSeed;
    CryptoPP::RSA::PrivateKey* privateKey;
    CryptoPP::RSA::PublicKey* publicKey;
    std::pair<char*,short> certificate;
};

} /* namespace Network */
#endif /* NETWORKENCRYPTION_H_ */
