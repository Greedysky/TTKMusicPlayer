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

bool RegularWrapper::hasMatch(const QString &str) const
{
#ifdef QMMP_GREATER_NEW
    return m_regular.match(str).hasMatch();
#else
    return str.indexOf(m_regular) != -1;
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
