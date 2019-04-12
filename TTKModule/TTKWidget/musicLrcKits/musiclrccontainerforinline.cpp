#include "musiclrccontainerforinline.h"
#include "musiclrcmanagerforinline.h"
#include "musiclrcartphotouploadwidget.h"
#include "musiclrcfloatwidget.h"
#include "musiclrcfloatplaywidget.h"
#include "musiclrclocallinkwidget.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"
#include "musicclickedlabel.h"
#include "musiclrcanalysis.h"
#include "musicurlutils.h"
#include "musicstringutils.h"
#include "musicnumberdefine.h"
#include "musicinlinelrcuiobject.h"
#include "musicrightareawidget.h"
#include "musiclrccommentswidget.h"
#include "musiclrctranslatedwidget.h"
#include "musiclrcposterwidget.h"
#include "musiclayoutanimationwidget.h"
#include "musicbottomareawidget.h"
#include "musicleftareawidget.h"
#include "musictopareawidget.h"
#include "musicbackgroundmanager.h"
#include "musictime.h"
#include "musicwidgetheaders.h"

#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QActionGroup>

#define LRC_CHANGED_OFFSET_LIMIT    20
#define LRC_TIME_LABEL_POSITION     20

MusicLrcContainerForInline::MusicLrcContainerForInline(QWidget *parent)
    : MusicLrcContainer(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    setLayout(vBoxLayout);

    m_lrcAnalysis = new MusicLrcAnalysis(this);
    m_lrcAnalysis->setLineMax(MUSIC_LRC_INLINE_MAX_LINE);

    m_containerType = LRC_INLINE_TPYE;
    m_layoutWidget = new MusicVLayoutAnimationWidget(this);
    m_layoutWidget->connectTo(this);
    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
       MusicLrcManager *w = new MusicLrcManagerForInline(this);
       m_layoutWidget->addWidget(w);
       m_musicLrcContainer.append(w);
    }
    vBoxLayout->addWidget(m_layoutWidget);

    m_lrcChangeOffset = 0;
    m_lrcChangeState = false;
    m_mouseLeftPressed = false;
    m_showArtistBackground = true;
    m_lrcDisplayAll = false;
    m_mouseMoved = false;
    m_changeSpeedValue = 0;
    m_animationFreshTime = 0;
    m_lrcChangeDelta = -1;
    m_lrcSizeProperty = -1;

    initFunctionLabel();

    m_lrcFloatWidget = new MusicLrcFloatWidget(this);
    m_floatPlayWidget = nullptr;
    initCurrentLrc(tr("noCurrentSongPlay"));
    createNoLrcCurrentInfo();

    m_commentsWidget = nullptr;
    m_translatedWidget = nullptr;
}

MusicLrcContainerForInline::~MusicLrcContainerForInline()
{
    clearAllMusicLRCManager();
    delete m_lrcAnalysis;
    delete m_functionLabel;
    delete m_lrcFloatWidget;
    delete m_floatPlayWidget;
    delete m_noLrcCurrentInfo;
    delete m_commentsWidget;
    delete m_translatedWidget;
    delete m_layoutWidget;
}

void MusicLrcContainerForInline::startTimerClock()
{
    m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->startTimerClock();
}

void MusicLrcContainerForInline::stopLrcMask()
{
    m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->stopLrcMask();
    m_layoutWidget->stop();
}

void MusicLrcContainerForInline::setSettingParameter()
{
    MusicLrcContainer::setSettingParameter();
    const int size = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
    if(m_lrcSizeProperty == -1)
    {
        m_lrcSizeProperty = size;
    }
    if(m_lrcChangeDelta == -1)
    {
        m_lrcChangeDelta = size;
    }

    setLrcSize(size);
}

void MusicLrcContainerForInline::updateCurrentLrc(qint64 time)
{
    if(m_lrcAnalysis->isValid() && !m_mouseLeftPressed)
    {
        m_animationFreshTime = time;
        m_layoutWidget->start();
    }
}

