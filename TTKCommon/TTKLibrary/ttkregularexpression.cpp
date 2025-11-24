#include "ttkregularexpression.h"

TTKRegularExpression::TTKRegularExpression()
    : m_regular()
#if TTK_QT_VERSION_CHECK(5,0,0) && !TTK_QT_VERSION_CHECK(5,1,0)
    , m_match(m_regular.match({}))
#endif
{

}

TTKRegularExpression::TTKRegularExpression(const QString &pattern, int option)
    : m_regular(pattern)
#if TTK_QT_VERSION_CHECK(5,0,0) && !TTK_QT_VERSION_CHECK(5,1,0)
    , m_match(m_regular.match({}))
#endif
{
    setPatternOptions(option);
}

QString TTKRegularExpression::pattern() const
{
    return m_regular.pattern();
}

void TTKRegularExpression::setPattern(const QString &v)
{
    m_regular.setPattern(v);
}

int TTKRegularExpression::patternOptions() const
{
    int option = PatternOption::NoPatternOption;
    if(!isGreediness())
    {
        option |= PatternOption::InvertedGreedinessOption;
    }

    if(!isCaseSensitivity())
    {
        option |= PatternOption::CaseInsensitiveOption;
    }
    return option;
}

void TTKRegularExpression::setPatternOptions(const int option)
{
    if(option & PatternOption::InvertedGreedinessOption)
    {
        setGreediness(false);
    }

    if(option & PatternOption::CaseInsensitiveOption)
    {
        setCaseSensitivity(false);
    }
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
    return m_match.capturedStart();
#else
    return m_regular.indexIn(str, pos);
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

bool TTKRegularExpression::isGreediness() const
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    return !(flags & QRegularExpression::InvertedGreedinessOption);
#else
    return !m_regular.isMinimal();
#endif
}

void TTKRegularExpression::setGreediness(bool v)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    m_regular.setPatternOptions(v ? (flags & ~QRegularExpression::InvertedGreedinessOption) : (flags | QRegularExpression::InvertedGreedinessOption));
#else
    m_regular.setMinimal(!v);
#endif
}

bool TTKRegularExpression::isCaseSensitivity() const
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    return !(flags & QRegularExpression::CaseInsensitiveOption);
#else
    return m_regular.caseSensitivity() == Qt::CaseSensitive;
#endif
}

void TTKRegularExpression::setCaseSensitivity(bool v)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    m_regular.setPatternOptions(v ? (flags & ~QRegularExpression::CaseInsensitiveOption) : (flags | QRegularExpression::CaseInsensitiveOption));
#else
    m_regular.setCaseSensitivity(v ? Qt::CaseSensitive : Qt::CaseInsensitive);
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
const QRegularExpression &TTKRegularExpression::value() const
#else
const QRegExp &TTKRegularExpression::value() const
#endif
{
    return m_regular;
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
