#include "musiclrcfloatwidget.h"
#include "musicbgthememanager.h"

#include <QPropertyAnimation>
#include <QPushButton>
#include <QCheckBox>
#include <QMouseEvent>

MusicLrcFloatPhotoPlaneWidget::MusicLrcFloatPhotoPlaneWidget(QWidget *parent)
    : QLabel(parent)
{

}

void MusicLrcFloatPhotoPlaneWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
       emit clicked();
    }
}



MusicLrcFloatPhotoWidget::MusicLrcFloatPhotoWidget(QWidget *parent)
    : QLabel(parent)
{
    setGeometry(0, parent->height() - 180, parent->width(), 180);
    setObjectName("MusicLrcFloatPhotoWidget");
    setStyleSheet("#MusicLrcFloatPhotoWidget{background:rgba(0, 0, 0, 100);}");

    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(500);

    m_filmBGWidget = new QWidget(this);
    m_filmBGWidget->setGeometry(0, 0, parent->width(), 125);
    m_filmBGWidget->setStyleSheet("background-image:url(':/lrc/film');");

    m_plane1 = new MusicLrcFloatPhotoPlaneWidget(this);
    m_plane1->setGeometry(65, 30, PHOTO_WIDTH, PHOTO_HEIGHT);
    m_plane2 = new MusicLrcFloatPhotoPlaneWidget(this);
    m_plane2->setGeometry(210, 30, PHOTO_WIDTH, PHOTO_HEIGHT);
    m_plane3 = new MusicLrcFloatPhotoPlaneWidget(this);
    m_plane3->setGeometry(355, 30, PHOTO_WIDTH, PHOTO_HEIGHT);

    m_radio1 = new QCheckBox(this);
    m_radio1->setGeometry(155, 75, 20, 20);
    m_radio1->setStyleSheet(MusicObject::MusicSettingCheckButton);
    m_radio2 = new QCheckBox(this);
    m_radio2->setGeometry(300, 75, 20, 20);
    m_radio2->setStyleSheet(MusicObject::MusicSettingCheckButton);
    m_radio3 = new QCheckBox(this);
    m_radio3->setGeometry(445, 75, 20, 20);
    m_radio3->setStyleSheet(MusicObject::MusicSettingCheckButton);

    m_checkBox = new QCheckBox(tr("All"),this);
    m_checkBox->setGeometry(5, 130, 100, 20);
    m_checkBox->setChecked(true);
    m_checkBox->setStyleSheet(MusicObject::MusicSettingCheckButton);

    m_confirmButton = new QPushButton(tr("Confirm"),this);
    m_confirmButton->setGeometry(340, 130, 70, 20);
    m_confirmButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    m_confirmButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_cancelButton = new QPushButton(tr("Close"), this);
    m_cancelButton->setGeometry(420, 130, 70, 20);
    m_cancelButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    m_cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous = new QPushButton("<", this);
    m_previous->setGeometry(10, 38, 15, 50);
    m_next = new QPushButton(">", this);
    m_next->setGeometry(500, 38, 15, 50);
    m_previous->setCursor(QCursor(Qt::PointingHandCursor));
    m_next->setCursor(QCursor(Qt::PointingHandCursor));
    const QString style = "background:rgba(255, 255, 255, 50);border:none";
    m_previous->setStyleSheet( style );
    m_next->setStyleSheet( style );

    m_currentIndex = 0;

    connect(m_cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_previous, SIGNAL(clicked()), SLOT(photoPrevious()));
    connect(m_next, SIGNAL(clicked()), SLOT(photoNext()));
    connect(m_cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_plane1, SIGNAL(clicked()), SLOT(sendUserSelectArtBg1()));
    connect(m_plane2, SIGNAL(clicked()), SLOT(sendUserSelectArtBg2()));
    connect(m_plane3, SIGNAL(clicked()), SLOT(sendUserSelectArtBg3()));
    connect(m_radio1, SIGNAL(clicked(bool)), SLOT(userSelectCheckBoxChecked1(bool)));
    connect(m_radio2, SIGNAL(clicked(bool)), SLOT(userSelectCheckBoxChecked2(bool)));
    connect(m_radio3, SIGNAL(clicked(bool)), SLOT(userSelectCheckBoxChecked3(bool)));
    M_ARTBG.setObject(this);

    m_checkBox->setEnabled(false);
    m_cancelButton->setEnabled(false);
}