bool MusicLrcContainerForInline::transLyricFileToTime(const QString &lrcFileName)
{
    m_layoutWidget->stop();

    MusicLrcAnalysis::State state;
    if(QFileInfo(lrcFileName).suffix().toLower() == "krc")
    {
        M_LOGGER_INFO("use krc parser!");
        state = m_lrcAnalysis->transKrcFileToTime(lrcFileName);
    }
    else
    {
        M_LOGGER_INFO("use lrc parser!");
        state = m_lrcAnalysis->transLrcFileToTime(lrcFileName);
    }

    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText( QString() );
    }
    if(state == MusicLrcAnalysis::OpenFileFail)
    {
        m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->setText(tr("unFoundLrc"));
        showNoLrcCurrentInfo();
        return false;
    }
    if(state == MusicLrcAnalysis::LrcEmpty)
    {
        m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->setText(tr("lrcFileError"));
        showNoLrcCurrentInfo();
        return false;
    }
    else
    {
        m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->setText(tr("noCurrentSongPlay"));
    }

    m_noLrcCurrentInfo->hide(); ///hide error make lrc widget
    return true;
}

QString MusicLrcContainerForInline::text() const
{
    return m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->text();
}

qint64 MusicLrcContainerForInline::setSongSpeedChanged(qint64 time)
{
    return m_lrcAnalysis->setSongSpeedChanged(time);
}

bool MusicLrcContainerForInline::findText(qint64 total, QString &pre, QString &last, qint64 &interval) const
{
    return m_lrcAnalysis->findText(m_currentTime, total, pre, last, interval);
}

void MusicLrcContainerForInline::setLrcSize(int size)
{
    int index = mapLrcSizeProperty(size);
    if(index == -1)
    {
        return;
    }

    setLrcSizeProperty(index);
    index = (mapLrcSizeProperty(m_lrcSizeProperty) - index)/2;
    m_lrcSizeProperty = size;
    m_lrcAnalysis->setCurrentIndex(m_lrcAnalysis->getCurrentIndex() - index - 1);

    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setLrcFontSize(size);
        m_musicLrcContainer[i]->setY(35 + size);
        m_musicLrcContainer[i]->setText(m_lrcAnalysis->getText(i));
    }
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced, size);

    resizeWindow();
    setItemStyleSheet();
    startTimerClock();
}

int MusicLrcContainerForInline::getLrcSize() const
{
    return M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
}

void MusicLrcContainerForInline::resizeWindow()
{
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    int height = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height();

    if(m_lrcDisplayAll)
    {
        width += 320;
    }
    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        width += (m_lrcDisplayAll ? 0 : 320);
        height += 320/2;
    }

    resizeWidth(width - WINDOW_WIDTH_MIN, height - WINDOW_HEIGHT_MIN);
}

void MusicLrcContainerForInline::createFloatPlayWidget()
{
    delete m_floatPlayWidget;
    m_floatPlayWidget = nullptr;
    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        m_floatPlayWidget = new MusicLrcFloatPlayWidget(this);
        m_floatPlayWidget->resizeWindow(width(), height());
    }
}

void MusicLrcContainerForInline::lrcWidgetShowFullScreen()
{
    QHBoxLayout *l = MStatic_cast(QHBoxLayout*, m_functionLabel->layout());
    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        l->removeItem(l->itemAt(l->count() - 1));
    }
    else
    {
        l->addStretch(1);
    }
}

void MusicLrcContainerForInline::lrcSizeChanged(QAction *action)
{
    switch(action->data().toInt())
    {
        case 0: setLrcSize(14); break;
        case 1: setLrcSize(18); break;
        case 2: setLrcSize(26); break;
        case 3: setLrcSize(36); break;
        case 4: setLrcSize(72); break;
        default: break;
    }
}

