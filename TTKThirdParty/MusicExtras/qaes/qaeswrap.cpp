#include "qaeswrap.h"
extern "C" {
#include "aes.h"
}

QByteArray QAesWrap::encrypt(const QString &in, const QString &key, const QString &iv)
{
    AES_KEY aes;
    char encrypt_string[1024] = {0};
    int nLen = in.length();
    int nBei = nLen / AES_BLOCK_SIZE + 1;
    int nTotal = nBei * AES_BLOCK_SIZE;
    char *enc_s = new char[nTotal + 1]();

    int nNumber = (nLen % 16 > 0) ? nTotal - nLen :16;

    memset(enc_s, nNumber, nTotal);
    memcpy(enc_s, in.toStdString().data(), nLen);
    if(AES_set_encrypt_key((unsigned char *)key.toStdString().data(), 128, &aes) < 0)
    {
        return QByteArray();
    }

    AES_cbc_encrypt((unsigned char *)enc_s, (unsigned char *)encrypt_string, nTotal,
                    &aes,
                    (unsigned char *)iv.toStdString().data(), AES_ENCRYPT);
    delete enc_s;
    return QByteArray(encrypt_string).toBase64();
}

QByteArray QAesWrap::decrypt(const QByteArray &in, const QString &key, const QString &iv)
{
    AES_KEY aes;
    char encrypt_string[1024] = {0};
    const char *decode = QString(QByteArray::fromBase64(in)).toStdString().data();

    if(AES_set_decrypt_key((unsigned char *)key.toStdString().data(), 128, &aes) < 0)
    {
        return QByteArray();
    }
    AES_cbc_encrypt((unsigned char *)decode, (unsigned char *)encrypt_string, strlen(decode),
                    &aes,
                    (unsigned char *)iv.toStdString().data(), AES_DECRYPT);

    return QByteArray(QString(encrypt_string).remove("\x0F").toUtf8());
}