MusicLrcFloatPhotoWidget::~MusicLrcFloatPhotoWidget()
{
    delete m_radio1, m_radio2, m_radio3;
    delete m_plane1, m_plane2, m_plane3;
    delete m_animation;
    delete m_filmBGWidget;
    delete m_previous, m_next;
    delete m_confirmButton, m_cancelButton;
    delete m_checkBox;
}

void MusicLrcFloatPhotoWidget::show()
{
    QWidget::show();
    m_animation->setStartValue(QRect(0, 500, 115, 105));
    m_animation->setEndValue( geometry() );
    m_animation->start();
    showPhoto();
}

void MusicLrcFloatPhotoWidget::confirmButtonClicked()
{
    QStringList list;
    foreach(int i, m_selectNum)
    {
       list <<  m_artPath[i];
    }
    M_ARTBG.setArtPhotoPaths(list);
    close();
}

void MusicLrcFloatPhotoWidget::showPhoto()
{
    m_previous->setEnabled(m_currentIndex != 0);
    m_next->setEnabled(m_currentIndex != (ceil(m_artPath.count() *1.0 / PHOTO_PERLINE) - 1));

    QPixmap nullPix;
    nullPix.fill(Qt::black);
    //check show pixmap
    int indexCheck = m_currentIndex * PHOTO_PERLINE;
    m_plane1->setPixmap( (indexCheck + 0) < m_artPath.count() ? QPixmap( m_artPath[indexCheck + 0] )
                                           .scaled(PHOTO_WIDTH, PHOTO_HEIGHT): nullPix );
    m_plane2->setPixmap( (indexCheck + 1) < m_artPath.count() ? QPixmap( m_artPath[indexCheck + 1] )
                                           .scaled(PHOTO_WIDTH, PHOTO_HEIGHT): nullPix );
    m_plane3->setPixmap( (indexCheck + 2) < m_artPath.count() ? QPixmap( m_artPath[indexCheck + 2] )
                                           .scaled(PHOTO_WIDTH, PHOTO_HEIGHT): nullPix );
    //check show radio button
    m_radio1->setChecked( m_selectNum.contains(indexCheck + 0) );
    m_radio2->setChecked( m_selectNum.contains(indexCheck + 1) );
    m_radio3->setChecked( m_selectNum.contains(indexCheck + 2) );
    m_radio1->setVisible( (indexCheck + 0) < m_artPath.count() );
    m_radio2->setVisible( (indexCheck + 1) < m_artPath.count() );
    m_radio3->setVisible( (indexCheck + 2) < m_artPath.count() );
}

void MusicLrcFloatPhotoWidget::photoPrevious()
{
    if(--m_currentIndex < 0)
    {
        m_currentIndex = 0;
    }
    showPhoto();
}

void MusicLrcFloatPhotoWidget::artHasChanged()
{
    m_selectNum.clear();
    m_artPath = M_ARTBG.getArtPhotoPaths();
    for(int i=0; i<m_artPath.count(); ++i)
    {
        m_selectNum << i;
    }
}

