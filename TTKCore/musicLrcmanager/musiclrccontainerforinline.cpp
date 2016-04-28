#include "musiclrccontainerforinline.h"
#include "musiclrcmanagerforinline.h"
#include "musiclrcartphotoupload.h"
#include "musiclrcfloatwidget.h"
#include "musiclrcerrorwidget.h"
#include "musiclrclocallinkwidget.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"
#include "musicclickedlabel.h"
#include "musiclrcanalysis.h"
#include "musicutils.h"

#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QActionGroup>

MusicLrcContainerForInline::MusicLrcContainerForInline(QWidget *parent)
    : MusicLrcContainer(parent)
{
    m_vBoxLayout = new QVBoxLayout(this);
    m_vBoxLayout->setMargin(0);

    setLayout(m_vBoxLayout);
    m_containerType = "INLINE";
    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
    {
       MusicLRCManager *w = new MusicLRCManagerForInline(this);
       m_vBoxLayout->addWidget(w);
       m_musicLrcContainer.append(w);
    }

    setLinearGradientColor(MusicLRCManager::Origin);
    m_mouseMovedAt = QPoint(-1, -1);
    m_mousePressedAt = QPoint(-1, -1);
    m_mouseLeftPressed = false;
    m_showArtBackground = true;
    m_changeSpeedValue = 0;

    createNoLrcCurrentInfo();
    initCurrentLrc(tr("noCurrentSongPlay"));

    m_lrcAnalysis = new MusicLrcAnalysis(this);
    m_lrcFloatWidget = new MusicLrcFloatWidget(this);
}

MusicLrcContainerForInline::~MusicLrcContainerForInline()
{
    clearAllMusicLRCManager();
    delete m_vBoxLayout;
    delete m_lrcAnalysis;
    delete m_lrcFloatWidget;
    delete m_noLrcCurrentInfo;
}

bool MusicLrcContainerForInline::transLrcFileToTime(const QString &lrcFileName)
{
    MusicLrcAnalysis::State state = m_lrcAnalysis->transLrcFileToTime(lrcFileName);
    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
    {
        m_musicLrcContainer[i]->setText( QString() );
    }
    if(state == MusicLrcAnalysis::OpenFileFail)
    {
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("unFoundLrc"));
        showNoLrcCurrentInfo();
        return false;
    }
    if(state == MusicLrcAnalysis::LrcEmpty)
    {
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("lrcFileError"));
        showNoLrcCurrentInfo();
        return false;
    }
    else
    {
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("noCurrentSongPlay"));
    }

    m_noLrcCurrentInfo->hide(); ///hide error make lrc widget
    return true;
}

QString MusicLrcContainerForInline::text() const
{
    return m_musicLrcContainer[CURRENT_LRC_PAINT]->text();
}

void MusicLrcContainerForInline::setMaskLinearGradientColor(QColor color) const
{
    m_musicLrcContainer[CURRENT_LRC_PAINT]->setMaskLinearGradientColor(color);
}

void MusicLrcContainerForInline::setSettingParameter()
{
    MusicLrcContainer::setSettingParameter();
    setItemStyleSheet();
}

void MusicLrcContainerForInline::setItemStyleSheet()
{
    for(int i=0; i< MIN_LRCCONTAIN_COUNT; ++i)
    {
        if(i == 0 || i == 10) setItemStyleSheet(i, 5, 90);
        else if(i == 1 || i == 9) setItemStyleSheet(i, 4, 80);
        else if(i == 2 || i == 8) setItemStyleSheet(i, 3, 60);
        else if(i == 3 || i == 7) setItemStyleSheet(i, 2, 40);
        else if(i == 4 || i == 6) setItemStyleSheet(i, 1, 20);
        else setItemStyleSheet(i, 0, 0);
    }
}

void MusicLrcContainerForInline::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLRCManagerForInline *w = MStatic_cast(MusicLRCManagerForInline*, m_musicLrcContainer[index]);
    w->setCenterOnLrc(index != CURRENT_LRC_PAINT);
    w->setFontSize(size);

    int value = M_SETTING->value("LrcColorTransChoiced").toInt() - transparent;
    value = (value < 0) ? 0 : value;
    value = (value > 100) ? 100 : value;
    w->setFontTransparent(value);
    w->setTransparent(value);
    if(M_SETTING->value("LrcColorChoiced").toInt() != -1)
    {
        setLinearGradientColor((MusicLRCManager::LrcColorType)M_SETTING->value("LrcColorChoiced").toInt());
        setMaskLinearGradientColor();
    }
    else
    {
        w->setLinearGradientColor(M_SETTING->value("LrcBgColorChoiced").value<QColor>());
        setMaskLinearGradientColor(M_SETTING->value("LrcFgColorChoiced").value<QColor>());
    }
}

