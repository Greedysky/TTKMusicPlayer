#include "projectm4widget.h"
#include "projectmhelper.h"
#include <projectM-4/playlist.h>
#include <projectM-4/projectM.h>

ProjectM4Widget::ProjectM4Widget(QListWidget *widget, QWidget *parent)
#ifdef QT_OPENGL_WIDGET
    : QOpenGLWidget(parent)
#else
    : QGLWidget(parent)
#endif
    , m_itemWidget(widget)
{
    setMinimumSize(580, 320);
}

ProjectM4Widget::~ProjectM4Widget()
{
    if(m_playlistHandle)
    {
        projectm_playlist_destroy(m_playlistHandle);
        m_playlistHandle = nullptr;
    }

    if(m_handle)
    {
        projectm_destroy(m_handle);
        m_handle = nullptr;
    }
}

void ProjectM4Widget::addPCM(float *left, float *right)
{
    short buf[2][QMMP_VISUAL_NODE_SIZE];
    for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
    {
        buf[i * 2] = left[i];
        buf[i * 2 + 1] = right[i];
    }

    projectm_pcm_add_float(m_handle, buf, QMMP_VISUAL_NODE_SIZE, PROJECTM_STEREO);
}

void ProjectM4Widget::initializeGL()
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

    if(!m_handle)
    {
        m_handle = projectm_create();
        if(!m_handle)
        {
            qDebug("ProjectM4Widget: unable to initialize GL widget");
            return;
        }

        projectm_set_texture_search_paths(m_handle, qPrintable(PROJECTM_TEXTURE_PATH), 1);
        projectm_set_fps(m_handle, 60);
        projectm_set_mesh_size(m_handle, 220, 125);
        projectm_set_aspect_correction(m_handle, true);
        projectm_set_preset_duration(m_handle, 10.0);
        projectm_set_soft_cut_duration(m_handle, 3.0);
        projectm_set_hard_cut_enabled(m_handle, true);
        projectm_set_hard_cut_sensitivity(m_handle, 1.0);
        projectm_set_beat_sensitivity(m_handle, 1.0);
        projectm_set_easter_egg(m_handle, 1.0);

        m_playlistHandle = projectm_playlist_create(m_handle);
        projectm_playlist_set_shuffle(m_playlistHandle, false);
        projectm_playlist_set_preset_switched_event_callback(m_playlistHandle, &ProjectM4Widget::presetSwitchedEvent, this);

        const QFileInfoList folderList(fileListByPath(PROJECTM_PRESET_PATH, QStringList() << "*.prjm" << "*.milk"));
        for(const QFileInfo &fin : folderList)
        {
            projectm_playlist_add_preset(m_playlistHandle, qPrintable(fin.absoluteFilePath()), false);
            m_itemWidget->addItem(fin.fileName());
            m_itemWidget->setCurrentRow(0, QItemSelectionModel::Select);
        }

        connect(m_itemWidget, SIGNAL(currentRowChanged(int)), this, SLOT(selectPreset(int)));
    }
}

void ProjectM4Widget::resizeGL(int w, int h)
{
    if(m_handle)
    {
        initializeGL();
        projectm_set_window_size(m_handle, w, h);
    }
}

void ProjectM4Widget::paintGL()
{
    if(m_handle)
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        projectm_opengl_render_frame(m_handle);
    }
}

void ProjectM4Widget::nextPreset()
{
    if(m_playlistHandle)
    {
        projectm_playlist_play_next(m_playlistHandle, true);
    }
}

void ProjectM4Widget::previousPreset()
{
    if(m_playlistHandle)
    {
        projectm_playlist_play_previous(m_playlistHandle, true);
    }
}

void ProjectM4Widget::setShuffle(bool enabled)
{
    if(m_playlistHandle)
    {
        projectm_playlist_set_shuffle(m_playlistHandle, enabled);
    }
}

void ProjectM4Widget::lockPreset(bool lock)
{
    if(m_handle)
    {
        projectm_set_preset_locked(m_handle, lock);
    }
}

void ProjectM4Widget::selectPreset(int index)
{
    if(m_playlistHandle)
    {
        projectm_playlist_set_position(m_playlistHandle, index, true);
    }
}

void ProjectM4Widget::presetSwitchedEvent(bool isHardCut, unsigned int index, void *data)
{
    Q_UNUSED(isHardCut);
    static_cast<ProjectM4Widget *>(data)->m_itemWidget->setCurrentRow(index);
}
