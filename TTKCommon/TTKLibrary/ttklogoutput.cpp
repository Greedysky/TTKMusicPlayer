#include "ttklogoutput.h"
#include "ttksingleton.h"

#include <QDir>
#include <QApplication>

#if !TTK_QT_VERSION_CHECK(5,0,0)
class QMessageLogContext {};
using QtMessageHandler = QtMsgHandler;
#define qInstallMessageHandler qInstallMsgHandler
#endif

class TTKLogOutput
{
    TTK_DECLARE_SINGLETON_CLASS(TTKLogOutput)
public:
    /*!
     * Initiailize log module.
     */
    void initialize(const QString &module);
    /*!
     * Install log output handler.
     */
    void install();
    /*!
     * Uninstall log output handler.
     */
    void uninstall();

    /*!
     * Set max size for log file.
     */
    void setMaxSize(size_t maxSize) noexcept;
    /*!
     * Remove log file after max second.
     */
    void setExpireSecond(size_t maxSecond) noexcept;
    /*!
     * Set log output dir path.
     */
    void setOutputPath(const QString &dir) noexcept;

    /*!
     * Log output handler.
     */
#if TTK_QT_VERSION_CHECK(5,0,0)
    static void loggerHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);
#else
    static void loggerHandler(QtMsgType type, const char *message);
#endif

private:
    /*!
     * Object constructor.
     */
    TTKLogOutput();
    /*!
     * Object destructor.
     */
    ~TTKLogOutput();

    /*!
     * Open log output file.
     */
    void open();
    /*!
     * Save log output file.
     */
    void save(const QString &message);
    /*!
     * Write log output file.
     */
    void write(QtMsgType type, const QMessageLogContext &context, const QString &message);

private:
    QFile m_file;
    qint64 m_maxSize, m_maxSecond;
    QString m_module, m_dateTime, m_outputDir;
    QMutex m_mutex;
    QtMessageHandler m_defaultHandler;

};


TTKLogOutput::TTKLogOutput()
    : m_file(),
      m_maxSize(5 * 1024 * 1024),
      m_maxSecond(7 * 24 * 3600),
      m_module(TTK_STR_CAT(TTK_NAN_STR, TTK_DEFAULT_STR)),
      m_dateTime(TTK_DATE_FORMAT),
#ifdef Q_OS_WIN
      m_outputDir(QString::fromLocal8Bit(getenv("APPDATA")) + TTK_SEPARATOR + "ttk/"),
#else
      m_outputDir(QDir::homePath() + TTK_SEPARATOR + ".config/ttk/"),
#endif
      m_mutex(),
      m_defaultHandler(nullptr)
{

}

TTKLogOutput::~TTKLogOutput()
{
    if(m_file.isOpen())
    {
        m_file.close();
    }
}

void TTKLogOutput::initialize(const QString &module)
{
    if(!module.isEmpty())
    {
        m_module = module + "_";
    }
    else
    {
        m_module.clear();
    }
}

static void removeFiles(const QString &path, const qint64 time)
{
    QDir dir(path);
    for(const QFileInfo &fin : dir.entryInfoList())
    {
        const QString &fileName = fin.fileName();
        if(fileName == "." || fileName == "..")
        {
            continue;
        }

#if TTK_QT_VERSION_CHECK(5,10,0)
        const qint64 old = fin.birthTime().toMSecsSinceEpoch();
#else
        const qint64 old = fin.created().toMSecsSinceEpoch();
#endif
        const qint64 current = QDateTime::currentMSecsSinceEpoch();
        if((current - old) / 1000 > time)
        {
            QFile::remove(fin.absoluteFilePath());
        }
    }
}

void TTKLogOutput::install()
{
    if(m_defaultHandler)
    {
        return;
    }

    const QDir dir(m_outputDir);
    if(!dir.exists())
    {
        dir.mkdir(m_outputDir);
    }

    // remove old history
    removeFiles(m_outputDir, m_maxSecond);

    // open new log handler
    open();

    m_defaultHandler = qInstallMessageHandler(TTKLogOutput::loggerHandler);
}

void TTKLogOutput::uninstall()
{
    if(m_defaultHandler)
    {
        m_defaultHandler = nullptr;
        qInstallMessageHandler(m_defaultHandler);

        m_file.close();
    }
}

void TTKLogOutput::setMaxSize(size_t maxSize) noexcept
{
    m_maxSize = maxSize;
}

void TTKLogOutput::setExpireSecond(size_t maxSecond) noexcept
{
    m_maxSecond = maxSecond;
}

void TTKLogOutput::setOutputPath(const QString &dir) noexcept
{
    m_outputDir = dir + TTK_SEPARATOR;
}

#if TTK_QT_VERSION_CHECK(5,0,0)
void TTKLogOutput::loggerHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    TTKSingleton<TTKLogOutput>::instance()->write(type, context, message);
}
#else
void TTKLogOutput::loggerHandler(QtMsgType type, const char *message)
{
    TTKSingleton<TTKLogOutput>::instance()->write(type, {}, message);
}
#endif

void TTKLogOutput::open()
{
    if(m_file.isOpen())
    {
        return;
    }

    m_dateTime = QDate::currentDate().toString(TTK_DATE_FORMAT);
    const QString &fileName = m_outputDir + m_module + m_dateTime;

    int index = 1;
    do
    {
        m_file.setFileName(fileName + QString("_%1.log").arg(index++));
    }
    while(m_file.size() >= m_maxSize);

    if(!m_file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        TTK_ERROR_STREAM("Open log file failed: " << m_file.fileName());
    }
}

void TTKLogOutput::save(const QString &message)
{
    if(m_file.isOpen())
    {
        QTextStream out(&m_file);
        out << message << TTK_WLINEFEED;
        m_file.flush();
    }
}

void TTKLogOutput::write(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    Q_UNUSED(type);
    Q_UNUSED(context);

    m_mutex.lock();

    // output to console
#if TTK_QT_VERSION_CHECK(5,5,0)
    if(type == QtDebugMsg || type == QtWarningMsg || type == QtInfoMsg)
#else
    if(type == QtDebugMsg || type == QtWarningMsg)
#endif
    {
        QTextStream(stdout) << message << QtNamespace(endl);
    }
    else
    {
        QTextStream(stderr) << message << QtNamespace(endl);
    }

    // output to file
    if(m_file.isOpen())
    {
        const QString &date = QDate::currentDate().toString(TTK_DATE_FORMAT);
        const bool moreLarge = m_file.size() >= m_maxSize;
        const bool nextDate = date.compare(m_dateTime, Qt::CaseInsensitive) != 0;

        if(moreLarge || nextDate)
        {
            m_file.close();
            open();
        }
    }

    save(message);
    m_mutex.unlock();
}


void TTK::initiailizeLog(const QString &module)
{
    TTKSingleton<TTKLogOutput>::instance()->initialize(module);
}

void TTK::installLogHandler()
{
    TTKSingleton<TTKLogOutput>::instance()->install();
}

void TTK::removeLogHandler()
{
    TTKSingleton<TTKLogOutput>::instance()->uninstall();
}

void TTK::setLogMaxSize(qint64 maxSize)
{
    TTKSingleton<TTKLogOutput>::instance()->setMaxSize(maxSize);
}

void TTK::setExpireSecond(qint64 maxSecond)
{
    TTKSingleton<TTKLogOutput>::instance()->setExpireSecond(maxSecond);
}

void TTK::setOutputPath(const QString &dir)
{
    TTKSingleton<TTKLogOutput>::instance()->setOutputPath(dir);
}