void MusicLrcContainerForInline::startTimerClock()
{
    m_musicLrcContainer[CURRENT_LRC_PAINT]->startTimerClock();
}

void MusicLrcContainerForInline::stopLrcMask()
{
    m_musicLrcContainer[CURRENT_LRC_PAINT]->stopLrcMask();
}

qint64 MusicLrcContainerForInline::setSongSpeedAndSlow(qint64 time)
{
    return m_lrcAnalysis->setSongSpeedAndSlow(time);
}

bool MusicLrcContainerForInline::findText(qint64 total, QString &pre, QString &last, qint64 &interval) const
{
    return m_lrcAnalysis->findText(m_currentTime, total, pre, last, interval);
}

void MusicLrcContainerForInline::updateCurrentLrc(qint64 time)
{
    if(m_lrcAnalysis->valid())
    {
        for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
        {
            m_musicLrcContainer[i]->setText(m_lrcAnalysis->getText(i));
        }
        m_lrcAnalysis->setCurrentIndex(m_lrcAnalysis->getCurrentIndex() + 1);
        m_musicLrcContainer[CURRENT_LRC_PAINT]->startLrcMask(time);

        setItemStyleSheet();
    }
}

void MusicLrcContainerForInline::setLrcSize(MusicLRCManager::LrcSizeTable size) const
{
    if(size < 13 || size > 17)
    {
        M_LOGGER_ERROR("set lrc size error!");
        return;
    }
    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
    {
        m_musicLrcContainer[i]->setLrcFontSize(size);
    }
    M_SETTING->setValue(MusicSettingManager::LrcSizeChoiced, size);
}

int MusicLrcContainerForInline::getLrcSize() const
{
    return M_SETTING->value(MusicSettingManager::LrcSizeChoiced).toInt();
}

void MusicLrcContainerForInline::createNoLrcCurrentInfo()
{
    m_noLrcCurrentInfo = new MusicClickedLabel(this);
    QFont f = m_noLrcCurrentInfo->font();
    f.setPointSize(15);
    f.setUnderline(true);
    m_noLrcCurrentInfo->setStyleSheet("color:rgb(244, 244, 244)");
    m_noLrcCurrentInfo->setFont(f);
    m_noLrcCurrentInfo->setText(tr("makeLrc"));

    connect(m_noLrcCurrentInfo, SIGNAL(clicked()), SLOT(theCurrentLrcMaked()));
    m_noLrcCurrentInfo->hide();
}

void MusicLrcContainerForInline::showNoLrcCurrentInfo()
{
    QRect rect = m_musicLrcContainer[CURRENT_LRC_PAINT + 1]->geometry();
    QFontMetrics me = m_noLrcCurrentInfo->fontMetrics();
    int w = me.width(m_noLrcCurrentInfo->text());
    int h = me.height();

    m_noLrcCurrentInfo->setGeometry((rect.width() - w)/2, rect.y(), w, h);
    m_noLrcCurrentInfo->show();
}

void MusicLrcContainerForInline::initCurrentLrc(const QString &str)
{
    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
    {
        m_musicLrcContainer[i]->setText( QString() );
    }
    m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(str);
}

void MusicLrcContainerForInline::resizeWidth(int width)
{
    for(int i=0; i< MIN_LRCCONTAIN_COUNT; ++i)
    {
        MStatic_cast(MusicLRCManagerForInline*, m_musicLrcContainer[i])->setLrcPerWidth(width);
        m_lrcFloatWidget->resizeWidth(width);
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
}

void MusicLrcContainerForInline::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFont font;
    painter.setFont(font);
    painter.setPen(QColor(Qt::white));
    painter.drawLine(0, m_mouseMovedAt.y(), width(), m_mouseMovedAt.y());
    painter.end();
}

void MusicLrcContainerForInline::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseLeftPressed)
    {
        m_mouseMovedAt = event->pos();
        update();
    }
}

void MusicLrcContainerForInline::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mouseLeftPressed = true;
        setCursor(Qt::CrossCursor);
        m_mouseMovedAt = m_mousePressedAt = event->pos();
        update();
    }
}

