#include "musiclrcfloatphotowidget.h"
#include "musicbackgroundmanager.h"
#include "musicinteriorfloatuiobject.h"
#include "musicwidgetheaders.h"
#include "musicimageutils.h"
#include "musicfileutils.h"
#include "musicwidgetutils.h"

#include <qmath.h>
#include <QTimer>

#define PHOTO_PERLINE       4

MusicLrcFloatPhotoItem::MusicLrcFloatPhotoItem(int index, QWidget *parent)
    : TTKClickedLabel(parent),
      m_index(index)
{
    setFixedSize(120, 65);

    m_checkBox = new QCheckBox(this);
    m_checkBox->setGeometry(100, 45, 16, 16);
    m_checkBox->setStyleSheet(MusicUIObject::InteriorFloatPhotoItem);
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
    if(!m_pixPath.isEmpty())
    {
        pix.load(m_pixPath);
        pix = MusicUtils::Image::roundedPixmap(pix, size(), 5, 5);
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
    Q_EMIT boxClicked(m_index);
}

void MusicLrcFloatPhotoItem::sendUserSelectArt()
{
    const QPixmap &pixmap = QtLablePixmap(this);
    if(!pixmap.isNull())
    {
        Q_EMIT itemClicked(m_index);
    }
}

void MusicLrcFloatPhotoItem::exportArtPixmap()
{
    const QString &path = MusicUtils::File::getSaveFileName(this, "Jpeg(*.jpg)");
    if(!path.isEmpty())
    {
        const QPixmap pix(m_pixPath);
        pix.save(path, JPG_FILE_SUFFIX);
    }
}

void MusicLrcFloatPhotoItem::enterEvent(QtEnterEvent *event)
{
    TTKClickedLabel::enterEvent(event);
    const QPixmap &pixmap = QtLablePixmap(this);
    if(!pixmap.isNull())
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        unsetCursor();
    }
}

void MusicLrcFloatPhotoItem::contextMenuEvent(QContextMenuEvent *event)
{
    TTKClickedLabel::contextMenuEvent(event);
    const QPixmap &pixmap = QtLablePixmap(this);
    if(!pixmap.isNull())
    {
        QMenu menu(this);
        menu.setStyleSheet(MusicUIObject::MenuStyle02);
        menu.addAction(tr("Export"), this, SLOT(exportArtPixmap()));
        menu.exec(QCursor::pos());
    }
}



MusicLrcFloatPhotoWidget::MusicLrcFloatPhotoWidget(QWidget *parent)
    : MusicAbstractFloatWidget(parent),
      m_currentIndex(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    m_artistLabel = new QLabel(this);
    m_artistLabel->setAlignment(Qt::AlignCenter);
    m_artistLabel->setStyleSheet(MusicUIObject::ColorStyle12 + MusicUIObject::BorderStyle07 + MusicUIObject::BackgroundStyle06);
    layout->addWidget(m_artistLabel);

    QWidget *areaWidget = new QWidget(this);
    areaWidget->setObjectName("AreaWidget");
    areaWidget->setStyleSheet(QString("#AreaWidget{%1}").arg(MusicUIObject::BackgroundStyle07));
    QHBoxLayout *areaLayout = new QHBoxLayout(areaWidget);
    areaLayout->setContentsMargins(9, 18, 9, 9);
    areaWidget->setLayout(areaLayout);
    layout->addWidget(areaWidget);

    m_previous = new QPushButton(this);
    m_previous->setCursor(QCursor(Qt::PointingHandCursor));
    m_previous->setStyleSheet(MusicUIObject::InteriorFloatPhotoPrevious);
    m_previous->setFixedSize(20, 52);
    areaLayout->addWidget(m_previous);

    for(int i = 0; i < PHOTO_PERLINE; ++i)
    {
        MusicLrcFloatPhotoItem *item = new MusicLrcFloatPhotoItem(i, this);
        areaLayout->addWidget(item);
        m_planes << item;

        connect(item, SIGNAL(itemClicked(int)), SLOT(sendUserSelectArtBackground(int)));
        connect(item, SIGNAL(boxClicked(int)), SLOT(userSelectCheckBoxChecked(int)));
    }

    m_next = new QPushButton(this);
    m_next->setCursor(QCursor(Qt::PointingHandCursor));
    m_next->setStyleSheet(MusicUIObject::InteriorFloatPhotoNext);
    m_next->setFixedSize(20, 52);
    areaLayout->addWidget(m_next);

    QWidget *functionWidget = new QWidget(this);
    functionWidget->setObjectName("FunctionWidget");
    functionWidget->setStyleSheet(QString("#FunctionWidget{%1}").arg(MusicUIObject::BackgroundStyle07));
    QHBoxLayout *functionLayout = new QHBoxLayout(functionWidget);
    functionLayout->setContentsMargins(18, 9, 18, 9);
    functionLayout->setSpacing(15);
    functionWidget->setLayout(functionLayout);
    layout->addWidget(functionWidget);

    m_checkBox = new QCheckBox(tr("All"), functionWidget);
    m_checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_checkBox->setStyleSheet(MusicUIObject::CheckBoxStyle05);
    m_checkBox->setFixedSize(50, 20);
    m_checkBox->setChecked(true);
    functionLayout->addWidget(m_checkBox);
    functionLayout->addStretch(1);

    QPushButton *confirmButton = new QPushButton(tr("OK"), functionWidget);
    confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    confirmButton->setStyleSheet(MusicUIObject::InteriorFloatSetting + MusicUIObject::PushButtonStyle06);
    confirmButton->setFixedSize(50, 25);
    functionLayout->addWidget(confirmButton);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"), functionWidget);
    cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    cancelButton->setStyleSheet(MusicUIObject::InteriorFloatSetting + MusicUIObject::PushButtonStyle06);
    cancelButton->setFixedSize(50, 25);
    functionLayout->addWidget(cancelButton);

#ifdef Q_OS_UNIX
    m_previous->setFocusPolicy(Qt::NoFocus);
    m_next->setFocusPolicy(Qt::NoFocus);
    m_checkBox->setFocusPolicy(Qt::NoFocus);
    confirmButton->setFocusPolicy(Qt::NoFocus);
    cancelButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_previous, SIGNAL(clicked()), SLOT(photoPrevious()));
    connect(m_next, SIGNAL(clicked()), SLOT(photoNext()));
    connect(m_checkBox, SIGNAL(clicked(bool)), SLOT(selectAllStateChanged(bool)));
    connect(confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(G_BACKGROUND_PTR, SIGNAL(artistNameChanged()), SLOT(artistNameChanged()));
}

