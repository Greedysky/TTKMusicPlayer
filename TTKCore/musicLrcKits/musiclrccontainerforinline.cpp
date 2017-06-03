#include "musiclrccontainerforinline.h"
#include "musiclrcmanagerforinline.h"
#include "musiclrcartphotouploadwidget.h"
#include "musiclrcfloatwidget.h"
#include "musiclrclocallinkwidget.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"
#include "musicclickedlabel.h"
#include "musiclrcanalysis.h"
#include "musiccoreutils.h"
#include "musicstringutils.h"
#include "musicnumberdefine.h"
#include "musicinlinelrcuiobject.h"
#include "musicrightareawidget.h"
#include "musiclrccommentswidget.h"
#include "musiclrctranslatedwidget.h"
#include "musiclayoutanimation.h"
#include "musicleftareawidget.h"

#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QActionGroup>
#include <QPushButton>
#include <QTextEdit>

MusicLrcContainerForInline::MusicLrcContainerForInline(QWidget *parent)
    : MusicLrcContainer(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    setLayout(vBoxLayout);

    m_lrcAnalysis = new MusicLrcAnalysis(this);
    m_lrcAnalysis->setLineMax(MUSIC_LRC_INLINE_MAX_LINE);

    m_containerType = "INLINE";
    m_layoutWidget = new MusicLayoutAnimation(this);
    m_layoutWidget->connectTo(this);
    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
       MusicLRCManager *w = new MusicLRCManagerForInline(this);
       m_layoutWidget->addWidget(w);
       m_musicLrcContainer.append(w);
    }
    vBoxLayout->addWidget(m_layoutWidget);

    setLinearGradientColor(MusicLRCManager::Origin);
    m_mouseMovedAt = QPoint(-1, -1);
    m_mousePressedAt = QPoint(-1, -1);
    m_mouseLeftPressed = false;
    m_showArtBackground = true;
    m_lrcDisplayAll = false;
    m_changeSpeedValue = 0;
    m_animationFreshTime = 0;

    initFunctionLabel();

    m_lrcFloatWidget = new MusicLrcFloatWidget(this);
    initCurrentLrc(tr("noCurrentSongPlay"));
    createNoLrcCurrentInfo();

    m_commentsWidget = nullptr;
    m_translatedWidget = nullptr;
}

MusicLrcContainerForInline::~MusicLrcContainerForInline()
{
    clearAllMusicLRCManager();
    delete m_lrcAnalysis;
    delete m_lrcFloatWidget;
    delete m_noLrcCurrentInfo;
    delete m_commentsWidget;
    delete m_translatedWidget;
    delete m_layoutWidget;
}

