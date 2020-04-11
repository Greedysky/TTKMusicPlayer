#include "musiclrccontainerforcortana.h"
#include "musiclrcmanagerfordesktop.h"
#include "musicuiobject.h"
#include "musicfunctionuiobject.h"
#include "musicdesktoplrcuiobject.h"
#include "musicsettingmanager.h"
#include "musiclrcdefines.h"

#include "musicapplication.h"
#include "musicbottomareawidget.h"
#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

MusicLrcContainerForCortana::MusicLrcContainerForCortana(QWidget *parent)
    : MusicLrcContainer(parent)
{
    m_containerType = LRC_CORTANA_TPYE;
    m_geometry.setX(340);
    m_geometry.setY(20);

    m_musicLrcContainer << new MusicLrcManagerHorizontalDesktop(this)
                        << new MusicLrcManagerHorizontalDesktop(this);
    foreach(MusicLrcManager *manager, m_musicLrcContainer)
    {
        manager->setSelfGeometry(m_geometry.x(), m_geometry.y());
    }
#ifdef Q_OS_WIN
    PDWORD_PTR result = nullptr;
    HWND hTaskBar = FindWindowW(L"Shell_TrayWnd", nullptr);
    if(hTaskBar)
    {
        HWND hCortanaBar = FindWindowExW(hTaskBar, nullptr, L"TrayDummySearchControl", nullptr);
        if(hCortanaBar)
        {
            SendMessageTimeoutW(hCortanaBar, 0x52C, 0, 0, SMTO_NORMAL, 1000, result);
            ShowWindow(hCortanaBar, 0);
            SetParent((HWND)winId(), hCortanaBar);

            RECT rect;
            if(GetClientRect(hCortanaBar, &rect))
            {
                setGeometry(0, 0, rect.right, rect.bottom);
            }
        }
    }
#endif
}

MusicLrcContainerForCortana::~MusicLrcContainerForCortana()
{
    clearAllMusicLRCManager();
}

void MusicLrcContainerForCortana::startTimerClock()
{
    m_musicLrcContainer[!m_reverse]->startTimerClock();
}

void MusicLrcContainerForCortana::stopLrcMask()
{
    foreach(MusicLrcManager *manager, m_musicLrcContainer)
    {
        manager->stopLrcMask();
    }
}

void MusicLrcContainerForCortana::applySettingParameter()
{
    MusicLrcContainer::applySettingParameter();
    foreach(MusicLrcManager *manager, m_musicLrcContainer)
    {
        manager->setLrcFontSize(10);
    }

    const QPoint &point = M_SETTING_PTR->value(MusicSettingManager::DLrcGeometry).toPoint();
    if(!point.isNull())
    {
        move(point);
    }
}

void MusicLrcContainerForCortana::initCurrentLrc() const
{
    if(m_currentTime == 0)
    {
        m_musicLrcContainer[0]->setText(tr("welcome use TTKMusicPlayer"));
    }

    if(m_currentTime == 0)
    {
        const int width = m_musicLrcContainer[0]->x();
        m_musicLrcContainer[0]->setGeometry(0, 2, width, m_geometry.y());
        m_musicLrcContainer[1]->setGeometry(0, m_geometry.y() + 2, 0, 0);
    }
}

void MusicLrcContainerForCortana::updateCurrentLrc(const QString &first, const QString &second, qint64 time)
{
    m_reverse = !m_reverse;
    m_musicLrcContainer[m_reverse]->reset();
    m_musicLrcContainer[m_reverse]->setText(second);
    m_musicLrcContainer[!m_reverse]->setText(first);
    m_musicLrcContainer[!m_reverse]->startLrcMask(time);

    int width = m_musicLrcContainer[0]->x();
    m_musicLrcContainer[0]->setGeometry(0, 2, width, m_geometry.y());
    width = m_musicLrcContainer[1]->x();
    int pos = m_geometry.x() - width;
    if(pos < 0)
    {
        pos = 0;
    }
    m_musicLrcContainer[1]->setGeometry(pos, m_geometry.y() + 2, width, m_geometry.y());
}
