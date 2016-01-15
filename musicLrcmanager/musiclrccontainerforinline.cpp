#include "musiclrccontainerforinline.h"
#include "musiclrcmanagerforinline.h"
#include "musiclrcartphotoupload.h"
#include "musiclrcfloatwidget.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"
#include "musicclickedlabel.h"

#include <QUrl>
#include <QPainter>
#include <QDesktopServices>
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
    m_currentLrcIndex = 0;
    m_mouseMovedAt = QPoint(-1, -1);
    m_mousePressedAt = QPoint(-1, -1);
    m_mouseLeftPressed = false;
    m_showArtBackground = true;
    m_showInlineLrc = true;
    m_changeSpeedValue = 0;

    createNoLrcCurrentInfo();
    initLrc(tr("noCurrentSongPlay"));

    m_lrcFloatWidget = new MusicLrcFloatWidget(this);
}

MusicLrcContainerForInline::~MusicLrcContainerForInline()
{
    clearAllMusicLRCManager();
    delete m_vBoxLayout;
    delete m_lrcFloatWidget;
    delete m_noLrcCurrentInfo;
}

bool MusicLrcContainerForInline::transLrcFileToTime(const QString &lrcFileName)
{
    m_lrcContainer.clear();///Clear the original map
    m_currentShowLrcContainer.clear();///Clear the original lrc
    QFile file(m_currentLrcFileName = lrcFileName); ///Open the lyrics file

    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
    {
        m_musicLrcContainer[i]->setText( QString() );
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("unFoundLrc"));
        showNoLrcCurrentInfo();
        return false;
    }
    else
    {
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("noCurrentSongPlay"));
    }

    QString getAllText = QString(file.readAll());
    file.close();
    //The lyrics by line into the lyrics list
    QStringList lines = getAllText.split("\n");
    //This is the time the label format[00:05.54]
    //Regular expressionsd {2} Said matching 2 numbers
    QRegExp reg("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    foreach(QString oneLine, lines)
    {
        QString temp = oneLine;
        temp.replace(reg, "");
        /*Replace the regular expression matching in place with the empty
        string, then we get the lyrics text,And then get all the time the
        label in the current row, and separately with lyrics text into QMap
        IndexIn () to return to the first matching position, if the return
        is -1, said no match,Under normal circumstances, POS should be
        followed is corresponding to the lyrics file
        */
        int pos = reg.indexIn(oneLine, 0);
        while(pos != -1)
        { //That match
            QString cap = reg.cap(0);
            //Return zeroth expression matching the content
            //The time tag into the time value, in milliseconds
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int millisecond = regexp.cap(0).toInt();
            qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
            //Insert into lrcContainer
            m_lrcContainer.insert(totalTime, temp);
            pos += reg.matchedLength();
            pos = reg.indexIn(oneLine, pos);//Matching all
        }
    }
    //If the lrcContainer is empty
    if (m_lrcContainer.isEmpty())
    {
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("lrcFileError"));
        showNoLrcCurrentInfo();
        return false;
    }

    m_noLrcCurrentInfo->hide(); ///hide error make lrc widget
    m_currentShowLrcContainer.clear();
    m_currentLrcIndex = 0;

    for(int i=0; i<MIN_LRCCONTAIN_COUNT/2; ++i)
    {
        m_currentShowLrcContainer << QString();
    }
    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
       m_lrcContainer.insert(0, QString());
    }

    MIntStringMapIt it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }
    for(int i=0; i<MIN_LRCCONTAIN_COUNT/2; ++i)
    {
        m_currentShowLrcContainer << QString();
    }
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
    QList<qint64> keys(m_lrcContainer.keys());
    qint64 beforeTime;
    if(!keys.isEmpty())
    {
        beforeTime = keys[0];
    }
    for(int i=1; i<keys.count(); ++i)
    {
        qint64 afterTime = keys[i];
        if(beforeTime <= time && time <= afterTime)
        {
            time = afterTime;
            break;
        }
        beforeTime = afterTime;
    }

    for(int i=0; i<m_currentShowLrcContainer.count(); ++i)
    {
        if(m_currentShowLrcContainer[i] == m_lrcContainer.value(time))
        {
            if((m_currentLrcIndex = i - CURRENT_LRC_PAINT - 1) < 0 )
            {
                m_currentLrcIndex = 0;
            }
            break;
        }
    }
    return time;
}

void MusicLrcContainerForInline::updateCurrentLrc(qint64 time)
{
    if(!m_lrcContainer.isEmpty() &&
        m_currentLrcIndex + MIN_LRCCONTAIN_COUNT <= m_currentShowLrcContainer.count())
    {
        for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
        {
            m_musicLrcContainer[i]->setText(m_currentShowLrcContainer[m_currentLrcIndex + i]);
        }
        ++m_currentLrcIndex;
        m_musicLrcContainer[CURRENT_LRC_PAINT]->startLrcMask(time);

        for(int i=1; i<= MIN_LRCCONTAIN_COUNT; ++i)
        {
            MusicLRCManagerForInline *w = static_cast<MusicLRCManagerForInline*>(m_musicLrcContainer[i-1]);
            w->setCenterOnLrc(true);
            if(i == 1 || i == 9)
            {
                w->setFontSize(3);
                w->setTransparent(220);
            }
            else if(i == 2 || i == 8)
            {
                w->setFontSize(2);
                w->setTransparent(105);
            }
            else if(i == 3 || i == 4 || i == 6 || i == 7)
            {
                w->setFontSize(1);
                w->setTransparent(45);
            }
            else
            {
                w->setCenterOnLrc(false);
            }
        }
    }
}

