#include "ttksuperenum.h"

#include <QStringList>

TTKSuperEnum::TTKSuperEnum(const QString &value)
    : m_enumValue()
{
    int enumValue = 0;
    const QStringList &enums = value.split(',');

    for(QString data : qAsConst(enums))
    {
        const int pos = data.indexOf('=');
        if(pos != TTKSuperEnum::Null)
        {
            enumValue = data.right(data.length() - pos - 1).trimmed().toInt();
            data = data.left(pos);
        }

        m_enumValue[enumValue] = data.trimmed();
        enumValue++;
    }
}

QString TTKSuperEnum::keyToString(int value) const
{
    if(TTKSuperEnum::Null == value)
    {
        return "Null";
    }

    const auto& it = m_enumValue.find(value);
    return it != m_enumValue.end() ? it->second : "Invalid";
}

int TTKSuperEnum::stringToKey(const QString &name) const
{
    if("Null" == name)
    {
        return TTKSuperEnum::Null;
    }

    for(auto itr = m_enumValue.begin(); itr != m_enumValue.end(); ++itr)
    {
        if(itr->second == name)
        {
            return itr->first;
        }
    }
    return TTKSuperEnum::Null;
}
