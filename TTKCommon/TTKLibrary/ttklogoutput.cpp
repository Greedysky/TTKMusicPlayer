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
    void removeHistory(size_t maxSecond) noexcept;

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
    QString m_module, m_dateTime;
    int m_maxSize, m_maxSecond;
    QMutex m_mutex;
    QtMessageHandler m_defaultHandler;

};

#define LOG_DIR_PATH QApplication::applicationDirPath() + "/log/"

TTKLogOutput::TTKLogOutput()
    : m_file(),
      m_dateTime(),
      m_maxSize(),
      m_maxSecond(),
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
        if(current - old > time)
        {
            QFile::remove(fin.absoluteFilePath());
        }
    }
}

void TTKLogOutput::install()
{
    const QString &path = LOG_DIR_PATH;
    const QDir dir(path);
    if(!dir.exists())
    {
        dir.mkdir(path);
    }

    // remove old history
    removeFiles(path, m_maxSecond);

    // open new log handler
    open();

    if(!m_defaultHandler)
    {
        m_defaultHandler = qInstallMessageHandler(TTKLogOutput::loggerHandler);
    }
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

void TTKLogOutput::removeHistory(size_t maxSecond) noexcept
{
    m_maxSecond = maxSecond;
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
    const QString &fileName = LOG_DIR_PATH + m_module + m_dateTime;

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

#if TTK_QT_VERSION_CHECK(5,0,0)
    m_defaultHandler(type, context, message);
#else
    m_defaultHandler(type, message.toUtf8().constData());
#endif

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

void TTK::setLogMaxSize(size_t maxSize)
{
    TTKSingleton<TTKLogOutput>::instance()->setMaxSize(maxSize);
}

void TTK::removeLogHistory(size_t maxSecond)
{
    TTKSingleton<TTKLogOutput>::instance()->removeHistory(maxSecond);
}
