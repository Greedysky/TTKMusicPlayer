#include "musiclrcfloatphotowidget.h"
#include "musicbackgroundmanager.h"
#include "musicinlinefloatuiobject.h"
#include "musicleftareawidget.h"

#include <qmath.h>
#include <QMenu>
#include <QTimer>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>

#define PHOTO_WIDTH     110
#define PHOTO_HEIGHT    65
#define PHOTO_PERLINE   3

MusicLrcFloatPhotoItem::MusicLrcFloatPhotoItem(int index, QWidget *parent)
    : MusicClickedLabel(parent)
{
    resize(PHOTO_WIDTH, PHOTO_HEIGHT);

    m_index = index;
    m_checkBox = new QCheckBox(this);
    m_checkBox->setGeometry(90, 45, 20, 20);
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_checkBox->setFocusPolicy(Qt::NoFocus);
#endif

    connect(this, SIGNAL(clicked()), SLOT(sendUserSelectArt()));
    connect(m_checkBox, SIGNAL(clicked()), SLOT(sendUserBoxClicked()));
}

MusicLrcFloatPhotoItem::~MusicLrcFloatPhotoItem()
{
    delete m_checkBox;
}

QString MusicLrcFloatPhotoItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcFloatPhotoItem::setPhoto(const QString &path)
{
    m_pixPath = path;

    QPixmap pix;
    if(m_pixPath.isEmpty())
    {
        pix.fill(Qt::black);
    }
    else
    {
        pix.load(m_pixPath);
        pix = pix.scaled(size());
    }
    setPixmap(pix);
}

void MusicLrcFloatPhotoItem::setBoxChecked(bool check)
{
    m_checkBox->setChecked(check);
}

bool MusicLrcFloatPhotoItem::boxChecked()
{
    return m_checkBox->isChecked();
}

void MusicLrcFloatPhotoItem::setBoxVisible(bool v)
{
    m_checkBox->setVisible(v);
}

void MusicLrcFloatPhotoItem::sendUserBoxClicked()
{
    emit boxClicked(m_index);
}

void MusicLrcFloatPhotoItem::sendUserSelectArt()
{
    if(!pixmap()->isNull())
    {
        emit itemClicked(m_index);
    }
}

void MusicLrcFloatPhotoItem::exportArtPixmap()
{
    QString filename = QFileDialog::getSaveFileName( this,
        tr("choose a filename to save under"), QDir::currentPath(), "Jpeg(*.jpg)");
    if(!filename.isEmpty())
    {
        QPixmap pix(m_pixPath);
        pix.save(filename, "jpg");
    }
}

void MusicLrcFloatPhotoItem::contextMenuEvent(QContextMenuEvent *event)
{
    MusicClickedLabel::contextMenuEvent(event);
    if(!pixmap()->isNull())
    {
        QMenu menu(this);
        menu.setStyleSheet(MusicUIObject::MMenuStyle02);
        menu.addAction(tr("Export"), this, SLOT(exportArtPixmap()));
        menu.exec(QCursor::pos());
    }
}


