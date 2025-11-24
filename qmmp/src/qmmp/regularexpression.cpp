#include "regularexpression.h"

RegularExpression::RegularExpression()
    : m_regular()
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) && QT_VERSION < QT_VERSION_CHECK(5,1,0)
    , m_match(m_regular.match({}))
#endif
{

}

RegularExpression::RegularExpression(const QString &pattern, int option)
    : m_regular(pattern)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) && QT_VERSION < QT_VERSION_CHECK(5,1,0)
    , m_match(m_regular.match({}))
#endif
{
    setPatternOptions(option);
}

QString RegularExpression::pattern() const
{
    return m_regular.pattern();
}

void RegularExpression::setPattern(const QString &v)
{
    m_regular.setPattern(v);
}

int RegularExpression::patternOptions() const
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

void RegularExpression::setPatternOptions(const int option)
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
    return m_match.capturedStart();
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

bool RegularExpression::isGreediness() const
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return !m_regular.isMinimal();
#else
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    return !(flags & QRegularExpression::InvertedGreedinessOption);
#endif
}

void RegularExpression::setGreediness(bool v)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    m_regular.setMinimal(!v);
#else
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    m_regular.setPatternOptions(v ? (flags & ~QRegularExpression::InvertedGreedinessOption) : (flags | QRegularExpression::InvertedGreedinessOption));
#endif
}

bool RegularExpression::isCaseSensitivity() const
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    return m_regular.caseSensitivity() == Qt::CaseSensitive;
#else
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    return !(flags & QRegularExpression::CaseInsensitiveOption);
#endif
}

void RegularExpression::setCaseSensitivity(bool v)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    m_regular.setCaseSensitivity(v ? Qt::CaseSensitive : Qt::CaseInsensitive);
#else
    const QRegularExpression::PatternOptions flags = m_regular.patternOptions();
    m_regular.setPatternOptions(v ? (flags & ~QRegularExpression::CaseInsensitiveOption) : (flags | QRegularExpression::CaseInsensitiveOption));
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