void MusicLrcContainerForInline::lrcTimeSpeedChanged(QAction *action)
{
    int timeValue = 0;
    switch(action->data().toInt())
    {
        case 0: timeValue = -0.5*MT_S2MS; break;
        case 1: timeValue = -MT_S2MS; break;
        case 2: timeValue = -2*MT_S2MS; break;
        case 3: timeValue = -5*MT_S2MS; break;
        case 4: timeValue = 0.5*MT_S2MS; break;
        case 5: timeValue = MT_S2MS; break;
        case 6: timeValue = 2*MT_S2MS; break;
        case 7: timeValue = 5*MT_S2MS; break;
        default: break;
    }

    m_changeSpeedValue += timeValue;
    revertLrcTimeSpeed( timeValue );
}

void MusicLrcContainerForInline::revertLrcTimeSpeed()
{
    if(m_changeSpeedValue == 0)
    {
        return;
    }
    revertLrcTimeSpeed( -m_changeSpeedValue );
}

void MusicLrcContainerForInline::saveLrcTimeChanged()
{
    m_lrcAnalysis->saveLrcTimeChanged();
}

void MusicLrcContainerForInline::artistBackgroundChanged()
{
    m_showArtistBackground = !m_showArtistBackground;
    emit artistBackgroundHasChanged();
}

void MusicLrcContainerForInline::showArtistBackgroundUploadedWidget()
{
    MusicLrcArtPhotoUploadWidget(this).exec();
    m_showArtistBackground = true;
    emit artistBackgroundHasChanged();
}

void MusicLrcContainerForInline::lrcOpenFileDir() const
{
    MusicUtils::Url::openUrl(QFileInfo(m_lrcAnalysis->getCurrentFileName()).absoluteFilePath());
}

void MusicLrcContainerForInline::lrcCopyClipboard() const
{
    QClipboard *clipBoard = QApplication::clipboard();
    clipBoard->setText(m_lrcAnalysis->getAllLrcString());
}

void MusicLrcContainerForInline::showLocalLinkWidget()
{
    MusicLrcLocalLinkWidget w(this);
    w.setCurrentSongName(m_currentSongName);
    w.exec();
}

void MusicLrcContainerForInline::showSongCommentsWidget()
{
    if(m_currentSongName.isEmpty())
    {
        return;
    }

    delete m_commentsWidget;
    m_commentsWidget = new MusicLrcCommentsWidget(this);
    m_commentsWidget->initWidget(true);
    m_commentsWidget->setGeometry(0, height()/5, width(), height()*4/5);
    m_commentsWidget->show();
    m_commentsWidget->setCurrentSongName(m_currentSongName);
}

void MusicLrcContainerForInline::showSoundKMicroWidget()
{
    if(m_currentSongName.isEmpty())
    {
        return;
    }

    MusicLeftAreaWidget::instance()->createSoundKMicroWidget(m_currentSongName);
}

void MusicLrcContainerForInline::showLrcPosterWidget()
{
    MusicLrcPosterWidget poster(this);
    poster.setCurrentLrcs(m_lrcAnalysis->getAllLrcList(), m_currentSongName);
    poster.exec();
}

void MusicLrcContainerForInline::getTranslatedLrcFinished(const QString &data)
{
    QString text;
    foreach(const QString &var, data.split("\r"))
    {
        text += var.trimmed() + "\r\n";
    }

    delete m_translatedWidget;
    m_translatedWidget = new MusicLrcTranslatedWidget(this);
    m_translatedWidget->setPlainText(m_currentSongName, text);
    m_translatedWidget->setGeometry(0, height()/5, width(), height()*4/5);
    m_translatedWidget->show();
}

void MusicLrcContainerForInline::musicSongMovieClicked()
{
    if(m_currentSongName.isEmpty())
    {
        return;
    }

    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        MusicBottomAreaWidget::instance()->lrcWidgetShowFullScreen();
    }
    MusicRightAreaWidget::instance()->musicVideoButtonSearched(m_currentSongName, QString());
}

void MusicLrcContainerForInline::updateAnimationLrc()
{
    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText(m_lrcAnalysis->getText(i));
    }
    m_lrcAnalysis->setCurrentIndex(m_lrcAnalysis->getCurrentIndex() + 1);
    m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->startLrcMask(m_animationFreshTime);
    setItemStyleSheet();
}

