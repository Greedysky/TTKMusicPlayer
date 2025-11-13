#include "ttkinitialization.h"
#include "ttkdesktopscreen.h"
#include "ttklogoutput.h"
#include "ttkversion.h"
#include "ttkdumper.h"

#include <QTextCodec>
#include <QApplication>
#include <QFontDatabase>

#ifdef Q_OS_LINUX
#  include <malloc.h>
#endif

#if !TTK_QT_VERSION_CHECK(5,1,0)
bool qEnvironmentVariableIsSet(const char *v) noexcept
{
#ifdef Q_CC_MSVC
    size_t s = 0;
    (void)getenv_s(&s, 0, 0, v);
    return s != 0;
#else
    return ::getenv(v) != nullptr;
#endif
}
#endif

void TTK::initialize(TTK::Attribute attr)
{
    if(attr & TTK::Attribute::UseLog)
    {
        // initiailize log module
        TTK::initiailizeLog(TTK_APP_NAME);
        TTK::installLogHandler();
    }

    TTK_INFO_STREAM("TTK Application start, flag value is" << attr);

    if(attr & TTK::Attribute::ScaleFactor)
    {
#if TTK_QT_VERSION_CHECK(6,0,0)
    // do nothing
#elif TTK_QT_VERSION_CHECK(5,4,0)
#  if TTK_QT_VERSION_CHECK(5,12,0)
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
      QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#    if TTK_QT_VERSION_CHECK(5,14,0)
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#    endif
#  elif TTK_QT_VERSION_CHECK(5,6,0)
      const float dpi = TTKDesktopScreen::dotsPerInch() / 96.0;
      qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi < 1.0 ? 1.0 : dpi));
#  endif
#endif
    }

#ifdef Q_OS_UNIX
    if(attr & TTK::Attribute::UseXCB)
    {
        // froce using xcb on XWayland
        if(qgetenv("XDG_SESSION_TYPE") == "wayland" && !qEnvironmentVariableIsSet("QT_QPA_PLATFORM"))
        {
            qputenv("QT_QPA_PLATFORM", "xcb");
        }
    }
#endif
}


TTKInitialization::TTKInitialization()
    : TTKInitialization(nullptr)
{

}

TTKInitialization::TTKInitialization(const TTKCleanFunctor &functor)
{
    TTKRegisterOrganization(TTK_APP);

    m_dumper = new TTKDumper(functor);
    m_dumper->execute();
}

TTKInitialization::~TTKInitialization()
{
    delete m_dumper;
}

void TTKInitialization::generateFont()
{
    QFont font = QApplication::font();
    for(const QString &family : QFontDatabase().families())
    {
        if(family == "微软雅黑" ||
           family == QString::fromUtf8("微软雅黑") ||
           family == QString::fromLocal8Bit("微软雅黑") ||
           family == "Microsoft YaHei")
        {
            font.setFamily(family);
            QApplication::setFont(font);
            TTK_INFO_STREAM("TTK application uses 'Microsoft YaHei' font");
            break;
        }
    }
}

void TTKInitialization::codecForLocale()
{
#if !TTK_QT_VERSION_CHECK(5,0,0)
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
}

void TTKInitialization::execute()
{
    execute(QCoreApplication::arguments());
}

void TTKInitialization::execute(const QStringList &arguments)
{
#ifdef Q_OS_LINUX
    // memory free
    mallopt(M_MMAP_THRESHOLD, 1024 * 1024);   // 1MB mmap
    mallopt(M_TRIM_THRESHOLD, 2 * 1024 * 1024); // 2MB brk
#endif

    for(const QString &arg : arguments)
    {
        if("-v" == arg || "--version" == arg || "-version" == arg)
        {
            QString text =  TTK_STR_CAT("\nModule: ", TTK_APP_NAME, " v", TTK_VERSION_STR, "\n");
                    text += TTK_STR_CAT("Build: ", __DATE__, " ", __TIME__, "\n");
                    text += TTK_STR_CAT("Offical web page: https://github.com/Greedysky/", TTK_APP_NAME, "\n");
                    text += "Copyright(C) 2015 - 2025 Greedysky All Rights Reserved\n";
            TTK_INFO_STREAM(text);
            exit(EXIT_SUCCESS);
        }
    }
}
