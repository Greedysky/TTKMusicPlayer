#include "projectmwidget.h"

#include <QDir>
#include <qmmp/qmmp.h>

static QFileInfoList fileListByPath(const QString &dpath, const QStringList &filter)
{
    QDir dir(dpath);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden);
    const QFileInfoList& folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(const QFileInfo &fin : folderList)
    {
        fileList.append(fileListByPath(fin.absoluteFilePath(), filter));
    }

    return fileList;
}


ProjectMWrapper::ProjectMWrapper(const projectM::Settings &settings, int flags, QObject *parent)
    : QObject(parent), projectM(settings, flags)
{
}

void ProjectMWrapper::setCurrentPreset(int index)
{
    if(index >= 0)
    {
        projectM::selectPreset(index);
    }
}

void ProjectMWrapper::presetSwitchedEvent(bool isHardCut, unsigned int index) const
{
    Q_UNUSED(isHardCut);
    emit currentPresetChanged(index);
}


ProjectMWidget::ProjectMWidget(QListWidget *widget, QWidget *parent)
#ifdef QT_OPENGL_WIDGET
    : QOpenGLWidget(parent)
#else
    : QGLWidget(parent)
#endif
    , m_itemWidget(widget)
{
    setMinimumSize(580, 320);
}

ProjectMWidget::~ProjectMWidget()
{
    delete m_projectM;
}

projectM *ProjectMWidget::projectMInstance()
{
    return m_projectM;
}

void ProjectMWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0);
    // Setup our viewport
    glViewport(0, 0, width(), height());
    // Change to the projection matrix and set our viewing volume.
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glLineStipple(2, 0xAAAA);

    if(!m_projectM)
    {
        projectM::Settings settings;
        settings.meshX = 32;
        settings.meshY = 24;
        settings.fps = 35;
        settings.textureSize = 1024;
        settings.windowWidth = 512;
        settings.windowHeight = 512;
        settings.presetURL = qPrintable(Qmmp::ttkPluginPath() + "/config/presets");
        settings.titleFontURL = qPrintable(Qmmp::ttkPluginPath() + "/config/fonts/Vera.ttf");
        settings.menuFontURL = qPrintable(Qmmp::ttkPluginPath() + "/config/fonts/VeraMono.ttf");
        settings.smoothPresetDuration = 5;
        settings.presetDuration = 30;
        settings.beatSensitivity = 1.0;
        settings.aspectCorrection = true;
        settings.easterEgg = 1.0;
        settings.shuffleEnabled = false;
        settings.softCutRatingsEnabled = false;
        m_projectM = new ProjectMWrapper(settings, projectM::FLAG_DISABLE_PLAYLIST_LOAD);

        const RatingList list = {3, 3};
        const QString &path = QString::fromLocal8Bit(settings.presetURL.c_str());
        const QFileInfoList folderList(fileListByPath(path, QStringList() << "*.prjm" << "*.milk"));
        for(const QFileInfo &fin : folderList)
        {
            m_projectM->addPresetURL(fin.absoluteFilePath().toStdString(), fin.fileName().toStdString(), list);
            m_itemWidget->addItem(fin.fileName());
            m_itemWidget->setCurrentRow(0, QItemSelectionModel::Select);
        }

        connect(m_itemWidget, SIGNAL(currentRowChanged(int)), m_projectM, SLOT(setCurrentPreset(int)));
        connect(m_projectM, SIGNAL(currentPresetChanged(int)), SLOT(setCurrentRow(int)));
        randomPreset();
    }
}

void ProjectMWidget::resizeGL(int w, int h)
{
    if(m_projectM)
    {
        m_projectM->projectM_resetGL(w, h);
        initializeGL();
    }
}

void ProjectMWidget::paintGL()
{
    if(m_projectM)
    {
        m_projectM->renderFrame();
    }
}

void ProjectMWidget::nextPreset()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_n, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::previousPreset()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_p, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::randomPreset()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_r, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::lockPreset(bool lock)
{
    m_projectM->setPresetLock(lock);
}

void ProjectMWidget::setCurrentRow(int row)
{
    m_itemWidget->setCurrentRow(row);
}
