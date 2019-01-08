#include "qaeswrap.h"
#include "base64.h"
extern "C" {
#include "aes.h"
}

#define DATA_CACHE_SIZE      1024

QAesWrap::QAesWrap()
{

}

QByteArray QAesWrap::encryptECB(const QByteArray &in, const QByteArray &key)
{
    AES_KEY aes;
    char encrypt_string[DATA_CACHE_SIZE] = {0};
    const int nLen = in.length();
    const int nBei = nLen / AES_BLOCK_SIZE + 1;
    const int nTotal = nBei * AES_BLOCK_SIZE;
    char *enc_str = new char[nTotal + 1]();

    const int nNumber = (nLen % AES_BLOCK_SIZE > 0) ? nTotal - nLen : AES_BLOCK_SIZE;

    memset(enc_str, nNumber, nTotal);
    memcpy(enc_str, in.data(), nLen);

    if(AES_set_encrypt_key((unsigned char *)key.data(), 128, &aes) < 0)
    {
        return QByteArray();
    }

    AES_ecb_encrypt((unsigned char *)enc_str, (unsigned char *)encrypt_string, &aes, AES_ENCRYPT);
    delete[] enc_str;

    enc_str = Base64Encode((const unsigned char *)encrypt_string, nTotal);
    QByteArray data(enc_str);

    free(enc_str);

    return data;
}

QByteArray QAesWrap::decryptECB(const QByteArray &in, const QByteArray &key)
{
    AES_KEY aes;
    char decrypt_string[DATA_CACHE_SIZE] = {0};
    char *decode = Base64Decode((const unsigned char *)in.data(), in.length());

    if(AES_set_decrypt_key((unsigned char *)key.data(), 128, &aes) < 0)
    {
        return QByteArray();
    }
    AES_ecb_encrypt((unsigned char *)decode, (unsigned char *)decrypt_string, &aes, AES_DECRYPT);

    free(decode);

    return QByteArray(QString(decrypt_string).remove("\x0F").toUtf8());
}

QByteArray QAesWrap::encryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv)
{
    AES_KEY aes;
    char encrypt_string[DATA_CACHE_SIZE] = {0};
    const int nLen = in.length();
    const int nBei = nLen / AES_BLOCK_SIZE + 1;
    const int nTotal = nBei * AES_BLOCK_SIZE;
    char *enc_str = new char[nTotal + 1]();

    const int nNumber = (nLen % AES_BLOCK_SIZE > 0) ? nTotal - nLen : AES_BLOCK_SIZE;

    memset(enc_str, nNumber, nTotal);
    memcpy(enc_str, in.data(), nLen);

    if(AES_set_encrypt_key((unsigned char *)key.data(), 128, &aes) < 0)
    {
        return QByteArray();
    }

    AES_cbc_encrypt((unsigned char *)enc_str, (unsigned char *)encrypt_string, nTotal, &aes, (unsigned char *)iv.data(), AES_ENCRYPT);
    delete[] enc_str;

    enc_str = Base64Encode((const unsigned char *)encrypt_string, nTotal);
    QByteArray data(enc_str);

    free(enc_str);

    return data;
}

QByteArray QAesWrap::decryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv)
{
    AES_KEY aes;
    char decrypt_string[DATA_CACHE_SIZE] = {0};
    char *decode = Base64Decode((const unsigned char *)in.data(), in.length());

    if(AES_set_decrypt_key((unsigned char *)key.data(), 128, &aes) < 0)
    {
        return QByteArray();
    }
    AES_cbc_encrypt((unsigned char *)decode, (unsigned char *)decrypt_string, strlen(decode), &aes, (unsigned char *)iv.data(), AES_DECRYPT);

    free(decode);

    return QByteArray(QString(decrypt_string).remove("\x0F").toUtf8());
}