void MusicLrcContainerForInline::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setCursor(Qt::ArrowCursor);
        m_mouseLeftPressed = false;
        changeLrcPostion("mouse");
        m_mouseMovedAt = m_mousePressedAt = QPoint(-1, -1);
        update();
    }
}

void MusicLrcContainerForInline::wheelEvent(QWheelEvent *event)
{
    MusicLrcContainer::wheelEvent(event);
    changeLrcPostion(QString::number(event->delta()));
}

void MusicLrcContainerForInline::changeLrcPostion(const QString &type)
{
    int index = m_lrcAnalysis->getCurrentIndex();
    int level = index;
    if(type == "mouse")
    {
        index += (m_mousePressedAt.y() - m_mouseMovedAt.y()) / 35;
    }
    else
    {
        type.toInt() < 0 ? --index : ++index;
    }

    if(index < 0)
    {
        index = 0;
    }

    qint64 time = m_lrcAnalysis->findTime(index);
    if(time != -1)
    {
        emit updateCurrentTime(time);
    }
    m_lrcAnalysis->setCurrentIndex(time != -1 ? index : level);
}

void MusicLrcContainerForInline::contextMenuEvent(QContextMenuEvent *)
{
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

    menu.addAction(tr("searchLrcs"), this, SLOT(searchMusicLrcs()));
    menu.addAction(tr("updateLrc"), this, SIGNAL(theCurrentLrcUpdated()));
    menu.addAction(tr("makeLrc"), this, SLOT(theCurrentLrcMaked()));
    menu.addAction(tr("errorLrc"), this, SLOT(theCurrentLrcError()));
    menu.addSeparator();
    menu.addMenu(&changColorMenu);
    menu.addMenu(&changeLrcSize);
    bool hasLrcContainer = !m_lrcAnalysis->isEmpty();
    menu.addMenu(&changeLrcTimeFast)->setEnabled(hasLrcContainer);
    menu.addMenu(&changeLrcTimeSlow)->setEnabled(hasLrcContainer);
    menu.addAction(tr("revert"), this, SLOT(revertLrcTimeSpeed()))->setEnabled(hasLrcContainer);
    menu.addSeparator();

    //////////////////////////////////////////////////
    QActionGroup *group = new QActionGroup(this);
    group->addAction(changeLrcSize.addAction(tr("smaller")));
    group->addAction(changeLrcSize.addAction(tr("small")));
    group->addAction(changeLrcSize.addAction(tr("middle")));
    group->addAction(changeLrcSize.addAction(tr("big")));
    group->addAction(changeLrcSize.addAction(tr("bigger")));
    connect(group, SIGNAL(triggered(QAction*)), SLOT(lrcSizeChanged(QAction*)));

    changeLrcSize.addSeparator();
    changeLrcSize.addAction(tr("custom"),this,SLOT(currentLrcCustom()));
    createColorMenu(changColorMenu);

    //////////////////////////////////////////////////
    QActionGroup *lrcTimeFastGroup = new QActionGroup(this);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast0.5s")));
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast1s")));
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast2s")));
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast5s")));
    connect(lrcTimeFastGroup, SIGNAL(triggered(QAction*)), SLOT(lrcTimeSpeedChanged(QAction*)));

    //////////////////////////////////////////////////
    QActionGroup *lrcTimeSlowGroup = new QActionGroup(this);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow0.5s")));
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow1s")));
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow2s")));
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow5s")));
    connect(lrcTimeSlowGroup, SIGNAL(triggered(QAction*)), SLOT(lrcTimeSpeedChanged(QAction*)));

    //////////////////////////////////////////////////
    QAction *artBgAc = menu.addAction(tr("artbgoff"), this, SLOT(theArtBgChanged()));
    m_showArtBackground ? artBgAc->setText(tr("artbgoff")) : artBgAc->setText(tr("artbgon")) ;
    QAction *showLrc = menu.addAction(tr("lrcoff"), this, SLOT(theLinkLrcChanged()));
    m_linkLocalLrc ? showLrc->setText(tr("lrcoff")) : showLrc->setText(tr("lrcon"));
    menu.addAction(tr("artbgupload"), this, SLOT(theArtBgUploaded()));
    menu.addSeparator();

    QString fileName = m_lrcAnalysis->getCurrentFileName();
    bool fileCheck = !fileName.isEmpty() && QFile::exists(fileName);
    changeLrcLinkMenu.addAction(tr("localLink"), this, SLOT(showLocalLinkWidget()));
    QAction *lrcLinkAc = changeLrcLinkMenu.addAction(tr("localLinkOff"), this, SLOT(theLinkLrcChanged()));
    m_linkLocalLrc ? lrcLinkAc->setText(tr("localLinkOff")) : lrcLinkAc->setText(tr("localLinkOn"));
    menu.addMenu(&changeLrcLinkMenu);
    menu.addAction(tr("copyToClip"), this, SLOT(lrcCopyClipboard()))->setEnabled( fileCheck );
    menu.addAction(tr("showLrcFile"), this, SLOT(lrcOpenFileDir()))->setEnabled( fileCheck );

    menu.addSeparator();
    menu.addAction(tr("customSetting"),this,SLOT(currentLrcCustom()));

    menu.exec(QCursor::pos());
}

