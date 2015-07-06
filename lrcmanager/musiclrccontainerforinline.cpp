#include "musiclrccontainerforinline.h"
#include "musiclrcmanagerforinline.h"
#include <QVBoxLayout>
#include <QSettings>
#include <QDebug>
#include <QPainter>

MusicLrcContainerForInline::MusicLrcContainerForInline(QWidget *parent) :
    MusicLrcContainer(parent)
{
    m_vBoxLayout = new QVBoxLayout(this);
    this->setLayout(m_vBoxLayout);
    m_containerType = "INLINE";
    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
    {
       MusicLRCManager *w = new MusicLRCManagerForInline(this);
       m_vBoxLayout->addWidget(w);
       m_musicLrcContainer.append(w);
    }

    changeCurrentLrcColorOrigin();
    m_currentLrcIndex = 0;
    m_mouseMovedAt = QPoint(-1,-1);
    m_mousePressedAt = QPoint(-1,-1);
    m_mouseLeftPressed = false;
    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
        m_musicLrcContainer[i]->setText(".........");
    m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("noCurrentSongPlay"));

}

MusicLrcContainerForInline::~MusicLrcContainerForInline()
{
    clearAllMusicLRCManager();
    delete m_vBoxLayout;
}

bool MusicLrcContainerForInline::transLrcFileToTime(const QString& lrcFileName)
{
    static_cast<MusicLRCManagerForInline*>(m_musicLrcContainer[CURRENT_LRC_PAINT])->setUpdateLrc(false);
    m_lrcContainer.clear();///Clear the original map
    m_currentShowLrcContainer.clear();///Clear the original lrc
    QFile file(lrcFileName); ///Open the lyrics file

    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
        m_musicLrcContainer[i]->setText(".........");
    if(!file.open(QIODevice::ReadOnly))
    {
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("unFoundLrc"));
        return false;
    }
    else
        m_musicLrcContainer[CURRENT_LRC_PAINT]->setText(tr("noCurrentSongPlay"));

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
      temp.replace(reg,"");
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
        return false;
    }

    m_currentShowLrcContainer.clear();
    m_currentLrcIndex = 0;
    m_currentShowLrcContainer<<"........."<<".........";

    if(m_lrcContainer.find(0) == m_lrcContainer.end())
       m_lrcContainer.insert(0,".........");

    QMapIterator<qint64,QString> it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer.append(it.value());
    }
    m_currentShowLrcContainer<<" "<<" ";

    return true;
}

QString MusicLrcContainerForInline::text() const
{
    return m_musicLrcContainer[CURRENT_LRC_PAINT]->text();
}

void MusicLrcContainerForInline::setMaskLinearGradientColor(QColor color)
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

void MusicLrcContainerForInline::setSongSpeedAndSlow(qint64 time)
{
    foreach(qint64 value, m_lrcContainer.keys())
    {
        if(time < value)
        {
            time = value;
            break;
        }
    }
    for(int i=0; i<m_currentShowLrcContainer.count(); ++i)
    {
        if(m_currentShowLrcContainer[i] == m_lrcContainer.value(time))
        {
            m_currentLrcIndex = i - 3;
            break;
        }
    }
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
        static_cast<MusicLRCManagerForInline*>(m_musicLrcContainer[CURRENT_LRC_PAINT])->setUpdateLrc(true);
        m_musicLrcContainer[CURRENT_LRC_PAINT]->startLrcMask(time);

        for(int i=1; i<=5; ++i)
        {
            MusicLRCManagerForInline *w = static_cast<MusicLRCManagerForInline*>(m_musicLrcContainer[i-1]);
            if(i == 1 || i == 5)
            {
                w->setFontSize(2);
                w->setTransparent(85);
            }
            if(i == 2 || i == 4)
            {
                w->setFontSize(1);
                w->setTransparent(45);
            }
        }
    }
}

void MusicLrcContainerForInline::changeLrcSize(LrcSizeTable size)
{
    for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
    {
        m_musicLrcContainer[i]->setLrcFontSize(size);
    }
    QSettings().setValue("LRCSIZECHOICED",size);
}

void MusicLrcContainerForInline::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFont font;
    painter.setFont(font);
    painter.setPen(QColor(Qt::white));
    painter.drawLine(10, m_mouseMovedAt.y(), width() - 10, m_mouseMovedAt.y());
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
        int level = m_currentLrcIndex;
        m_currentLrcIndex += (m_mousePressedAt.y() - m_mouseMovedAt.y()) / 35;
        if(m_currentLrcIndex < 0) m_currentLrcIndex = 0;
        if(m_currentLrcIndex + MIN_LRCCONTAIN_COUNT < m_currentShowLrcContainer.count())
        {
            QMapIterator<qint64,QString> it(m_lrcContainer);
            for(int i=0; i<m_currentLrcIndex + 1; ++i)
                if(it.hasNext()) it.next();
            emit updateCurrentTime(it.key());
        }
        else
            m_currentLrcIndex = level;
        m_mouseMovedAt = m_mousePressedAt = QPoint(-1,-1);
        update();
    }
}

void MusicLrcContainerForInline::wheelEvent(QWheelEvent *event)
{
    MusicLrcContainer::wheelEvent(event);
    int level = m_currentLrcIndex;
    event->delta() < 0 ? --m_currentLrcIndex : ++m_currentLrcIndex;
    if(m_currentLrcIndex < 0) m_currentLrcIndex = 0;
    if(m_currentLrcIndex + MIN_LRCCONTAIN_COUNT < m_currentShowLrcContainer.count())
    {
        QMapIterator<qint64,QString> it(m_lrcContainer);
        for(int i=0; i<m_currentLrcIndex + 1; ++i)
            if(it.hasNext()) it.next();
        emit updateCurrentTime(it.key());
    }
    else
        m_currentLrcIndex = level;
    update();
}

void MusicLrcContainerForInline::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    QMenu changColorMenu(tr("changColorMenu"),this);
    QMenu changeLrcSize(tr("changeLrcSize"),this);
    changColorMenu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    changeLrcSize.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    menu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    menu.addMenu(&changColorMenu);
    menu.addMenu(&changeLrcSize);
    menu.addAction(tr("updateLrc"), this, SIGNAL(theCurrentLrcUpdated()));

    changeLrcSize.addAction(tr("smaller"),this,SLOT(changeLrcSizeSmaller()));
    changeLrcSize.addAction(tr("small"),this,SLOT(changeLrcSizeSmall()));
    changeLrcSize.addAction(tr("middle"),this,SLOT(changeLrcSizeMiddle()));
    changeLrcSize.addAction(tr("big"),this,SLOT(changeLrcSizeBig()));
    changeLrcSize.addAction(tr("bigger"),this,SLOT(changeLrcSizeBigger()));

    createColorMenu(changColorMenu);
    menu.addAction(tr("customSetting"),this,SLOT(currentLrcCustom()));

    menu.exec(QCursor::pos());
}
