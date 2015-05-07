#include "musicremotewidgetforrectangle.h"
#include <QDesktopWidget>

MusicRemoteWidgetForRectangle::MusicRemoteWidgetForRectangle(QWidget *parent)
            : MusicRemoteWidget(parent)
{
    setGeometry(200,200,230,70);
    QRect deskRect = QApplication::desktop()->availableGeometry();
    move( deskRect.width() - width() - 150, height() + 70);

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(255,0,0,10));
    setPalette(pal);

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(5,5,5,2);
    vbox->setSpacing(0);
    vbox->addWidget(m_mainWidget);

    QString styleButton = "\
        QPushButton{background:transparent;border-none;}\
        QPushButton::hover{image:url(:/image/shadow);}";
    m_PreSongButton->setStyleSheet(styleButton);
    m_NextSongButton->setStyleSheet(styleButton);
    m_PlayButton->setStyleSheet(styleButton);
    m_SettingButton->setStyleSheet(styleButton);
    m_mainWidget->setStyleSheet("#mainWidget{background-color:rgba(0,0,0,200)}");

    QHBoxLayout *mhbox = new QHBoxLayout(m_mainWidget);
    mhbox->setContentsMargins(0,0,0,0);
    mhbox->setSpacing(0);
    m_songNameLabel = new QLabel(this);
    mhbox->addWidget(m_songNameLabel);
    styleButton = "QLabel{ background:rgba(255,255,255,50); color:white; \
                   font-weight:bold; }";
    m_songNameLabel->setStyleSheet(styleButton);

    m_toolWidget = new QWidget(this);
    vbox->addWidget(m_toolWidget);
    QHBoxLayout *hbox = new QHBoxLayout(m_toolWidget);
    hbox->setContentsMargins(0,0,0,0);
    hbox->setSpacing(0);
    hbox->addWidget(m_showMainWindow);
    hbox->addWidget(m_PreSongButton);
    hbox->addWidget(m_PlayButton);
    hbox->addWidget(m_NextSongButton);
    hbox->addWidget(m_volumnWidget);
    hbox->addWidget(m_SettingButton);
}

MusicRemoteWidgetForRectangle::~MusicRemoteWidgetForRectangle()
{
    delete m_songNameLabel;
}

void MusicRemoteWidgetForRectangle::setLabelText(const QString& value)
{
    m_songNameLabel->setText(QFontMetrics(font()).elidedText(value,
                             Qt::ElideRight, 190));
}
