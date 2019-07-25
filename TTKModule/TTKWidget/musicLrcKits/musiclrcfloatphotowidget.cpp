#include "musiclrcfloatphotowidget.h"
#include "musicbackgroundmanager.h"
#include "musicinlinefloatuiobject.h"
#include "musicfileutils.h"
#include "musicwidgetheaders.h"

#include <qmath.h>
#include <QTimer>
#include <QPainter>

#define PHOTO_WIDTH              110
#define PHOTO_HEIGHT             65
#define PHOTO_PERLINE            3
#define PHOTO_BACKGROUNDG_WIDTH  44

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
    const QString &filename = MusicUtils::File::getSaveFileDialog(this, "Jpeg(*.jpg)");
    if(!filename.isEmpty())
    {
        QPixmap pix(m_pixPath);
        pix.save(filename, JPG_FILE_PREFIX);
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

void MusicLrcFloatPhotoItem::enterEvent(QEvent *event)
{
    MusicClickedLabel::enterEvent(event);
    if(!pixmap()->isNull())
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        unsetCursor();
    }
}



MusicLrcFloatPhotoWidget::MusicLrcFloatPhotoWidget(QWidget *parent)
    : MusicFloatAbstractWidget(parent)
{
    setObjectName("MusicLrcFloatPhotoWidget");
    setStyleSheet(QString("#MusicLrcFloatPhotoWidget{%1}").arg(MusicUIObject::MBackgroundStyle08));

    m_filmBackgroundWidget = new QWidget(this);
    m_filmBackgroundWidget->setGeometry(0, 0, 680, 125);

    MusicLrcFloatPhotoItem *item;
    item = new MusicLrcFloatPhotoItem(0, m_filmBackgroundWidget);
    item->move(109, 30);
    m_planes << item;
    item = new MusicLrcFloatPhotoItem(1, m_filmBackgroundWidget);
    item->move(289, 30);
    m_planes << item;
    item = new MusicLrcFloatPhotoItem(2, m_filmBackgroundWidget);
    item->move(469, 30);
    m_planes << item;

    m_checkBox = new QCheckBox(tr("All"), this);
    m_checkBox->setGeometry(29, 130, 100, 20);
    m_checkBox->setChecked(true);
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_confirmButton = new QPushButton(tr("Confirm"), this);
    m_confirmButton->setGeometry(589, 130, 60, 22);
    m_confirmButton->setStyleSheet(MusicUIObject::MKGInlineFloatSetting + MusicUIObject::MPushButtonStyle08);
    m_confirmButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous = new QPushButton("<", m_filmBackgroundWidget);
    m_previous->setGeometry(29, 38, 15, 50);
    m_next = new QPushButton(">", m_filmBackgroundWidget);
    m_next->setGeometry(634, 38, 15, 50);
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
        connect(item, SIGNAL(itemClicked(int)), SLOT(sendUserSelectArtBackground(int)));
        connect(item, SIGNAL(boxClicked(int)), SLOT(userSelectCheckBoxChecked(int)));
    }
    connect(M_BACKGROUND_PTR, SIGNAL(artistNameChanged()), SLOT(artistNameChanged()));
    connect(m_checkBox, SIGNAL(clicked(bool)), SLOT(selectAllStateChanged(bool)));
}

MusicLrcFloatPhotoWidget::~MusicLrcFloatPhotoWidget()
{
    qDeleteAll(m_planes);
    delete m_previous;
    delete m_next;
    delete m_confirmButton;
    delete m_checkBox;
    delete m_filmBackgroundWidget;
}

void MusicLrcFloatPhotoWidget::resizeWindow(int width, int height)
{
    m_rectIn = QRect(0, 555 + height, 133 + width, 105);
    m_rectOut = QRect(0, 355 + height, 680 + width, 180);

    m_filmBackgroundWidget->move(width/2, 0);
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
    M_BACKGROUND_PTR->setArtistPhotoPathList(list);
    close();
}

void MusicLrcFloatPhotoWidget::showPhoto() const
{
    m_previous->setEnabled(m_currentIndex != 0);
    int page = ceil(m_artPath.count() *1.0 / PHOTO_PERLINE) - 1;
    if(page < 0)
    {
        page = 0;
    }

    m_next->setEnabled(m_currentIndex != page);

    const int indexCheck = m_currentIndex * PHOTO_PERLINE;
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

void MusicLrcFloatPhotoWidget::artistNameChanged()
{
    if(isVisible())
    {
        m_currentIndex = 0;
        close();
    }

    m_selectNum.clear();
    m_artPath = M_BACKGROUND_PTR->getArtistPhotoPathList();
    for(int i=0; i<m_artPath.count(); ++i)
    {
        m_selectNum << i;
    }
}

void MusicLrcFloatPhotoWidget::photoNext()
{
    int page = ceil(m_artPath.count() *1.0 / PHOTO_PERLINE) - 1;
    if(page < 0)
    {
        page = 0;
    }

    if(++m_currentIndex > page)
    {
        m_currentIndex = page;
    }
    showPhoto();
}

void MusicLrcFloatPhotoWidget::sendUserSelectArtBackground(int index)
{
    M_BACKGROUND_PTR->setUserSelectArtistIndex(m_currentIndex * PHOTO_PERLINE + index);
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

void MusicLrcFloatPhotoWidget::paintEvent(QPaintEvent *event)
{
    MusicFloatAbstractWidget::paintEvent(event);

    QPainter painter(this);
    for(int i=0; i<= ceil(width()/PHOTO_BACKGROUNDG_WIDTH); ++i)
    {
        painter.drawPixmap(PHOTO_BACKGROUNDG_WIDTH*i, 0, QPixmap(":/lrc/lb_film_bg"));
    }
}