void MusicLrcContainerForInline::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    QMenu changColorMenu(tr("changColorMenu"), this);
    QMenu changeLrcSize(tr("changeLrcSize"), this);
    QMenu changeLrcTimeFast(tr("changeLrcTimeFast"), this);
    QMenu changeLrcTimeSlow(tr("changeLrcTimeSlow"), this);
    QMenu changeLrcLinkMenu(tr("lrcLinkMenu"), this);

    changColorMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    changeLrcSize.setStyleSheet(MusicUIObject::MMenuStyle02);
    changeLrcTimeFast.setStyleSheet(MusicUIObject::MMenuStyle02);
    changeLrcTimeSlow.setStyleSheet(MusicUIObject::MMenuStyle02);
    changeLrcLinkMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);

    const bool hasLrcContainer = !m_lrcAnalysis->isEmpty();
    menu.addAction(tr("searchLrcs"), this, SLOT(searchMusicLrcs()));
    menu.addAction(tr("updateLrc"), this, SIGNAL(currentLrcUpdated()));
    menu.addAction(tr("makeLrc"), this, SLOT(showLrcMakedWidget()));
    menu.addAction(tr("errorLrc"), this, SLOT(showLrcErrorWidget()));
    menu.addAction(tr("lrcPoster"), this, SLOT(showLrcPosterWidget()))->setEnabled(hasLrcContainer);
    menu.addSeparator();
    menu.addAction(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen() ? tr("showNormalMode") : tr("showFullMode"),
                   MusicBottomAreaWidget::instance(), SLOT(lrcWidgetShowFullScreen()));
    menu.addSeparator();
    menu.addMenu(&changColorMenu);
    menu.addMenu(&changeLrcSize);

    menu.addMenu(&changeLrcTimeFast)->setEnabled(hasLrcContainer);
    menu.addMenu(&changeLrcTimeSlow)->setEnabled(hasLrcContainer);
    menu.addAction(tr("revert"), this, SLOT(revertLrcTimeSpeed()))->setEnabled(hasLrcContainer);
    menu.addAction(tr("saveLrcChanged"), this, SLOT(saveLrcTimeChanged()))->setEnabled(hasLrcContainer);
    menu.addSeparator();

    //
    QActionGroup *group = new QActionGroup(this);
    group->addAction(changeLrcSize.addAction(tr("smaller")))->setData(0);
    group->addAction(changeLrcSize.addAction(tr("small")))->setData(1);
    group->addAction(changeLrcSize.addAction(tr("middle")))->setData(2);
    group->addAction(changeLrcSize.addAction(tr("big")))->setData(3);
    group->addAction(changeLrcSize.addAction(tr("bigger")))->setData(4);
    int index = -1, size = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
    switch(size)
    {
        case 14: index = 0; break;
        case 18: index = 1; break;
        case 26: index = 2; break;
        case 36: index = 3; break;
        case 72: index = 4; break;
        default: break;
    }
    if(index > -1 && index < group->actions().count())
    {
        group->actions()[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
    connect(group, SIGNAL(triggered(QAction*)), SLOT(lrcSizeChanged(QAction*)));

    changeLrcSize.addSeparator();
    changeLrcSize.addAction(tr("custom"), this, SLOT(currentLrcCustom()));
    createColorMenu(changColorMenu);

    //
    QActionGroup *lrcTimeFastGroup = new QActionGroup(this);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast0.5s")))->setData(0);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast1s")))->setData(1);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast2s")))->setData(2);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast5s")))->setData(3);
    connect(lrcTimeFastGroup, SIGNAL(triggered(QAction*)), SLOT(lrcTimeSpeedChanged(QAction*)));

    //
    QActionGroup *lrcTimeSlowGroup = new QActionGroup(this);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow0.5s")))->setData(4);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow1s")))->setData(5);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow2s")))->setData(6);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow5s")))->setData(7);
    connect(lrcTimeSlowGroup, SIGNAL(triggered(QAction*)), SLOT(lrcTimeSpeedChanged(QAction*)));

    //
    QAction *artAction = menu.addAction(tr("artbgoff"), this, SLOT(artistBackgroundChanged()));
    m_showArtistBackground ? artAction->setText(tr("artbgoff")) : artAction->setText(tr("artbgon"));
    QAction *showLrc = menu.addAction(tr("lrcoff"), this, SLOT(linkLrcStateChanged()));
    m_linkLocalLrc ? showLrc->setText(tr("lrcoff")) : showLrc->setText(tr("lrcon"));
    menu.addAction(tr("artbgupload"), this, SLOT(showArtistBackgroundUploadedWidget()));
    menu.addAction(tr("artbgsetting"), MusicTopAreaWidget::instance(), SLOT(musicSetAsArtistBackground()))->setEnabled(!M_BACKGROUND_PTR->isEmpty());
    menu.addSeparator();

    const QString &fileName = m_lrcAnalysis->getCurrentFileName();
    const bool fileCheck = !fileName.isEmpty() && QFile::exists(fileName);

    changeLrcLinkMenu.addAction(tr("localLink"), this, SLOT(showLocalLinkWidget()));
    QAction *lrcLinkAc = changeLrcLinkMenu.addAction(tr("localLinkOff"), this, SLOT(linkLrcStateChanged()));
    m_linkLocalLrc ? lrcLinkAc->setText(tr("localLinkOff")) : lrcLinkAc->setText(tr("localLinkOn"));
    menu.addMenu(&changeLrcLinkMenu);
    menu.addAction(tr("copyToClip"), this, SLOT(lrcCopyClipboard()))->setEnabled( fileCheck );
    menu.addAction(tr("showLrcFile"), this, SLOT(lrcOpenFileDir()))->setEnabled( fileCheck );

    menu.addSeparator();
    menu.addAction(tr("customSetting"), this, SLOT(currentLrcCustom()));

    menu.exec(QCursor::pos());
}

