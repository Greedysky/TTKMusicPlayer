#include "musiclrcfloatphotowidget.h"
#include "musicbgthememanager.h"
#include <QMouseEvent>
#include <QCheckBox>
#include <QPushButton>
#include <QTimer>
#include <QButtonGroup>

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
    : MusicLrcFloatAbstractWidget(parent)
{
    m_rectIn = QRect(0, 500, 115, 105);
    m_rectOut = QRect(0, 300, 640, 180);

    setGeometry(m_rectOut);
    setObjectName("MusicLrcFloatPhotoWidget");
    setStyleSheet("#MusicLrcFloatPhotoWidget{" + MusicUIObject::MCustomStyle03 + "}");

    m_filmBGWidget = new QWidget(this);
    m_filmBGWidget->setGeometry(0, 0, parent->width(), 125);
    m_filmBGWidget->setStyleSheet(MusicUIObject::MCustomStyle21);

    m_plane1 = new MusicLrcFloatPhotoPlaneWidget(this);
    m_plane1->setGeometry(65, 30, PHOTO_WIDTH, PHOTO_HEIGHT);
    m_plane2 = new MusicLrcFloatPhotoPlaneWidget(this);
    m_plane2->setGeometry(210, 30, PHOTO_WIDTH, PHOTO_HEIGHT);
    m_plane3 = new MusicLrcFloatPhotoPlaneWidget(this);
    m_plane3->setGeometry(355, 30, PHOTO_WIDTH, PHOTO_HEIGHT);

    m_radio1 = new QCheckBox(this);
    m_radio1->setGeometry(155, 75, 20, 20);
    m_radio1->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_radio2 = new QCheckBox(this);
    m_radio2->setGeometry(300, 75, 20, 20);
    m_radio2->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_radio3 = new QCheckBox(this);
    m_radio3->setGeometry(445, 75, 20, 20);
    m_radio3->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_checkBox = new QCheckBox(tr("All"),this);
    m_checkBox->setGeometry(5, 130, 100, 20);
    m_checkBox->setChecked(true);
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_confirmButton = new QPushButton(tr("Confirm"),this);
    m_confirmButton->setGeometry(340, 130, 70, 20);
    m_confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    m_confirmButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_cancelButton = new QPushButton(tr("Close"), this);
    m_cancelButton->setGeometry(420, 130, 70, 20);
    m_cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    m_cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous = new QPushButton("<", this);
    m_previous->setGeometry(10, 38, 15, 50);
    m_next = new QPushButton(">", this);
    m_next->setGeometry(500, 38, 15, 50);
    m_previous->setCursor(QCursor(Qt::PointingHandCursor));
    m_next->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous->setStyleSheet( MusicUIObject::MCustomStyle22 );
    m_next->setStyleSheet( MusicUIObject::MCustomStyle22 );

    m_currentIndex = 0;

    connect(m_cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_previous, SIGNAL(clicked()), SLOT(photoPrevious()));
    connect(m_next, SIGNAL(clicked()), SLOT(photoNext()));
    connect(m_cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_plane1, SIGNAL(clicked()), SLOT(sendUserSelectArtBg1()));
    connect(m_plane2, SIGNAL(clicked()), SLOT(sendUserSelectArtBg2()));
    connect(m_plane3, SIGNAL(clicked()), SLOT(sendUserSelectArtBg3()));

    QButtonGroup *radioGroup = new QButtonGroup(this);
    radioGroup->setExclusive(false);
    radioGroup->addButton(m_radio1, 0);
    radioGroup->addButton(m_radio2, 1);
    radioGroup->addButton(m_radio3, 2);
    connect(radioGroup, SIGNAL(buttonClicked(int)), SLOT(userSelectCheckBoxChecked(int)));
    M_BG_MANAGER->setObject(this);

    m_checkBox->setEnabled(false);
    m_cancelButton->setEnabled(false);
}

MusicLrcFloatPhotoWidget::~MusicLrcFloatPhotoWidget()
{
    delete m_radio1, m_radio2, m_radio3;
    delete m_plane1, m_plane2, m_plane3;
    delete m_filmBGWidget;
    delete m_previous, m_next;
    delete m_confirmButton, m_cancelButton;
    delete m_checkBox;
}

void MusicLrcFloatPhotoWidget::show()
{
    QWidget::show();
    animationOut();
    showPhoto();
}

void MusicLrcFloatPhotoWidget::close()
{
    animationIn();
    QTimer::singleShot(m_animation->duration(), this, SLOT(parentClose()));
}

void MusicLrcFloatPhotoWidget::confirmButtonClicked()
{
    QStringList list;
    foreach(int i, m_selectNum)
    {
       list <<  m_artPath[i];
    }
    M_BG_MANAGER->setArtPhotoPaths(list);
    close();
}

void MusicLrcFloatPhotoWidget::showPhoto() const
{
    m_previous->setEnabled(m_currentIndex != 0);
    int page = ceil(m_artPath.count() *1.0 / PHOTO_PERLINE) - 1;
    m_next->setEnabled(m_currentIndex != (page = page < 0 ? 0 : page) );

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
    m_artPath = M_BG_MANAGER->getArtPhotoPaths();
    for(int i=0; i<m_artPath.count(); ++i)
    {
        m_selectNum << i;
    }
}

void MusicLrcFloatPhotoWidget::photoNext()
{
    int page = ceil(m_artPath.count() *1.0 / PHOTO_PERLINE) - 1;
    if( ++m_currentIndex > (page = page < 0 ? 0 : page) )
    {
        m_currentIndex = page;
    }
    showPhoto();
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg1()
{
    M_BG_MANAGER->sendUserSelectArtBg(m_currentIndex * PHOTO_PERLINE + 0);
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg2()
{
    M_BG_MANAGER->sendUserSelectArtBg(m_currentIndex * PHOTO_PERLINE + 1);
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg3()
{
    M_BG_MANAGER->sendUserSelectArtBg(m_currentIndex * PHOTO_PERLINE + 2);
}

void MusicLrcFloatPhotoWidget::userSelectCheckBoxChecked(int index)
{
    bool status = true;
    switch(index)
    {
        case 0: status = m_radio1->isChecked(); break;
        case 1: status = m_radio2->isChecked(); break;
        case 2: status = m_radio3->isChecked(); break;
        default: break;
    }
    index = m_currentIndex * PHOTO_PERLINE + index;
    if(status)
    {
        m_selectNum << index;
    }
    else
    {
        m_selectNum.remove(index);
    }
}
