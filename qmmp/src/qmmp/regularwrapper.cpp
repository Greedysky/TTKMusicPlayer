#include "regularwrapper.h"

RegularWrapper::RegularWrapper()
{

}

RegularWrapper::RegularWrapper(const QString &pattern)
{
#ifdef QMMP_GREATER_NEW
    m_regular = QRegularExpression(pattern);
#else
    m_regular = QRegExp(pattern);
#endif
}

RegularWrapper::~RegularWrapper()
{

}

bool RegularWrapper::hasMatch(const QString &str)
{
#ifdef QMMP_GREATER_NEW
    m_match = m_regular.match(str);
    return m_match.hasMatch();
#else
    return str.indexOf(m_regular) != -1;
#endif
}

QString RegularWrapper::value(int index) const
{
#ifdef QMMP_GREATER_NEW
    return m_match.captured(index);
#else
    return m_regular.cap(index);
#endif
}

QString RegularWrapper::escape(const QString &str)
{
#ifdef QMMP_GREATER_NEW
    return QRegularExpression::escape(str);
#else
    return QRegExp::escape(str);
#endif
}

#ifdef QMMP_GREATER_NEW
RegularWrapper::operator QRegularExpression () const
{
    return  m_regular;
}
#else
RegularWrapper::operator QRegExp () const
{
    return  m_regular;
}
#endif
