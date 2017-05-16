#ifndef _XML_OOXMLVERIFIER_H_
#define _XML_OOXMLVERIFIER_H_

#include "./XmlCertificate.h"

#define OOXML_SIGNATURE_VALID           0
#define OOXML_SIGNATURE_INVALID         1
#define OOXML_SIGNATURE_NOTSUPPORTED    2
#define OOXML_SIGNATURE_BAD             3

class COOXMLSignature_private;
class Q_DECL_EXPORT COOXMLSignature
{
public:
    COOXMLSignature();
    ~COOXMLSignature();

public:
    int GetValid();
    std::string GetGuid();
    ICertificate* GetCertificate();
    std::string GetImageValidBase64();
    std::string GetImageInvalidBase64();

public:
    void Check();

    friend class COOXMLVerifier_private;
    friend class COOXMLVerifier;
private:
    COOXMLSignature_private* m_internal;
};

class COOXMLVerifier_private;
class Q_DECL_EXPORT COOXMLVerifier
{
public:
    COOXMLVerifier(const std::wstring& sFolder);
    ~COOXMLVerifier();

    int GetSignatureCount();
    COOXMLSignature* GetSignature(const int& index);

private:
    COOXMLVerifier_private* m_internal;
};

#endif //_XML_OOXMLVERIFIER_H_
