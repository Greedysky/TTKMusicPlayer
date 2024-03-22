#include "ttkcommandline.h"

#include <QStringList>

void TTKCommandLineParser::setDescription(const QString &description) noexcept
{
    m_description = description;
}

bool TTKCommandLineParser::addOption(const TTKCommandLineOption &option)
{
    if(option.first().isEmpty())
    {
        TTK_ERROR_STREAM("Command line option first key can't be empty");
        return false;
    }

    for(const TTKCommandLineOption &opt : qAsConst(m_options))
    {
        if(option == opt)
        {
            TTK_ERROR_STREAM("Command line option already having an option named");
            return false;
        }
    }

    m_options << option;
    return true;
}

bool TTKCommandLineParser::addOptions(const QList<TTKCommandLineOption> &options)
{
    bool result = true;
    for(const TTKCommandLineOption &opt : qAsConst(options))
    {
        result &= addOption(opt);
    }
    return result;
}

void TTKCommandLineParser::process()
{
    process(QCoreApplication::arguments().mid(1));
}

void TTKCommandLineParser::process(const QStringList &arguments)
{
    QString lastCommand;
    for(const QString &arg : qAsConst(arguments))
    {
        const QString &cmd = arg.trimmed();
        if(cmd.startsWith("-") || cmd.startsWith("--"))
        {
            m_commands.insert(cmd, {});
            lastCommand = cmd;
        }
        else if(!m_commands.isEmpty() && !lastCommand.isEmpty())
        {
            m_commands[lastCommand] = arg;
        }
    }
}

bool TTKCommandLineParser::isSet(const TTKCommandLineOption &option) const
{
    const QList<QString> &keys = m_commands.keys();
    return keys.contains(option.first()) || keys.contains(option.second());
}

QString TTKCommandLineParser::value(const TTKCommandLineOption &option) const
{
    const QString &first = m_commands.value(option.first());
    const QString &second = m_commands.value(option.second());
    return second.isEmpty() ? first : second;
}

void TTKCommandLineParser::showHelp() const
{
    QString text = "TTK Command Line Module Options:\n";
    if(!m_description.isEmpty())
    {
        text = m_description + text;
    }

    for(const TTKCommandLineOption &option : qAsConst(m_options))
    {
        const QString &first = option.first();
        const QString &second = option.second();
        const QString &line = !second.isEmpty() ? (first + ", " + second) : first;
        text += line.leftJustified(20) + option.description() + "\n";
    }
    TTK_LOG_STREAM(text);
}