void MusicLrcContainerForInline::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(m_mouseLeftPressed)
    {
        const int line = (height() - m_functionLabel->height())/2;
        QPainter painter(this);
        QFont font;
        painter.setFont(font);
        painter.setPen(QColor(Qt::white));
        painter.drawLine(0, line, width(), line);

        qint64 v = m_lrcAnalysis->getCurrentIndex() - 1;
        if(v < 0)
        {
            v = 0;
        }
        else if(v >= m_lrcAnalysis->count())
        {
            v = m_lrcAnalysis->count() - m_lrcAnalysis->getMiddle() + 2;
        }
        v = m_lrcAnalysis->findTime(v);
        painter.drawText(LRC_TIME_LABEL_POSITION, line - LRC_TIME_LABEL_POSITION/2, MusicTime::msecTime2LabelJustified(v));
    }
}

void MusicLrcContainerForInline::resizeEvent(QResizeEvent *event)
{
    MusicLrcContainer::resizeEvent(event);
    resizeWindow();
}

void MusicLrcContainerForInline::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseLeftPressed && m_lrcAnalysis->isValid())
    {
        m_layoutWidget->stop();

        m_mouseMoved = true;
        const int offset = event->globalY() - m_mousePressedAt.y();
        m_mousePressedAt = event->globalPos();

        m_lrcChangeOffset += offset;
        if(m_lrcChangeState && offset > 0)
        {
            m_lrcChangeState = false;
            m_lrcChangeOffset = 0;
        }
        else if(!m_lrcChangeState && offset < 0)
        {
            m_lrcChangeState = true;
            m_lrcChangeOffset = 0;
        }

        if(m_lrcChangeOffset !=0 && m_lrcChangeOffset % LRC_CHANGED_OFFSET_LIMIT == 0)
        {
            int index = m_lrcAnalysis->getCurrentIndex();
            index += m_lrcChangeOffset / LRC_CHANGED_OFFSET_LIMIT;
            m_lrcChangeOffset = 0;

            if(index < 0)
            {
                index = 0;
            }
            else if(index >= m_lrcAnalysis->count())
            {
                index = m_lrcAnalysis->count() - m_lrcAnalysis->getMiddle() + 2;
            }

            int value = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
            value = (mapLrcSizeProperty(m_lrcChangeDelta) - mapLrcSizeProperty(value))/2;

            m_lrcAnalysis->setCurrentIndex(index);
            for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
            {
                m_musicLrcContainer[i]->setText(m_lrcAnalysis->getText(i - value - 1));
            }
            setItemStyleSheet();
        }

        update();
    }
}

