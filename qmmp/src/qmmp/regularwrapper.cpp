#include "regularwrapper.h"

RegularWrapper::RegularWrapper()
{

}

RegularWrapper::RegularWrapper(const QString &pattern)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
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
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    m_match = m_regular.match(str);
    return m_match.hasMatch();
#else
    return str.indexOf(m_regular) != -1;
#endif
}

QString RegularWrapper::value(int index) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return m_match.captured(index);
#else
    return m_regular.cap(index);
#endif
}

QString RegularWrapper::escape(const QString &str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return QRegularExpression::escape(str);
#else
    return QRegExp::escape(str);
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
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
