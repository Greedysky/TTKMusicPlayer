#include "charchecker.h"

#include <vector>
#include <functional>

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wtype-limits"
#endif

class CheckerBase
{
public:
    CheckerBase(const std::string &name);
    virtual ~CheckerBase();

    virtual bool detect(const std::string &str) const = 0;

private:
    std::string m_name;
};


CheckerBase::CheckerBase(const std::string &name)
    : m_name(name)
{

}

CheckerBase::~CheckerBase()
{

}

///////////////////////////////////////////////////////////////////////////////////////////
class AsciiChecker : public CheckerBase
{
public:
    AsciiChecker();

    virtual bool detect(const std::string &str) const override;

};

AsciiChecker::AsciiChecker()
    : CheckerBase("ascii")
{

}

bool AsciiChecker::detect(const std::string &str) const
{
    for(unsigned char ch : str)
    {
        if(ch >> 7) // whether the first bit is 1
        {
            return false;
        }
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////////////////
class GB18030Checker : public CheckerBase
{
public:
    GB18030Checker();
    virtual bool detect(const std::string &str) const override;

};

#define GB18030_two_byte_func(a, b, c, d) \
    [](const unsigned char *s) \
    { \
        return *s >= a && *s <= b && s[1] >= c && s[1] <= d; \
    }

#define GB18030_four_byte_func(min, max) \
    [](const unsigned char *s) \
    { \
        return GB18030_two_byte_func(0x81, 0x82, 0x30, 0x39)(s) && \
               GB18030_two_byte_func(0x81, 0xFE, 0x30, 0x39)(s + 2); \
    }

const std::vector< std::pair <int, std::function<bool(const unsigned char*)> > > GB18030_Detect = {
        {1, [](const unsigned char *s){ return *s <= 0x7F;} },
        {2, GB18030_two_byte_func(0xB0, 0xF7, 0xA1, 0xFE)},
        {2, GB18030_two_byte_func(0x81, 0xA0, 0x40, 0xFE)},
        {2, GB18030_two_byte_func(0xAA, 0xFE, 0x40, 0xA0)},
        {4, GB18030_four_byte_func(0x81, 0x82)},
        {4, GB18030_four_byte_func(0x95, 0x98)} };


GB18030Checker::GB18030Checker()
    : CheckerBase("gb18030")
{

}

bool GB18030Checker::detect(const std::string &str) const
{
    int index = -1;
    int length = str.length();
    const unsigned char* buffer = (const unsigned char*)str.c_str();
    while(index + 1 < length)
    {
        bool flag = false;
        int leftLength = length - index - 1;
        for(auto func : GB18030_Detect)
        {
            if(func.first <= leftLength && func.second(buffer))
            {
                index += func.first;
                buffer += func.first;
                flag = true;
                break;
            }
        }

        if(!flag)
        {
            break;
        }
    }
    return (index + 1 == length);
}
///////////////////////////////////////////////////////////////////////////////////////////

class GBKChecker : public CheckerBase
{
public:
    GBKChecker();
    virtual bool detect(const std::string &str) const override;

private:
    bool checkTwoBytes(const unsigned char *buffer) const;

};

GBKChecker::GBKChecker()
    : CheckerBase("gbk")
{

}

bool GBKChecker::detect(const std::string &str) const
{
    int index = -1;
    int length = str.length();
    const unsigned char* buffer = (const unsigned char*)str.c_str();
    while(index + 1 < length)
    {
        if(*buffer <= 0x7F)
        {
            index += 1;
            buffer += 1;
        }

        if(checkTwoBytes(buffer))
        {
            index += 2;
            buffer += 2;
        }
        else
        {
            break;
        }
    }
    return (index + 1 == length);
}

bool GBKChecker::checkTwoBytes(const unsigned char *buffer) const
{
    bool fValid = (*buffer >= 0x81 && *buffer <= 0xFE);
    ++buffer;
    bool sValid = (*buffer >= 40 && *buffer <= 0xFE && *buffer != 0x7F);
    return fValid && sValid;
}
///////////////////////////////////////////////////////////////////////////////////////////

class UTF8Checker : public CheckerBase
{
public:
    UTF8Checker();
    virtual bool detect(const std::string &str) const override;

};

bool bytesExtraCheck(const unsigned char* str, int length)
{
    for(int i=0; i<length; ++i)
    {
        if((str[i] & 0xC0) != 0x80)
        {
            return false;
        }
    }
    return true;
}

#define multibyte_utf8(a, b, num) \
    [](const unsigned char *s, int len) \
    { \
        return ((*s)&a) == b && len >= num && bytesExtraCheck(s + 1, num - 1); \
    }

const std::map<int, std::function<bool(const unsigned char *, int) > >UTF8_Detect = {
                        {1, multibyte_utf8(0x80, 0x00, 1)},
                        {2, multibyte_utf8(0xE0, 0xC0, 2)},
                        {3, multibyte_utf8(0xF0, 0xE0, 3)},
                        {4, multibyte_utf8(0xF8, 0xF0, 4)},
                        {5, multibyte_utf8(0xFC, 0xF8, 5)},
                        {6, multibyte_utf8(0xFE, 0xFC, 6)} };

UTF8Checker::UTF8Checker()
    : CheckerBase("utf-8")
{

}

bool UTF8Checker::detect(const std::string &str) const
{
    int index = -1 ;
    int length = str.length();
    const unsigned char *buffer = (const unsigned char*)str.c_str();
    while(index + 1 < length)
    {
        bool flag = false;
        for(auto &func : UTF8_Detect)
        {
            if(func.second(buffer, length - index - 1))
            {
                buffer += func.first;
                index += func.first;
                flag = true;
                break;
            }
        }

        if(!flag)
        {
            break;
        }
    }
    return (index + 1 == length);
}
///////////////////////////////////////////////////////////////////////////////////////////

class Big5Checker : public CheckerBase
{
public:
    Big5Checker();
    virtual bool detect(const std::string &str) const override;

private:
    bool checkTwoBytes(const unsigned char *buffer) const;

};

#define Big5_two_byte_func(min, max) \
    [](const unsigned char *s) \
    { \
        unsigned short value = (*s << 8) + s[1]; \
        return value >= min && value <= max; \
    }

const std::vector< std::function< bool(const unsigned char*) > > Big5_Detect = {
        Big5_two_byte_func(0xa140, 0xa3bf),
        Big5_two_byte_func(0xa440, 0xc67e),
        Big5_two_byte_func(0xc940, 0xf9d5) };

Big5Checker::Big5Checker()
    : CheckerBase("big5")
{

}

bool Big5Checker::detect(const std::string &str) const
{
    int index = -1;
    int length = str.length();
    const unsigned char *buffer = (const unsigned char*)str.c_str();
    while(index + 1 < length)
    {
        if(*buffer <= 0x7F)
        {
            index += 1;
            buffer += 1;
        }

        if(checkTwoBytes(buffer))
        {
            index += 2;
            buffer += 2;
        }
        else
        {
            break;
        }
    }
    return (index + 1 == length);
}

bool Big5Checker::checkTwoBytes(const unsigned char *buffer) const
{
    for(auto func : Big5_Detect)
    {
        if(func(buffer))
        {
            return true;
        }
    }
    return false;
}
///////////////////////////////////////////////////////////////////////////////////////////

class ShiftJISChecker : public CheckerBase
{
public:
    ShiftJISChecker();
    virtual bool detect(const std::string &str) const override;

};
#define ShiftJIS_one_byte_func(a, b) \
    [](const unsigned char *s) \
    { \
        return *s >=a && *s <= b; \
    }

#define ShiftJIS_two_byte_func(a, b, c, d) \
    [](const unsigned char *s) \
    { \
        return *s >=a && * s<= b && s[1] >= c && s[1] <= d; \
    }

const std::vector< std::pair<int, std::function<bool(const unsigned char*) > > > ShiftJIS_Detect = {
        {2, ShiftJIS_two_byte_func(0x40, 0x7E, 0x80, 0xFC)},
        {2, ShiftJIS_two_byte_func(0x81, 0x9F, 0xE0, 0xEF)},
        {1, ShiftJIS_one_byte_func(0x00, 0x7F)},
        {1, ShiftJIS_one_byte_func(0xA1, 0xDF)} };

ShiftJISChecker::ShiftJISChecker()
    : CheckerBase("shift_jis")
{

}

bool ShiftJISChecker::detect(const std::string &str) const
{
    int index = -1;
    int length = str.length();
    const unsigned char* buffer = (const unsigned char*)str.c_str();
    while(index + 1 < length)
    {
        bool flag = false;
        int leftLength = length - index - 1;
        for(auto func : ShiftJIS_Detect)
        {
            if(func.first >= leftLength && func.second(buffer))
            {
                index += func.first;
                buffer += func.first;
                flag = true;
                break;
            }
        }

        if(!flag)
        {
            break;
        }
    }
    return (index + 1 == length);
}
///////////////////////////////////////////////////////////////////////////////////////////

CharChecker::CharChecker()
{
    m_checkers.insert(ASCII, new AsciiChecker);
    m_checkers.insert(BIG5, new Big5Checker);
    m_checkers.insert(GB18030, new GB18030Checker);
    m_checkers.insert(GBK, new GBKChecker);
    m_checkers.insert(SHIFT_JIS, new ShiftJISChecker);
    m_checkers.insert(UTF8, new UTF8Checker);
}

CharChecker::~CharChecker()
{
    for(auto itr = m_checkers.begin(); itr != m_checkers.end(); ++itr)
    {
        delete itr.value();
    }
}

CharChecker::Type CharChecker::check(const QString &str)
{
    Type type = Unknown;
    for(auto itr = m_checkers.begin(); itr != m_checkers.end(); ++itr)
    {
        if(itr.value()->detect(str.toStdString()))
        {
            type = itr.key();
            break;
        }
    }

    return type;
}