void MusicLrcContainerForInline::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mouseMoved = false;
        m_mouseLeftPressed = true;
        setCursor(Qt::CrossCursor);
        m_mousePressedAt = event->globalPos();
        m_lrcChangeState = false;
        m_lrcChangeOffset = 0;
        update();
    }
}

void MusicLrcContainerForInline::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setCursor(Qt::ArrowCursor);
        m_mouseLeftPressed = false;
        m_mousePressedAt = event->globalPos();
        update();

        if(m_lrcAnalysis->isValid() && m_mouseMoved)
        {
            const qint64 time = m_lrcAnalysis->findTime(m_lrcAnalysis->getCurrentIndex());
            if(time != -1)
            {
                emit updateCurrentTime(time);
            }
        }
    }
}

void MusicLrcContainerForInline::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    MusicBottomAreaWidget::instance()->lrcWidgetShowFullScreen();
}

void MusicLrcContainerForInline::createColorMenu(QMenu &menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->addAction(menu.addAction(tr("IYellow")))->setData(0);
    group->addAction(menu.addAction(tr("IBlue")))->setData(1);
    group->addAction(menu.addAction(tr("IGray")))->setData(2);
    group->addAction(menu.addAction(tr("IPink")))->setData(3);
    group->addAction(menu.addAction(tr("IGreen")))->setData(4);
    group->addAction(menu.addAction(tr("IRed")))->setData(5);
    group->addAction(menu.addAction(tr("IPurple")))->setData(6);
    group->addAction(menu.addAction(tr("IOrange")))->setData(7);
    group->addAction(menu.addAction(tr("IIndigo")))->setData(8);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(changeCurrentLrcColor(QAction*)));
    menu.addSeparator();
    menu.addAction(tr("custom"), this, SLOT(currentLrcCustom()));

    const int index = M_SETTING_PTR->value("LrcColorChoiced").toInt();
    if(index > -1 && index < group->actions().count())
    {
        group->actions()[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
}

void MusicLrcContainerForInline::revertLrcTimeSpeed(qint64 pos)
{
    m_lrcAnalysis->revertLrcTime(pos);
    const qint64 beforeTime = setSongSpeedChanged(m_currentTime);
    updateCurrentLrc(beforeTime);

    if(m_changeSpeedValue + pos == 0)
    {
        m_changeSpeedValue = 0;
    }
    //
    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->setFontSize(15);
    toast->setFontMargin(10, 10);

    if(m_changeSpeedValue > 0)
        toast->setText(tr("after%1s").arg(m_changeSpeedValue*1.0/MT_S2MS));
    else if(m_changeSpeedValue < 0)
        toast->setText(tr("before%1s").arg(-m_changeSpeedValue*1.0/MT_S2MS));
    else
        toast->setText(tr("Restore"));

    const QPoint &globalPoint = mapToGlobal(QPoint(0, 0));
    toast->move(globalPoint.x() + (width() - toast->width())/2, globalPoint.y() + height() - toast->height() - 40);
    toast->show();
}

void MusicLrcContainerForInline::createNoLrcCurrentInfo()
{
    m_noLrcCurrentInfo = new MusicClickedLabel(this);
    MusicUtils::Widget::setLabelFontSize(m_noLrcCurrentInfo, 15);
    MusicUtils::Widget::setLabelFontStyle(m_noLrcCurrentInfo, MusicObject::FT_Underline);
    m_noLrcCurrentInfo->setStyleSheet(MusicUIObject::MColorStyle06);
    m_noLrcCurrentInfo->setText(tr("makeLrc"));

    connect(m_noLrcCurrentInfo, SIGNAL(clicked()), SLOT(showLrcMakedWidget()));
    m_noLrcCurrentInfo->hide();
}

void MusicLrcContainerForInline::showNoLrcCurrentInfo()
{
    QFontMetrics me = m_noLrcCurrentInfo->fontMetrics();
    const int w = me.width(m_noLrcCurrentInfo->text());
    const int h = me.height();
    const int offset = height()/m_lrcAnalysis->getLineMax()*(m_lrcAnalysis->getMiddle() + 1) - 40;

    m_noLrcCurrentInfo->setGeometry((width() - w)/2, offset, w, h);
    m_noLrcCurrentInfo->show();
}

void MusicLrcContainerForInline::initCurrentLrc(const QString &str)
{
    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText( QString() );
    }
    m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->setText(str);
}

