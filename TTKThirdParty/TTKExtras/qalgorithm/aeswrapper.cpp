#include "aeswrapper.h"
#include "aes.h"

static constexpr int DATA_CACHE_SIZE = 1024;

QByteArray PKCS7Padding(const QByteArray &in, int alignSize)
{
    // 计算需要填充字节数（按alignSize字节对齐进行填充）
    int remainder = in.size() % alignSize;
    int paddingSize = (remainder == 0) ? alignSize : (alignSize - remainder);

    // 进行填充
    QByteArray temp(in);
    temp.append(paddingSize, paddingSize);
    return temp;
}


QByteArray QAlgorithm::Aes::encryptECB(const QByteArray &in, const QByteArray &key, bool hex)
{
    AES_KEY aes;
    if(AES_set_encrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        return {};
    }

    const int remd = in.size() % AES_BLOCK_SIZE;
    const int number = (remd == 0) ? AES_BLOCK_SIZE : (AES_BLOCK_SIZE - remd);

    // 进行填充
    QByteArray buffer(in);
    buffer.append(number, number);
    QByteArray result(buffer.size(), '0');

    for (int i = 0; i < buffer.length() / AES_BLOCK_SIZE; i++)
    {
        AES_ecb_encrypt((const unsigned char*)buffer.data() + AES_BLOCK_SIZE * i,
                        (unsigned char*)result.data() + AES_BLOCK_SIZE * i,
                        &aes,
                        AES_ENCRYPT);
    }

    return hex ? result.toHex() : result.toBase64();
}

QByteArray QAlgorithm::Aes::decryptECB(const QByteArray &in, const QByteArray &key, bool hex)
{
    AES_KEY aes;
    if(AES_set_decrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        return {};
    }

    const QByteArray &buffer = hex ? QByteArray::fromHex(in) : QByteArray::fromBase64(in);
    QByteArray result(buffer.length(), '0');

    for (int i = 0; i < buffer.length() / AES_BLOCK_SIZE; i++)
    {
        AES_ecb_encrypt((const unsigned char*)buffer.data() + AES_BLOCK_SIZE * i,
                        (unsigned char*)result.data() + AES_BLOCK_SIZE * i,
                        &aes,
                        AES_DECRYPT);
    }

    const char padding = result.at(result.size() - 1);
    return result.left(result.size() - padding);
}

QByteArray QAlgorithm::Aes::encryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv, bool hex)
{
    AES_KEY aes;
    if(AES_set_encrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        return {};
    }

    const int length = in.length();
    const int total = (length / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    const int number = (length % AES_BLOCK_SIZE > 0) ? total - length : AES_BLOCK_SIZE;

    char buffer[DATA_CACHE_SIZE] = {0};
    char *input = new char[total + 1]();

    memset(input, number, total);
    memcpy(input, in.data(), length);

    AES_cbc_encrypt((const unsigned char *)input, (unsigned char *)buffer, total, &aes, (unsigned char *)iv.data(), AES_ENCRYPT);
    delete[] input;

    const QByteArray result((const char *)buffer, total);
    return hex ? result.toHex() : result.toBase64();
}

QByteArray QAlgorithm::Aes::decryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv, bool hex)
{
    AES_KEY aes;
    if(AES_set_decrypt_key((const unsigned char *)key.data(), 128, &aes) < 0)
    {
        return {};
    }

    char buffer[DATA_CACHE_SIZE] = {0};
    const QByteArray &input = hex ? QByteArray::fromHex(in) : QByteArray::fromBase64(in);

    AES_cbc_encrypt((const unsigned char *)input.data(), (unsigned char *)buffer, input.length(), &aes, (unsigned char *)iv.data(), AES_DECRYPT);

    QByteArray result(buffer);
    const char padding = result.at(result.size() - 1);
    return result.left(result.size() - padding);
}