MusicLrcFloatPhotoWidget::MusicLrcFloatPhotoWidget(QWidget *parent)
    : MusicFloatAbstractWidget(parent)
{
    setObjectName("MusicLrcFloatPhotoWidget");
    setStyleSheet(QString("#MusicLrcFloatPhotoWidget{%1}").arg(MusicUIObject::MBackgroundStyle08));

    m_filmBGWidget = new QWidget(this);
    m_filmBGWidget->setObjectName("FilmBGWidget");
    m_filmBGWidget->setGeometry(0, 0, 662, 125);
    m_filmBGWidget->setStyleSheet("#FilmBGWidget{background-image:url(':/lrc/lb_film_bg');}");

    MusicLrcFloatPhotoItem *item;
    item = new MusicLrcFloatPhotoItem(0, m_filmBGWidget);
    item->move(100, 30);
    m_planes << item;
    item = new MusicLrcFloatPhotoItem(1, m_filmBGWidget);
    item->move(280, 30);
    m_planes << item;
    item = new MusicLrcFloatPhotoItem(2, m_filmBGWidget);
    item->move(460, 30);
    m_planes << item;

    m_checkBox = new QCheckBox(tr("All"), this);
    m_checkBox->setGeometry(20, 130, 100, 20);
    m_checkBox->setChecked(true);
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_confirmButton = new QPushButton(tr("Confirm"), this);
    m_confirmButton->setGeometry(580, 130, 60, 22);
    m_confirmButton->setStyleSheet(MusicUIObject::MKGInlineFloatSetting + \
                                   MusicUIObject::MPushButtonStyle08);
    m_confirmButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous = new QPushButton("<", m_filmBGWidget);
    m_previous->setGeometry(20, 38, 15, 50);
    m_next = new QPushButton(">", m_filmBGWidget);
    m_next->setGeometry(625, 38, 15, 50);
    m_previous->setCursor(QCursor(Qt::PointingHandCursor));
    m_next->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_checkBox->setFocusPolicy(Qt::NoFocus);
    m_confirmButton->setFocusPolicy(Qt::NoFocus);
    m_previous->setFocusPolicy(Qt::NoFocus);
    m_next->setFocusPolicy(Qt::NoFocus);
#endif

    m_previous->setStyleSheet(MusicUIObject::MBackgroundStyle10 + MusicUIObject::MBorderStyle01);
    m_next->setStyleSheet(MusicUIObject::MBackgroundStyle10 + MusicUIObject::MBorderStyle01);

    m_currentIndex = 0;

    connect(m_confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_previous, SIGNAL(clicked()), SLOT(photoPrevious()));
    connect(m_next, SIGNAL(clicked()), SLOT(photoNext()));
    foreach(MusicLrcFloatPhotoItem *item, m_planes)
    {
        connect(item, SIGNAL(itemClicked(int)), SLOT(sendUserSelectArtBg(int)));
        connect(item, SIGNAL(boxClicked(int)), SLOT(userSelectCheckBoxChecked(int)));
    }
    connect(M_BACKGROUND_PTR, SIGNAL(artHasChanged()), SLOT(artHasChanged()));
    connect(m_checkBox, SIGNAL(clicked(bool)), SLOT(selectAllStateChanged(bool)));
}

MusicLrcFloatPhotoWidget::~MusicLrcFloatPhotoWidget()
{
    while(!m_planes.isEmpty())
    {
        delete m_planes.takeLast();
    }
    delete m_previous;
    delete m_next;
    delete m_confirmButton;
    delete m_checkBox;
    delete m_filmBGWidget;
}

QString MusicLrcFloatPhotoWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcFloatPhotoWidget::resizeWindow(int width, int height)
{
    bool f = MusicLeftAreaWidget::instance()->isLrcWidgetShowFullScreen();
    m_rectIn = QRect(0, (f ? 680 : 530) + height, 115 + width, 105);
    m_rectOut = QRect(0, (f ? 480 : 330) + height, 662 + width, 180);

    m_filmBGWidget->move(width/2, 0);
    m_checkBox->move(width/2 + 20, 130);
    m_confirmButton->move(width/2 + 580, 130);

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
    M_BACKGROUND_PTR->setArtPhotoPathList(list);
    close();
}

void MusicLrcFloatPhotoWidget::showPhoto() const
{
    m_previous->setEnabled(m_currentIndex != 0);
    int page = ceil(m_artPath.count() *1.0 / PHOTO_PERLINE) - 1;
    m_next->setEnabled(m_currentIndex != (page = page < 0 ? 0 : page) );

    int indexCheck = m_currentIndex * PHOTO_PERLINE;
    for(int i=0; i<m_planes.count(); ++i)
    {
        m_planes[i]->setPhoto( (indexCheck + i) < m_artPath.count() ? m_artPath[indexCheck + i] : QString() );
        //check show radio button
        m_planes[i]->setBoxChecked( m_selectNum.contains(indexCheck + i) );
        m_planes[i]->setBoxVisible( (indexCheck + i) < m_artPath.count() );
    }
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
    m_artPath = M_BACKGROUND_PTR->getArtPhotoPathList();
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

void MusicLrcFloatPhotoWidget::sendUserSelectArtBg(int index)
{
    M_BACKGROUND_PTR->setUserSelectArtIndex(m_currentIndex * PHOTO_PERLINE + index);
}

void MusicLrcFloatPhotoWidget::userSelectCheckBoxChecked(int index)
{
    bool status = true;
    if(-1 < index && index < m_planes.count())
    {
        status = m_planes[index]->boxChecked();
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
        foreach(MusicLrcFloatPhotoItem *item, m_planes)
        {
            item->setBoxChecked(false);
        }
        m_selectNum.clear();
    }
}