QString MusicLrcContainerForInline::getClassName()
{
    return staticMetaObject.className();
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

void MusicLrcContainerForInline::setMaskLinearGradientColor(const QList<QColor> &colors) const
{
    m_musicLrcContainer[m_lrcAnalysis->getMiddle()]->setMaskLinearGradientColor(colors);
}

void MusicLrcContainerForInline::setSettingParameter()
{
    MusicLrcContainer::setSettingParameter();
    int size = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
    setLrcSize(size);
}

void MusicLrcContainerForInline::updateCurrentLrc(qint64 time)
{
    if(m_lrcAnalysis->valid())
    {
        m_animationFreshTime = time;
        m_layoutWidget->start();
    }
}

bool MusicLrcContainerForInline::transLyricFileToTime(const QString &lrcFileName)
{
    m_layoutWidget->stop();

    MusicLrcAnalysis::State state;
    if(QFileInfo(lrcFileName).suffix() == "krc")
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

qint64 MusicLrcContainerForInline::setSongSpeedAndSlow(qint64 time)
{
    return m_lrcAnalysis->setSongSpeedAndSlow(time);
}

bool MusicLrcContainerForInline::findText(qint64 total, QString &pre, QString &last, qint64 &interval) const
{
    return m_lrcAnalysis->findText(m_currentTime, total, pre, last, interval);
}

void MusicLrcContainerForInline::setLrcSize(int size)
{
    if(size <= 14)
    {
        setLrcSizeProperty(0);
    }
    else if(14 < size && size <= 18)
    {
        setLrcSizeProperty(2);
    }
    else if(18 < size&& size <= 26)
    {
        setLrcSizeProperty(4);
    }
    else if(26 < size&& size <= 36)
    {
        setLrcSizeProperty(6);
    }
    else if(36 < size&& size <= 72)
    {
        setLrcSizeProperty(8);
    }

    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setLrcFontSize(size);
        m_musicLrcContainer[i]->setY(35 + size);
    }
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced, size);

    setSongSpeedAndSlow(m_currentTime);
    resizeWindow();
    setItemStyleSheet();
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
    resizeWidth(width - WINDOW_WIDTH_MIN, height - WINDOW_HEIGHT_MIN);
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

void MusicLrcContainerForInline::theArtBgChanged()
{
    m_showArtBackground = !m_showArtBackground;
    emit theArtBgHasChanged();
}

void MusicLrcContainerForInline::theArtBgUploaded()
{
    MusicLrcArtPhotoUploadWidget(this).exec();
    m_showArtBackground = true;
    emit theArtBgHasChanged();
}

void MusicLrcContainerForInline::lrcOpenFileDir() const
{
    MusicUtils::Core::openUrl(QFileInfo(m_lrcAnalysis->getCurrentFileName()).absoluteFilePath());
}

void MusicLrcContainerForInline::lrcCopyClipboard() const
{
    QClipboard *clipBoard = QApplication::clipboard();
    clipBoard->setText(m_lrcAnalysis->getAllLrcs());
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

    MusicRightAreaWidget::instance()->musicVideoButtonSearched(m_currentSongName);
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
    menu.addAction(tr("saveLrcChanged"), this, SLOT(saveLrcTimeChanged()))->setEnabled(hasLrcContainer);
    menu.addSeparator();

    //////////////////////////////////////////////////
    QActionGroup *group = new QActionGroup(this);
    QAction *ac1 = group->addAction(changeLrcSize.addAction(tr("smaller")));
    QAction *ac2 = group->addAction(changeLrcSize.addAction(tr("small")));
    QAction *ac3 = group->addAction(changeLrcSize.addAction(tr("middle")));
    QAction *ac4 = group->addAction(changeLrcSize.addAction(tr("big")));
    QAction *ac5 = group->addAction(changeLrcSize.addAction(tr("bigger")));
    int size = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
    (size == 14) ? ac1->setIcon(QIcon(":/contextMenu/btn_selected")) : ac1->setIcon(QIcon());
    (size == 18) ? ac2->setIcon(QIcon(":/contextMenu/btn_selected")) : ac2->setIcon(QIcon());
    (size == 26) ? ac3->setIcon(QIcon(":/contextMenu/btn_selected")) : ac3->setIcon(QIcon());
    (size == 36) ? ac4->setIcon(QIcon(":/contextMenu/btn_selected")) : ac4->setIcon(QIcon());
    (size == 72) ? ac5->setIcon(QIcon(":/contextMenu/btn_selected")) : ac5->setIcon(QIcon());
    ac1->setData(0);
    ac2->setData(1);
    ac3->setData(2);
    ac4->setData(3);
    ac5->setData(4);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(lrcSizeChanged(QAction*)));

    changeLrcSize.addSeparator();
    changeLrcSize.addAction(tr("custom"), this, SLOT(currentLrcCustom()));
    createColorMenu(changColorMenu);

    //////////////////////////////////////////////////
    QActionGroup *lrcTimeFastGroup = new QActionGroup(this);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast0.5s")))->setData(0);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast1s")))->setData(1);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast2s")))->setData(2);
    lrcTimeFastGroup->addAction(changeLrcTimeFast.addAction(tr("lrcTimeFast5s")))->setData(3);
    connect(lrcTimeFastGroup, SIGNAL(triggered(QAction*)), SLOT(lrcTimeSpeedChanged(QAction*)));

    //////////////////////////////////////////////////
    QActionGroup *lrcTimeSlowGroup = new QActionGroup(this);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow0.5s")))->setData(4);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow1s")))->setData(5);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow2s")))->setData(6);
    lrcTimeSlowGroup->addAction(changeLrcTimeSlow.addAction(tr("lrcTimeSlow5s")))->setData(7);
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
    menu.addAction(tr("customSetting"), this, SLOT(currentLrcCustom()));

    menu.exec(QCursor::pos());
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

