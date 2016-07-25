#include "musiclrcfloatphotowidget.h"
#include "musicbackgroundmanager.h"
#include "musicclickedlabel.h"

#include <qmath.h>
#include <QTimer>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>

MusicLrcFloatPhotoWidget::MusicLrcFloatPhotoWidget(QWidget *parent)
    : MusicFloatAbstractWidget(parent)
{
    setObjectName("MusicLrcFloatPhotoWidget");
    setStyleSheet("#MusicLrcFloatPhotoWidget{" + MusicUIObject::MCustomStyle03 + "}");

    m_filmBGWidget = new QWidget(this);
    m_filmBGWidget->setGeometry(0, 0, 662, 125);
    m_filmBGWidget->setStyleSheet(MusicUIObject::MCustomStyle21);

    m_plane1 = new MusicClickedLabel(this);
    m_plane1->setGeometry(100, 30, PHOTO_WIDTH, PHOTO_HEIGHT);
    m_plane2 = new MusicClickedLabel(this);
    m_plane2->setGeometry(280, 30, PHOTO_WIDTH, PHOTO_HEIGHT);
    m_plane3 = new MusicClickedLabel(this);
    m_plane3->setGeometry(460, 30, PHOTO_WIDTH, PHOTO_HEIGHT);

    m_radio1 = new QCheckBox(this);
    m_radio1->setGeometry(190, 75, 20, 20);
    m_radio1->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_radio2 = new QCheckBox(this);
    m_radio2->setGeometry(370, 75, 20, 20);
    m_radio2->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_radio3 = new QCheckBox(this);
    m_radio3->setGeometry(550, 75, 20, 20);
    m_radio3->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_checkBox = new QCheckBox(tr("All"), this);
    m_checkBox->setGeometry(15, 130, 100, 20);
    m_checkBox->setChecked(true);
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_confirmButton = new QPushButton(tr("Confirm"), this);
    m_confirmButton->setGeometry(500, 130, 70, 20);
    m_confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    m_confirmButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous = new QPushButton("<", this);
    m_previous->setGeometry(20, 38, 15, 50);
    m_next = new QPushButton(">", this);
    m_next->setGeometry(625, 38, 15, 50);
    m_previous->setCursor(QCursor(Qt::PointingHandCursor));
    m_next->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous->setStyleSheet( MusicUIObject::MCustomStyle22 );
    m_next->setStyleSheet( MusicUIObject::MCustomStyle22 );

    m_currentIndex = 0;

    connect(m_confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_previous, SIGNAL(clicked()), SLOT(photoPrevious()));
    connect(m_next, SIGNAL(clicked()), SLOT(photoNext()));
    connect(m_plane1, SIGNAL(clicked()), SLOT(sendUserSelectArtBg1()));
    connect(m_plane2, SIGNAL(clicked()), SLOT(sendUserSelectArtBg2()));
    connect(m_plane3, SIGNAL(clicked()), SLOT(sendUserSelectArtBg3()));

    QButtonGroup *radioGroup = new QButtonGroup(this);
    radioGroup->setExclusive(false);
    radioGroup->addButton(m_radio1, 0);
    radioGroup->addButton(m_radio2, 1);
    radioGroup->addButton(m_radio3, 2);
    connect(radioGroup, SIGNAL(buttonClicked(int)), SLOT(userSelectCheckBoxChecked(int)));
    connect(M_BACKGROUND_PTR, SIGNAL(artHasChanged()), SLOT(artHasChanged()));
    connect(m_checkBox, SIGNAL(clicked(bool)), SLOT(selectAllStateChanged(bool)));
}

MusicLrcFloatPhotoWidget::~MusicLrcFloatPhotoWidget()
{
    delete m_radio1;
    delete m_radio2;
    delete m_radio3;
    delete m_plane1;
    delete m_plane2;
    delete m_plane3;
    delete m_filmBGWidget;
    delete m_previous;
    delete m_next;
    delete m_confirmButton;
    delete m_checkBox;
}

QString MusicLrcFloatPhotoWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcFloatPhotoWidget::resizeWindow(int width, int height)
{
    m_rectIn = QRect(width/2, 530 + height, 115, 105);
    m_rectOut = QRect(width/2, 330 + height, 662, 180);
    setGeometry(m_rectOut);
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
       list << m_artPath[i];
    }
    M_BACKGROUND_PTR->setArtPhotoPaths(list);
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
    m_artPath = M_BACKGROUND_PTR->getArtPhotoPaths();
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
    M_BACKGROUND_PTR->setUserSelectArtIndex(m_currentIndex * PHOTO_PERLINE + 0);
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg2()
{
    M_BACKGROUND_PTR->setUserSelectArtIndex(m_currentIndex * PHOTO_PERLINE + 1);
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg3()
{
    M_BACKGROUND_PTR->setUserSelectArtIndex(m_currentIndex * PHOTO_PERLINE + 2);
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

    m_checkBox->setChecked(false);
}

void MusicLrcFloatPhotoWidget::selectAllStateChanged(bool state)
{
    if(state)
    {
        for(int i=0; i<m_artPath.count(); ++i)
        {
            m_selectNum << i;
        }
        showPhoto();
    }
    else
    {
        m_radio1->setChecked(false);
        m_radio2->setChecked(false);
        m_radio3->setChecked(false);
        m_selectNum.clear();
    }
}
