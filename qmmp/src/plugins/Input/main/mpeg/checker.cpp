#include "checker.h"

#include <vector>
#include <functional>

class AbstractChecker
{
public:
    AbstractChecker() = default;
    virtual ~AbstractChecker() = default;

    virtual bool detect(const std::string &str) const = 0;
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

const std::vector<std::pair<int, std::function<bool(const unsigned char*)>>> GB18030_Detect = {
    {1, [](const unsigned char *s){ return *s <= 0x7F;} },
    {2, GB18030_two_byte_func(0xB0, 0xF7, 0xA1, 0xFE)},
    {2, GB18030_two_byte_func(0x81, 0xA0, 0x40, 0xFE)},
    {2, GB18030_two_byte_func(0xAA, 0xFE, 0x40, 0xA0)},
    {4, GB18030_four_byte_func(0x81, 0x82)},
    {4, GB18030_four_byte_func(0x95, 0x98)} };

class GB18030Checker : public AbstractChecker
{
public:
    virtual bool detect(const std::string &str) const override final
    {
        int index = -1;
        const int length = static_cast<int>(str.length());
        const unsigned char* buffer = (const unsigned char*)str.c_str();
        while(index + 1 < length)
        {
            bool flag = false;
            int leftLength = length - index - 1;
            for(const auto &func : qAsConst(GB18030_Detect))
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
        return index + 1 == length;
    }
};

class GBKChecker : public AbstractChecker
{
public:
    virtual bool detect(const std::string &str) const override final
    {
        int index = -1;
        const int length = static_cast<int>(str.length());
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
        return index + 1 == length;
    }

private:
    bool checkTwoBytes(const unsigned char *buffer) const
    {
        bool fValid = (*buffer >= 0x81 && *buffer <= 0xFE);
        ++buffer;
        bool sValid = (*buffer >= 40 && *buffer <= 0xFE && *buffer != 0x7F);
        return fValid && sValid;
    }
};

CheckerHelper::CheckerHelper()
{
    m_checkers.insert("GBK", new GBKChecker);
    m_checkers.insert("GB18030", new GB18030Checker);
}

QString CheckerHelper::detect(const QString &str)
{
    QString v = "UTF-8";
    for(auto it = m_checkers.constBegin(); it != m_checkers.constEnd(); ++it)
    {
        if(it.value()->detect(str.toStdString()))
        {
            v = it.key();
            break;
        }
    }
    return v;
}
