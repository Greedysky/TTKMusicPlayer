#include "aeswrapper.h"
#include "base64.h"
#include "aes.h"

static constexpr int DATA_CACHE_SIZE = 1024;

QByteArray QAlgorithm::Aes::encryptECB(const QByteArray &in, const QByteArray &key)
{
    AES_KEY aes;
    char encryptString[DATA_CACHE_SIZE] = {0};
    const int length = in.length();
    const int bei = length / AES_BLOCK_SIZE + 1;
    const int total = bei * AES_BLOCK_SIZE;
    char *encStr = new char[total + 1]();

    const int nNumber = (length % AES_BLOCK_SIZE > 0) ? total - length : AES_BLOCK_SIZE;

    memset(encStr, nNumber, total);
    memcpy(encStr, in.data(), length);

    if(AES_set_encrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        delete[] encStr;
        return {};
    }

    AES_ecb_encrypt((const unsigned char *)encStr, (unsigned char *)encryptString, &aes, AES_ENCRYPT);
    delete[] encStr;

    const TTKString &str = QAlgorithm::base64Encode((const unsigned char *)encryptString, total);
    return QByteArray(str.data(), str.length());
}

QByteArray QAlgorithm::Aes::decryptECB(const QByteArray &in, const QByteArray &key)
{
    AES_KEY aes;
    char decryptString[DATA_CACHE_SIZE] = {0};
    const TTKString &str = QAlgorithm::base64Decode(TTKString(in.data(), in.length()));

    if(AES_set_decrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        return {};
    }
    AES_ecb_encrypt((const unsigned char *)str.data(), (unsigned char *)decryptString, &aes, AES_DECRYPT);

    return QByteArray(QString(decryptString).remove("\x0F").toUtf8());
}

QByteArray QAlgorithm::Aes::encryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv)
{
    AES_KEY aes;
    char encryptString[DATA_CACHE_SIZE] = {0};
    const int length = in.length();
    const int bei = length / AES_BLOCK_SIZE + 1;
    const int total = bei * AES_BLOCK_SIZE;
    char *encStr = new char[total + 1]();

    const int nNumber = (length % AES_BLOCK_SIZE > 0) ? total - length : AES_BLOCK_SIZE;

    memset(encStr, nNumber, total);
    memcpy(encStr, in.data(), length);

    if(AES_set_encrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        delete[] encStr;
        return {};
    }

    AES_cbc_encrypt((const unsigned char *)encStr, (unsigned char *)encryptString, total, &aes, (unsigned char *)iv.data(), AES_ENCRYPT);
    delete[] encStr;

    const TTKString &str = QAlgorithm::base64Encode((const unsigned char *)encryptString, total);
    return QByteArray(str.data(), str.length());
}

QByteArray QAlgorithm::Aes::decryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv)
{
    AES_KEY aes;
    char decryptString[DATA_CACHE_SIZE] = {0};
    const TTKString &str = QAlgorithm::base64Decode(TTKString(in.data(), in.length()));

    if(AES_set_decrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        return {};
    }
    AES_cbc_encrypt((const unsigned char *)str.data(), (unsigned char *)decryptString, str.length(), &aes, (unsigned char *)iv.data(), AES_DECRYPT);

    return QByteArray(QString(decryptString).remove("\x0F").toUtf8());
}
