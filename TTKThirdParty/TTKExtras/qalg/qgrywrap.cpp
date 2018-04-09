#include "qgrywrap.h"

#define FLAG    "Gry"
#define END     "="

const std::string in_data = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const std::string out_data = "nIbGF4v+5w8WBaNfYOyAXSRzrh9D7cZKPUixsl0C/pmtguMjLkdV3E21qe6QHToJ";

QString QGryWrap::encrypt(const QString &in)
{
    QString d(in);
    int sCount = 0;

    while(true)
    {
        if(d.endsWith(END))
        {
            ++sCount;
            d.chop(1);
        }
        else
        {
            break;
        }
    }

    QString v;
    for(int i=d.length() - 1; i>=0; --i)
    {
        v.append(d[i]);
    }
    for(int i=0; i<v.length(); ++i)
    {
        int index = in_data.find(v[i].toLatin1());
        v[i] = out_data.at(index);
    }
    for(int i=0; i<sCount; ++i)
    {
        v.append(END);
    }

    v.insert(0, FLAG);

    return v;
}

QString QGryWrap::decrypt(const QString &in)
{
    QString d(in);
    int sCount = 0;

    while(true)
    {
        if(d.endsWith(END))
        {
            ++sCount;
            d.chop(1);
        }
        else
        {
            break;
        }
    }

    d.remove(0, 3);
    for(int i=0; i<d.length(); ++i)
    {
        int index = out_data.find(d[i].toLatin1());
        d[i] = in_data.at(index);
    }

    QString v;
    for(int i=d.length() - 1; i>=0; --i)
    {
        v.append(d[i]);
    }
    for(int i=0; i<sCount; ++i)
    {
        v.append(END);
    }

    return v;
}