void MusicLrcContainerForInline::setLrcSize(MusicLRCManager::LrcSizeTable size) const
{
    if(size < 13 || size > 17)
    {
        M_LOGGER << "set lrc size error!" << LOG_END;
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
    QFontMetrics me = m_noLrcCurrentInfo->fontMetrics();
    int w = me.width(m_noLrcCurrentInfo->text());
    int h = me.height();

    ///there is a bug when playing not click the lrc widget
    m_noLrcCurrentInfo->setGeometry((width() - w)/2, (height() - h + 30)/2, w, h);
    m_noLrcCurrentInfo->show();
}

void MusicLrcContainerForInline::initLrc(const QString &str)
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
        static_cast<MusicLRCManagerForInline*>(m_musicLrcContainer[i])->setLrcPerWidth(width);
        m_lrcFloatWidget->resizeWidth(width);
    }

    if(m_lrcContainer.isEmpty())
    {
        initLrc(tr("unFoundLrc"));
        showNoLrcCurrentInfo();
    }
    else if(m_currentTime != 0 && m_currentLrcIndex == 0)
    {
        initLrc(tr("noCurrentSongPlay"));
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
    int level = m_currentLrcIndex;
    if(type == "mouse")
    {
        m_currentLrcIndex += (m_mousePressedAt.y() - m_mouseMovedAt.y()) / 35;
    }
    else
    {
        type.toInt() < 0 ? --m_currentLrcIndex : ++m_currentLrcIndex;
    }

    if(m_currentLrcIndex < 0)
    {
        m_currentLrcIndex = 0;
    }
    if(m_currentLrcIndex + MIN_LRCCONTAIN_COUNT < m_currentShowLrcContainer.count())
    {
        MIntStringMapIt it(m_lrcContainer);
        for(int i=0; i<m_currentLrcIndex + 1; ++i)
        {
            if(it.hasNext())
            {
                it.next();
            }
        }
        emit updateCurrentTime(it.key());
    }
    else
    {
        m_currentLrcIndex = level;
    }
}

void MusicLrcContainerForInline::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    QMenu changColorMenu(tr("changColorMenu"), this);
    QMenu changeLrcSize(tr("changeLrcSize"), this);
    QMenu changeLrcTimeFast(tr("changeLrcTimeFast"), this);
    QMenu changeLrcTimeSlow(tr("changeLrcTimeSlow"), this);

    changColorMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    changeLrcSize.setStyleSheet(MusicUIObject::MMenuStyle02);
    changeLrcTimeFast.setStyleSheet(MusicUIObject::MMenuStyle02);
    changeLrcTimeSlow.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);

    menu.addAction(tr("searchLrcs"), this, SLOT(searchMusicLrcs()));
    menu.addAction(tr("updateLrc"), this, SIGNAL(theCurrentLrcUpdated()));
    menu.addAction(tr("makeLrc"), this, SLOT(theCurrentLrcMaked()));
    menu.addSeparator();
    menu.addMenu(&changColorMenu);
    menu.addMenu(&changeLrcSize);
    bool hasLrcContainer = !m_lrcContainer.isEmpty();
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
    QAction *showLrc = menu.addAction(tr("lrcoff"), this, SLOT(theShowLrcChanged()));
    m_showInlineLrc ? showLrc->setText(tr("lrcoff")) : showLrc->setText(tr("lrcon"));
    menu.addAction(tr("artbgupload"), this, SLOT(theArtBgUploaded()));
    menu.addSeparator();
    bool fileCheck = !m_currentLrcFileName.isEmpty() && QFile::exists(m_currentLrcFileName);
    QAction *copyToClipAc = menu.addAction(tr("copyToClip"),this,SLOT(lrcCopyClipboard()));
    copyToClipAc->setEnabled( fileCheck );
    QAction *showLrcFileAc = menu.addAction(tr("showLrcFile"),this,SLOT(lrcOpenFileDir()));
    showLrcFileAc->setEnabled( fileCheck );

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
    MIntStringMapIt it(m_lrcContainer);
    MIntStringMap copy;
    while(it.hasNext())
    {
        it.next();
        copy.insert(it.key() + pos, it.value());
    }
    m_lrcContainer = copy;

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
    MusicLrcArtPhotoUpload artDialog(this);
    artDialog.exec();
    m_showArtBackground = true;
    emit theArtBgHasChanged();
}

void MusicLrcContainerForInline::theShowLrcChanged()
{
    m_showInlineLrc = !m_showInlineLrc;
    foreach(MusicLRCManager *w, m_musicLrcContainer)
    {
        w->setVisible( m_showInlineLrc );
    }
}

void MusicLrcContainerForInline::lrcOpenFileDir() const
{
    QDesktopServices::openUrl(QUrl(QFileInfo(m_currentLrcFileName).absolutePath(), QUrl::TolerantMode));
}

void MusicLrcContainerForInline::lrcCopyClipboard() const
{
    QClipboard *clipBoard = QApplication::clipboard();
    QString clipString;
    foreach(QString s, m_lrcContainer.values())
    {
        clipString.append(s);
    }
    clipBoard->setText(clipString);
}
