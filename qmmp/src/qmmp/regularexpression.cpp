#include "regularexpression.h"

RegularExpression::RegularExpression(const QString &pattern)
    : m_regular(pattern)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) && QT_VERSION < QT_VERSION_CHECK(5,1,0)
    , m_match(m_regular.match({}))
#endif
{

}

QString RegularExpression::pattern() const
{
    return m_regular.pattern();
}

void RegularExpression::setPattern(const QString &v)
{
    m_regular.setPattern(v);
}

bool RegularExpression::hasMatch(const QString &str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    m_match = m_regular.match(str);
    return m_match.hasMatch();
#else
    return str.indexOf(m_regular) != -1;
#endif
}

int RegularExpression::match(const QString &str, int pos)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    m_match = m_regular.match(str, pos);
    return m_match.capturedEnd();
#else
    return m_regular.indexIn(str, pos);
#endif
}

bool RegularExpression::greedinessOption() const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    return flags & QRegularExpression::InvertedGreedinessOption;
#else
    return m_regular.isMinimal();
#endif
}

void RegularExpression::setGreedinessOption(bool v)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    m_regular.setPatternOptions(v ? (flags | QRegularExpression::InvertedGreedinessOption) : (flags & ~QRegularExpression::InvertedGreedinessOption));
#else
    m_regular.setMinimal(v);
#endif
}

QString RegularExpression::captured(int index) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return m_match.captured(index);
#else
    return m_regular.cap(index);
#endif
}

int RegularExpression::capturedLength() const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return m_match.capturedLength();
#else
    return m_regular.matchedLength();
#endif
}

QString RegularExpression::escape(const QString &str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return QRegularExpression::escape(str);
#else
    return QRegExp::escape(str);
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
RegularExpression::operator QRegularExpression &()
#else
RegularExpression::operator QRegExp &()
#endif
{
    return m_regular;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
RegularExpression::operator const QRegularExpression &() const
#else
RegularExpression::operator const QRegExp &() const
#endif
{
    return m_regular;
}