void MusicLrcFloatPhotoWidget::photoNext()
{
    int page = ceil(m_artPath.count() *1.0 / PHOTO_PERLINE) - 1;
    if(++m_currentIndex > page )
    {
        m_currentIndex = page;
    }
    showPhoto();
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg1()
{
    M_ARTBG.sendUserSelectArtBg(m_currentIndex * PHOTO_PERLINE + 0);
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg2()
{
    M_ARTBG.sendUserSelectArtBg(m_currentIndex * PHOTO_PERLINE + 1);
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg3()
{
    M_ARTBG.sendUserSelectArtBg(m_currentIndex * PHOTO_PERLINE + 2);
}

void MusicLrcFloatPhotoWidget::userSelectCheckBoxChecked1(bool b)
{
    int index = m_currentIndex * PHOTO_PERLINE + 0;
    if(b)
        m_selectNum << index;
    else
        m_selectNum.remove(index);
}

void MusicLrcFloatPhotoWidget::userSelectCheckBoxChecked2(bool b)
{
    int index = m_currentIndex * PHOTO_PERLINE + 1;
    if(b)
        m_selectNum << index;
    else
        m_selectNum.remove(index);
}

void MusicLrcFloatPhotoWidget::userSelectCheckBoxChecked3(bool b)
{
    int index = m_currentIndex * PHOTO_PERLINE + 2;
    if(b)
        m_selectNum << index;
    else
        m_selectNum.remove(index);
}



MusicLrcFloatWidget::MusicLrcFloatWidget(QWidget *parent)
    : QLabel(parent)
{
    m_posX = parent->width() - 235;
    m_posY = parent->height()/4;
    setStyleSheet("background:rgba(0, 0, 0, 100)");
    setGeometry( m_posX + 110, m_posY + 51, 115, 105);

    m_floatPhotoWidget = new MusicLrcFloatPhotoWidget(parent);
    m_floatPhotoWidget->hide();

    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(500);

    const QString style = "QPushButton{background:transparent;text-align:left; \
                           color:white;}QPushButton::hover{image:url(:/lrc/shadow);}";

    m_update = new QPushButton(tr(" Update"), this);
    m_search = new QPushButton(tr(" Search"), this);
    m_more = new QPushButton(tr(" More"), this);
    m_wallp = new QPushButton(tr(" Wallp"), this);
    m_photo = new QPushButton(tr(" Photo"), this);

    m_update->setIcon(QIcon(":/lrc/update"));
    m_search->setIcon(QIcon(":/lrc/search"));
    m_more->setIcon(QIcon(":/lrc/more"));
    m_wallp->setIcon(QIcon(":/lrc/wallpaper"));
    m_photo->setIcon(QIcon(":/lrc/photo"));

    m_update->setStyleSheet( style );
    m_search->setStyleSheet( style );
    m_more->setStyleSheet( style );
    m_wallp->setStyleSheet( style );
    m_photo->setStyleSheet( style );

    m_update->setGeometry(10, 10, 80, 30);
    m_search->setGeometry(10, 50, 80, 30);
    m_more->setGeometry(10, 90, 80, 30);
    m_wallp->setGeometry(10, 130, 80, 30);
    m_photo->setGeometry(10, 170, 80, 30);

    m_update->setCursor(QCursor(Qt::PointingHandCursor));
    m_search->setCursor(QCursor(Qt::PointingHandCursor));
    m_more->setCursor(QCursor(Qt::PointingHandCursor));
    m_wallp->setCursor(QCursor(Qt::PointingHandCursor));
    m_photo->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_update, SIGNAL(clicked()), parent, SIGNAL(theCurrentLrcUpdated()));
    connect(m_search, SIGNAL(clicked()), parent, SLOT(searchMusicLrcs()));
    connect(m_photo, SIGNAL(clicked()), m_floatPhotoWidget, SLOT(show()));
}

MusicLrcFloatWidget::~MusicLrcFloatWidget()
{
    delete m_more, m_update, m_search;
    delete m_wallp, m_photo;
    delete m_floatPhotoWidget;
    delete m_animation;
}

void MusicLrcFloatWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_animation->setStartValue( geometry() );
    m_animation->setEndValue(QRect(m_posX, m_posY, 115, 210));
    m_animation->start();
}

void MusicLrcFloatWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_animation->setStartValue(QRect(m_posX, m_posY, 115, 210));
    m_animation->setEndValue(QRect(m_posX + 110, m_posY + 51, 115, 105));
    m_animation->start();
}