void MusicLrcContainerForInline::initFunctionLabel()
{
    m_functionLabel = new QWidget(this);
    m_functionLabel->setFixedHeight(40);
    QHBoxLayout *functionLayout = new QHBoxLayout(m_functionLabel);
    functionLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *translation = new QPushButton(this);
    QPushButton *movie = new QPushButton(this);
    QPushButton *microphone = new QPushButton(this);
    QPushButton *message = new QPushButton(this);

#ifdef Q_OS_UNIX
    translation->setFocusPolicy(Qt::NoFocus);
    movie->setFocusPolicy(Qt::NoFocus);
    microphone->setFocusPolicy(Qt::NoFocus);
    message->setFocusPolicy(Qt::NoFocus);
#endif

    translation->setFixedSize(30, 30);
    movie->setFixedSize(30, 30);
    microphone->setFixedSize(30, 30);
    message->setFixedSize(30, 30);

    translation->setStyleSheet(MusicUIObject::MKGInlineTranslation);
    movie->setStyleSheet(MusicUIObject::MKGInlineMovie);
    microphone->setStyleSheet(MusicUIObject::MKGInlineMicrophone);
    message->setStyleSheet(MusicUIObject::MKGInlineMessage);

    translation->setCursor(Qt::PointingHandCursor);
    movie->setCursor(Qt::PointingHandCursor);
    microphone->setCursor(Qt::PointingHandCursor);
    message->setCursor(Qt::PointingHandCursor);

    translation->setToolTip(tr("Translation"));
    movie->setToolTip(tr("MV"));
    microphone->setToolTip(tr("KMicro"));
    message->setToolTip(tr("Message"));

    connect(translation, SIGNAL(clicked()), m_lrcAnalysis, SLOT(getTranslatedLrc()));
    connect(movie, SIGNAL(clicked()), SLOT(musicSongMovieClicked()));
    connect(microphone, SIGNAL(clicked()), SLOT(showSoundKMicroWidget()));
    connect(message, SIGNAL(clicked()), SLOT(showSongCommentsWidget()));

    functionLayout->addStretch(1);
    functionLayout->addWidget(translation);
    functionLayout->addWidget(movie);
    functionLayout->addWidget(microphone);
    functionLayout->addWidget(message);
    functionLayout->addStretch(1);
    m_functionLabel->setLayout(functionLayout);

    layout()->addWidget(m_functionLabel);
}