void MusicLrcContainerForInline::lrcSizeChanged(QAction *action) const
{
    QString text = action->text();
    if(text == tr("smaller")) setLrcSize(MusicLRCManager::Smaller);
    else if (text == tr("small")) setLrcSize(MusicLRCManager::Small);
    else if (text == tr("middle")) setLrcSize(MusicLRCManager::Middle);
    else if (text == tr("big")) setLrcSize(MusicLRCManager::Big);
    else if (text == tr("bigger")) setLrcSize(MusicLRCManager::Bigger);
}

void MusicLrcContainerForInline::lrcTimeSpeedChanged(QAction *action)
{
    QString text = action->text();
    int timeValue = 0;
    if(text == tr("lrcTimeFast0.5s")) timeValue = -500;
    else if(text == tr("lrcTimeFast1s")) timeValue = -1000;
    else if(text == tr("lrcTimeFast2s")) timeValue = -2000;
    else if(text == tr("lrcTimeFast5s")) timeValue = -5000;
    else if(text == tr("lrcTimeSlow0.5s")) timeValue = 500;
    else if(text == tr("lrcTimeSlow1s")) timeValue = 1000;
    else if(text == tr("lrcTimeSlow2s")) timeValue = 2000;
    else if(text == tr("lrcTimeSlow5s")) timeValue = 5000;

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
    m_changeSpeedValue = 0;
}

void MusicLrcContainerForInline::revertLrcTimeSpeed(qint64 pos)
{
    m_lrcAnalysis->revertLrcTime(pos);
    qint64 beforeTime = setSongSpeedAndSlow(m_currentTime);
    updateCurrentLrc(beforeTime);

    /////////////////////////////////////////////////////////
    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->setFontSize(13);
    QString str;
    if(m_changeSpeedValue >= 0)
    {
        str = tr("after%1s").arg(m_changeSpeedValue/1000.0);
    }
    else
    {
        str = tr("before%1s").arg(-m_changeSpeedValue/1000.0);
    }
    toast->setText(str);
    QPoint globalPoint = mapToGlobal(QPoint(0, 0));
    int x = globalPoint.x() + (width() - toast->width())/2;
    int y = globalPoint.y() + height() - toast->height() - 20;
    toast->move(x, y);
    toast->show();
    /////////////////////////////////////////////////////////
}

void MusicLrcContainerForInline::theArtBgChanged()
{
    m_showArtBackground = !m_showArtBackground;
    emit theArtBgHasChanged();
}

void MusicLrcContainerForInline::theArtBgUploaded()
{
    MusicLrcArtPhotoUpload(this).exec();
    m_showArtBackground = true;
    emit theArtBgHasChanged();
}

void MusicLrcContainerForInline::lrcOpenFileDir() const
{
    MusicUtils::openUrl(QFileInfo(m_lrcAnalysis->getCurrentFileName()).absoluteFilePath());
}

void MusicLrcContainerForInline::lrcCopyClipboard() const
{
    QClipboard *clipBoard = QApplication::clipboard();
    clipBoard->setText(m_lrcAnalysis->getAllLrcs());
}

void MusicLrcContainerForInline::theCurrentLrcError()
{
    MusicLrcErrorWidget(this).exec();
}

void MusicLrcContainerForInline::showLocalLinkWidget()
{
    MusicLrcLocalLinkWidget w(this);
    w.setCurrentSongName(m_currentSongName);
    w.exec();
}
