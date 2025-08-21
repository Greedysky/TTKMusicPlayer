#include "ttkregularexpression.h"

TTKRegularExpression::TTKRegularExpression(const QString &pattern)
    : m_regular(pattern)
#if TTK_QT_VERSION_CHECK(5,0,0) && !TTK_QT_VERSION_CHECK(5,1,0)
    , m_match(m_regular.match({}))
#endif
{

}

QString TTKRegularExpression::pattern() const
{
    return m_regular.pattern();
}

void TTKRegularExpression::setPattern(const QString &v)
{
    m_regular.setPattern(v);
}

bool TTKRegularExpression::hasMatch(const QString &str)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    m_match = m_regular.match(str);
    return m_match.hasMatch();
#else
    return str.indexOf(m_regular) != -1;
#endif
}

int TTKRegularExpression::match(const QString &str, int pos)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    m_match = m_regular.match(str, pos);
    return m_match.capturedEnd();
#else
    return m_regular.indexIn(str, pos);
#endif
}

bool TTKRegularExpression::greedinessOption() const
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    return flags & QRegularExpression::InvertedGreedinessOption;
#else
    return m_regular.isMinimal();
#endif
}

void TTKRegularExpression::setGreedinessOption(bool v)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    m_regular.setPatternOptions(v ? (flags | QRegularExpression::InvertedGreedinessOption) : (flags & ~QRegularExpression::InvertedGreedinessOption));
#else
    m_regular.setMinimal(v);
#endif
}

QString TTKRegularExpression::captured(int index) const
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    return m_match.captured(index);
#else
    return m_regular.cap(index);
#endif
}

int TTKRegularExpression::capturedLength() const
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    return m_match.capturedLength();
#else
    return m_regular.matchedLength();
#endif
}

QString TTKRegularExpression::escape(const QString &str)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    return QRegularExpression::escape(str);
#else
    return QRegExp::escape(str);
#endif
}

#if TTK_QT_VERSION_CHECK(5,0,0)
TTKRegularExpression::operator QRegularExpression &()
#else
TTKRegularExpression::operator QRegExp &()
#endif
{
    return m_regular;
}

#if TTK_QT_VERSION_CHECK(5,0,0)
TTKRegularExpression::operator const QRegularExpression &() const
#else
TTKRegularExpression::operator const QRegExp &() const
#endif
{
    return m_regular;
}
