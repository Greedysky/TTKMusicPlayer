#include "qaeswrap.h"

QAesWrap::QAesWrap(const QByteArray &passwprd, const QByteArray &salt, AesBit bit)
    : mbit(bit)
{
    aes_key_setup((unsigned char *)passwprd.data(), mpass, mbit);
    memcpy(msalt, salt.data(), AES_BLOCK_SIZE);
}

bool QAesWrap::encrypt(const QByteArray &in, QByteArray &out, AesMode mode, PaddingMode pad) const
{
    if (in.isEmpty()) return false;
    out.clear();
    switch (mode) {
    case AES_CTR:
        out.resize(in.size());
        aes_encrypt_ctr((unsigned char *)in.data(),in.size(),(unsigned char *)out.data(),mpass,mbit,msalt);
        break;
    case AES_ECB:
    case AES_CBC:
        initPadding(in,out,mode,pad);
        break;
    default:
        return false;
    }
    return true;
}

void QAesWrap::initPadding(const QByteArray &in, QByteArray &out, AesMode mode, PaddingMode pad) const
{
    int size = in.size();
    int last = size % AES_BLOCK_SIZE;
    const BYTE * data = (unsigned char *)in.data();
    if (last == 0) {
        out.resize(size);
        if (mode == AES_ECB) {
            ecbencrypt(data,size,0,out);
        } else {
            aes_encrypt_cbc(data,size,0,(unsigned char *)out.data(),mpass,mbit,msalt);
        }
        return;
    }
    int blocks = size / AES_BLOCK_SIZE;

    BYTE datablocks[AES_BLOCK_SIZE] = {0};
    memcpy(datablocks,(data + blocks * AES_BLOCK_SIZE),last);

    uchar ch = uchar(AES_BLOCK_SIZE - last);

    switch (pad) {
    case ANSIX923:
    case PKCS7:
    {
        size  = blocks * AES_BLOCK_SIZE;
        out.resize((blocks +1) * AES_BLOCK_SIZE);
        if (pad == ANSIX923) {
            memset(&datablocks[last],0,(ch -1));
            datablocks[AES_BLOCK_SIZE -1] = ch;
        } else {
            memset(&datablocks[last],ch,ch);
        }

        if (mode == AES_ECB) {
            ecbencrypt(data,size,datablocks,out);
        } else {
            aes_encrypt_cbc(data,size,datablocks,(unsigned char *)out.data(),mpass,mbit,msalt);
        }
    }
        break;
    default:
    {
        if (blocks <= 0) {out = in; return;}
        out.resize(size);
        size  = blocks * AES_BLOCK_SIZE;
        if (mode == AES_ECB) {
            ecbencrypt(data,size,0,out);
        } else {
            aes_encrypt_cbc(data,size,0,(unsigned char *)out.data(),mpass,mbit,msalt);
        }
        for (int i = 0,j = size; i < last; ++i, ++j) {
            out[j] = datablocks[i];
        }
    }
        break;
    }
}

void QAesWrap::ecbencrypt(const BYTE * in, size_t size, BYTE *blcok, QByteArray & out) const
{
    BYTE buf_in[AES_BLOCK_SIZE] = {0}, buf_out[AES_BLOCK_SIZE] = {0};
    int blocks, idx;
    char * data = out.data();
    blocks = size / AES_BLOCK_SIZE;
    memset(buf_out,0,AES_BLOCK_SIZE);
    for (idx = 0; idx < blocks; idx++) {
        memcpy(buf_in, &in[idx * AES_BLOCK_SIZE], AES_BLOCK_SIZE);
        aes_encrypt(buf_in, buf_out, mpass, mbit);
        memcpy(&data[idx * AES_BLOCK_SIZE], buf_out, AES_BLOCK_SIZE);
        memset(buf_out,0,AES_BLOCK_SIZE);
    }
    if (blcok) {
        memcpy(buf_in, blcok, AES_BLOCK_SIZE);
        aes_encrypt(blcok, buf_out, mpass, mbit);
        memcpy(&data[idx * AES_BLOCK_SIZE], buf_out, AES_BLOCK_SIZE);
    }
}

void QAesWrap::ecbdecrypt(const BYTE *in, size_t size, QByteArray & out) const
{
    BYTE buf_in[AES_BLOCK_SIZE] = {0}, buf_out[AES_BLOCK_SIZE] = {0};
    int blocks, idx;
    char * data = out.data();
    blocks = size / AES_BLOCK_SIZE;
    memset(buf_out,0,AES_BLOCK_SIZE);
    for (idx = 0; idx < blocks; idx++) {
        memcpy(buf_in, &in[idx * AES_BLOCK_SIZE], AES_BLOCK_SIZE);
        aes_decrypt(buf_in, buf_out, mpass, mbit);
        memcpy(&data[idx * AES_BLOCK_SIZE], buf_out, AES_BLOCK_SIZE);
        memset(buf_out,0,AES_BLOCK_SIZE);
    }
}

bool QAesWrap::decrypt(const QByteArray & in, QByteArray & out, AesMode mode,PaddingMode pad) const
{
    if (in.isEmpty()) return false;
    out.clear();
    int size = in.size();
    out.resize(size);
    if (mode == AES_CTR) {
        aes_encrypt_ctr((unsigned char *)in.data(),size,(unsigned char *)out.data(),mpass,mbit,msalt);
        return true;
    }
    int last = size % AES_BLOCK_SIZE;
    int blocks = size / AES_BLOCK_SIZE;
    int useSize = blocks * AES_BLOCK_SIZE;
    if (blocks == 0 ) {
        out = in;
        return false;
    }
    QByteArray tdata;
    if (last > 0) {
        if(pad == ANSIX923 || pad == PKCS7) {
            out.clear();
            return false;
        }
        tdata = in.right(last);
    }


    switch (mode) {
    case AES_ECB:
        ecbdecrypt((unsigned char *)in.data(),useSize,out);
        break;
    case AES_CBC:
        aes_decrypt_cbc((unsigned char *)in.data(),useSize,(unsigned char *)out.data(),mpass,mbit,msalt);
        break;
    default:
        return false;
    }
    if (last > 0) {
        for (int i = useSize, j = 0;i < size; ++i,++j) {
            out[i] = tdata.at(j);
        }
    } else {
        char ch = out.at(size-1);
        if (ch < 16) {
            int removed = 0;
            int tsize = size - ch;
            bool isPad = true;
            if (pad == ANSIX923) {
                int mysize = size - 1;
                char tch;
                for (int i = tsize; i < mysize; ++i) {
                    tch = out.at(i);
                    if (tch != char(0x00)) {
                        isPad = false;
                    }
                }
                if (isPad) removed = ch;
            }else {
                char tch;
                for (int i = tsize; i < size; ++i) {
                    tch = out.at(i);
                    if (tch != ch) {
                        isPad = false;
                    }
                }
                if (isPad) removed = ch;
            }
            if (removed > 0) {
                out.remove(size - removed,removed);
            }
        }
    }

    return true;
}