MusicLrcFloatPhotoWidget::~MusicLrcFloatPhotoWidget()
{
    qDeleteAll(m_planes);
    delete m_previous;
    delete m_next;
    delete m_checkBox;
}

void MusicLrcFloatPhotoWidget::resizeGeometry(int width, int height)
{
    m_rectEnter = QRect(0, 525 + height, 680 + width, 185);
    m_rectLeave = QRect(0, 325 + height, 680 + width, 185);

    setGeometry(m_rectLeave);
}

void MusicLrcFloatPhotoWidget::show()
{
    QWidget::show();
    animationLeave();
    showPhoto();
}

void MusicLrcFloatPhotoWidget::close()
{
    animationEnter();
    QTimer::singleShot(m_animation->duration(), this, SLOT(parentClose()));
}

void MusicLrcFloatPhotoWidget::confirmButtonClicked()
{
    QStringList list;
    for(int i : qAsConst(m_selectNum))
    {
       list << m_photos[i];
    }

    G_BACKGROUND_PTR->setArtistPhotoPathList(list);
    close();
}

void MusicLrcFloatPhotoWidget::showPhoto() const
{
    m_previous->setEnabled(m_currentIndex != 0);
    int page = ceil(m_photos.count() * 1.0 / PHOTO_PERLINE) - 1;
    if(page < 0)
    {
        page = 0;
    }

    m_next->setEnabled(m_currentIndex != page);

    const int indexCheck = m_currentIndex * PHOTO_PERLINE;
    for(int i = 0; i < m_planes.count(); ++i)
    {
        m_planes[i]->setPhoto((indexCheck + i) < m_photos.count() ? m_photos[indexCheck + i] : QString());
        //check show radio button
        m_planes[i]->setBoxChecked(m_selectNum.contains(indexCheck + i));
        m_planes[i]->setBoxVisible((indexCheck + i) < m_photos.count());
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
    m_checkBox->setChecked(true);
    m_photos = G_BACKGROUND_PTR->artistPhotoPathList();

    for(int i = 0; i < m_photos.count(); ++i)
    {
        m_selectNum << i;
    }

    const QString &name = G_BACKGROUND_PTR->artistName();
    m_artistLabel->setText(name);
    m_artistLabel->setFixedSize(MusicUtils::Widget::fontTextWidth(m_artistLabel->font(), name) + 50, 35);
}

void MusicLrcFloatPhotoWidget::photoNext()
{
    int page = ceil(m_photos.count() * 1.0 / PHOTO_PERLINE) - 1;
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
    G_BACKGROUND_PTR->setUserSelectArtistIndex(m_currentIndex * PHOTO_PERLINE + index);
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
        for(int i = 0; i < m_photos.count(); ++i)
        {
            m_selectNum << i;
        }
        showPhoto();
    }
    else
    {
        for(MusicLrcFloatPhotoItem *item : qAsConst(m_planes))
        {
            item->setBoxChecked(false);
        }
        m_selectNum.clear();
    }
}
