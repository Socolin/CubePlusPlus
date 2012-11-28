#ifndef NETWORKENCRYPTION_H_
#define NETWORKENCRYPTION_H_

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

namespace Network
{

class NetworkEncryption
{
	static NetworkEncryption* instance;
public:
	static NetworkEncryption* GetInstance();
	virtual ~NetworkEncryption();

	const std::pair<char*, short>& GetCertificate() const
	{
		return certificate;
	}

	const CryptoPP::RSA::PublicKey*& getPublicKey() const
	{
		return publicKey;
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