void MusicLrcContainerForInline::setItemStyleSheet()
{
    const int length = m_lrcAnalysis->getLineMax();
    if(length == 11)
    {
        for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
        {
            if(i == 0 || i == 10) setItemStyleSheet(i, 5, 90);
            else if(i == 1 || i == 9) setItemStyleSheet(i, 4, 80);
            else if(i == 2 || i == 8) setItemStyleSheet(i, 3, 60);
            else if(i == 3 || i == 7) setItemStyleSheet(i, 2, 40);
            else if(i == 4 || i == 6) setItemStyleSheet(i, 1, 20);
            else setItemStyleSheet(i, 0, 0);
        }
    }
    else if(length == 9)
    {
        for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
        {
            if(i == 0 || i == 8) setItemStyleSheet(i, 8, 90);
            else if(i == 1 || i == 7) setItemStyleSheet(i, 6, 75);
            else if(i == 2 || i == 6) setItemStyleSheet(i, 4, 50);
            else if(i == 3 || i == 5) setItemStyleSheet(i, 2, 25);
            else setItemStyleSheet(i, 0, 0);
        }
    }
    else if(length == 7)
    {
        for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
        {
            if(i == 0 || i == 6) setItemStyleSheet(i, 12, 90);
            else if(i == 1 || i == 5) setItemStyleSheet(i, 8, 60);
            else if(i == 2 || i == 4) setItemStyleSheet(i, 4, 30);
            else setItemStyleSheet(i, 0, 0);
        }
    }
    else if(length == 5)
    {
        for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
        {
            if(i == 0 || i == 4) setItemStyleSheet(i, 16, 90);
            else if(i == 1 || i == 3) setItemStyleSheet(i, 8, 45);
            else setItemStyleSheet(i, 0, 0);
        }
    }
    else if(length == 3)
    {
        for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
        {
            if(i == 0 || i == 2) setItemStyleSheet(i, 20, 90);
            else setItemStyleSheet(i, 0, 0);
        }
    }
}

void MusicLrcContainerForInline::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLrcManagerForInline *w = MStatic_cast(MusicLrcManagerForInline*, m_musicLrcContainer[index]);
    w->setFontSize(size);

    int value = M_SETTING_PTR->value("LrcColorTransChoiced").toInt() - transparent;
    value = (value < 0) ? 0 : value;
    value = (value > 100) ? 100 : value;
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(M_SETTING_PTR->value("LrcColorChoiced").toInt() != -1)
    {
        const MusicLrcColor::LrcColorType index = MStatic_cast(MusicLrcColor::LrcColorType, M_SETTING_PTR->value("LrcColorChoiced").toInt());
        setLinearGradientColor(index);
    }
    else
    {
        const MusicLrcColor cl(MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcFrontgroundColorChoiced").toString()),
                               MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcBackgroundColorChoiced").toString()));
        setLinearGradientColor(cl);
    }
}

int MusicLrcContainerForInline::mapLrcSizeProperty(int size)
{
    if(size <= 14)
    {
        return 0;
    }
    else if(14 < size && size <= 18)
    {
        return 2;
    }
    else if(18 < size && size <= 26)
    {
        return 4;
    }
    else if(26 < size && size <= 36)
    {
        return 6;
    }
    else if(36 < size && size <= 72)
    {
        return 8;
    }
    else
    {
        return -1;
    }
}

void MusicLrcContainerForInline::setLrcSizeProperty(int property)
{
    const int length = MUSIC_LRC_INLINE_MAX_LINE - property;
    m_lrcAnalysis->setLineMax(length);
    for(int i=0; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
        m_musicLrcContainer[i]->show();
        m_musicLrcContainer[i]->reset();
        m_layoutWidget->removeWidget(m_musicLrcContainer[i]);
    }
    for(int i=0; i<length; ++i)
    {
        m_layoutWidget->addWidget(m_musicLrcContainer[i]);
    }
    for(int i=length; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
        m_musicLrcContainer[i]->hide();
    }
}

void MusicLrcContainerForInline::resizeWidth(int w, int h)
{
    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
        MStatic_cast(MusicLrcManagerForInline*, m_musicLrcContainer[i])->setLrcPerWidth(w);
    }

    m_lrcFloatWidget->resizeWindow(w, h);
    if(m_floatPlayWidget)
    {
        m_floatPlayWidget->resizeWindow(width(), height());
    }

    if(m_lrcAnalysis->isEmpty())
    {
        initCurrentLrc(tr("unFoundLrc"));
        showNoLrcCurrentInfo();
    }
    else if(m_currentTime != 0 && m_lrcAnalysis->getCurrentIndex() == 0)
    {
        initCurrentLrc(tr("noCurrentSongPlay"));
    }

    if(m_commentsWidget)
    {
        m_commentsWidget->setGeometry(0, height()/5, width(), height()*4/5);
    }
    if(m_translatedWidget)
    {
        m_translatedWidget->setGeometry(0, height()/5, width(), height()*4/5);
    }
}
