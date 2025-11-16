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
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return str.indexOf(m_regular) != -1;
#else
    m_match = m_regular.match(str);
    return m_match.hasMatch();
#endif
}

int RegularExpression::match(const QString &str, int pos)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return m_regular.indexIn(str, pos);
#else
    m_match = m_regular.match(str, pos);
    return m_match.capturedEnd();
#endif
}

bool RegularExpression::greedinessOption() const
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return m_regular.isMinimal();
#else
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    return flags & QRegularExpression::InvertedGreedinessOption;
#endif
}

void RegularExpression::setGreedinessOption(bool v)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    m_regular.setMinimal(v);
#else
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    m_regular.setPatternOptions(v ? (flags | QRegularExpression::InvertedGreedinessOption) : (flags & ~QRegularExpression::InvertedGreedinessOption));
#endif
}

QString RegularExpression::captured(int index) const
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return m_regular.cap(index);
#else
    return m_match.captured(index);
#endif
}

int RegularExpression::capturedLength() const
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return m_regular.matchedLength();
#else
    return m_match.capturedLength();
#endif
}

QString RegularExpression::escape(const QString &str)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return QRegExp::escape(str);
#else
    return QRegularExpression::escape(str);
#endif
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
const QRegExp &RegularExpression::value() const
#else
const QRegularExpression &RegularExpression::value() const
#endif
{
    return m_regular;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
RegularExpression::operator QRegExp &()
#else
RegularExpression::operator QRegularExpression &()
#endif
{
    return m_regular;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
RegularExpression::operator const QRegExp &() const
#else
RegularExpression::operator const QRegularExpression &() const
#endif
{
    return m_regular;
}