void MusicLrcContainerForInline::resizeEvent(QResizeEvent *event)
{
    MusicLrcContainer::resizeEvent(event);
    resizeWindow();
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

void MusicLrcContainerForInline::revertLrcTimeSpeed(qint64 pos)
{
    m_lrcAnalysis->revertLrcTime(pos);
    qint64 beforeTime = setSongSpeedAndSlow(m_currentTime);
    updateCurrentLrc(beforeTime);

    if(m_changeSpeedValue + pos == 0)
    {
        m_changeSpeedValue = 0;
    }
    /////////////////////////////////////////////////////////
    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->setFontSize(15);
    toast->setFontMargin(10, 10);

    if(m_changeSpeedValue > 0)
        toast->setText(tr("after%1s").arg(m_changeSpeedValue*1.0/MT_S2MS));
    else if(m_changeSpeedValue < 0)
        toast->setText(tr("before%1s").arg(-m_changeSpeedValue*1.0/MT_S2MS));
    else
        toast->setText(tr("Restore"));

    QPoint globalPoint = mapToGlobal(QPoint(0, 0));
    toast->move(globalPoint.x() + (width() - toast->width())/2,
                globalPoint.y() + height() - toast->height() - 40);
    toast->show();
    /////////////////////////////////////////////////////////
}

void MusicLrcContainerForInline::createNoLrcCurrentInfo()
{
    m_noLrcCurrentInfo = new MusicClickedLabel(this);
    MusicUtils::Widget::setLabelFontSize(m_noLrcCurrentInfo, 15);
    MusicUtils::Widget::setLabelFontStyle(m_noLrcCurrentInfo, MusicObject::FT_Underline);
    m_noLrcCurrentInfo->setStyleSheet(MusicUIObject::MColorStyle06);
    m_noLrcCurrentInfo->setText(tr("makeLrc"));

    connect(m_noLrcCurrentInfo, SIGNAL(clicked()), SLOT(theCurrentLrcMaked()));
    m_noLrcCurrentInfo->hide();
}

void MusicLrcContainerForInline::showNoLrcCurrentInfo()
{
    QFontMetrics me = m_noLrcCurrentInfo->fontMetrics();
    int w = me.width(m_noLrcCurrentInfo->text());
    int h = me.height();
    int offset = height()/m_lrcAnalysis->getLineMax()*(m_lrcAnalysis->getMiddle() + 1) - 40;

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
    QWidget *functionLabel = new QWidget(this);
    functionLabel->setFixedHeight(40);
    QHBoxLayout *functionLayout = new QHBoxLayout(functionLabel);
    functionLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *translation = new QPushButton(this);
    QPushButton *movie = new QPushButton(this);
    QPushButton *microphone = new QPushButton(this);
    QPushButton *message = new QPushButton(this);

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
    functionLabel->setLayout(functionLayout);

    layout()->addWidget(functionLabel);
}

void MusicLrcContainerForInline::setItemStyleSheet()
{
    int length = m_lrcAnalysis->getLineMax();
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
    MusicLRCManagerForInline *w = MStatic_cast(MusicLRCManagerForInline*, m_musicLrcContainer[index]);
    w->setCenterOnLrc(false);
    w->setFontSize(size);

    int value = M_SETTING_PTR->value("LrcColorTransChoiced").toInt() - transparent;
    value = (value < 0) ? 0 : value;
    value = (value > 100) ? 100 : value;
    w->setFontTransparent(value);
    w->setTransparent(value);
    if(M_SETTING_PTR->value("LrcColorChoiced").toInt() != -1)
    {
        setLinearGradientColor((MusicLRCManager::LrcColorType)M_SETTING_PTR->value("LrcColorChoiced").toInt());
        setMaskLinearGradientColor( QList<QColor>() << CL_Mask << CL_White << CL_Mask );
    }
    else
    {
        w->setLinearGradientColor(MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcBgColorChoiced").toString()));
        setMaskLinearGradientColor(MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcFgColorChoiced").toString()));
    }
}

void MusicLrcContainerForInline::setLrcSizeProperty(int property)
{
    int length = MUSIC_LRC_INLINE_MAX_LINE - property;
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
        MStatic_cast(MusicLRCManagerForInline*, m_musicLrcContainer[i])->setLrcPerWidth(w);
        m_lrcFloatWidget->resizeWindow(w, h);
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
