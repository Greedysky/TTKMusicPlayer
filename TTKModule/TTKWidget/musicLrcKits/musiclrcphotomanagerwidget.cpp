#include "musiclrcphotomanagerwidget.h"
#include "ui_musiclrcphotomanagerwidget.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"

MusicLrcPhotoItem::MusicLrcPhotoItem(QWidget *parent)
    : QLabel(parent),
      m_isSelected(false)
{
    setFixedSize(137, 100);
    setCursor(Qt::PointingHandCursor);
}

void MusicLrcPhotoItem::updatePixmap(const QString &path)
{
//    QPixmap pix(path);
//    setPixmap(image.m_pix.scaled(size()));
}

void MusicLrcPhotoItem::setSelected(bool v)
{
    m_isSelected = v;
}

void MusicLrcPhotoItem::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    m_isSelected = !m_isSelected;
    update();
}

void MusicLrcPhotoItem::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if(m_isSelected)
    {
        QPainter painter(this);
        painter.setBrush(QColor(0, 0, 0, 155));
        painter.drawRect(rect());
    }
}


MusicLrcPhotoWidget::MusicLrcPhotoWidget(QWidget *parent)
    : QWidget(parent)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_gridLayout->setContentsMargins(7, 0, 7, 0);
    setLayout(m_gridLayout);

    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
    addCellItem("a");
}

MusicLrcPhotoWidget::~MusicLrcPhotoWidget()
{
    delete m_gridLayout;
}

void MusicLrcPhotoWidget::addCellItem(const QString &path)
{
    MusicLrcPhotoItem *item = new MusicLrcPhotoItem(this);
    item->setPixmap(QPixmap(path));
//    item->setCloseEnabled(state);
//    item->setFileName(name);
//    item->setFilePath(path);
//    item->updatePixmap();

    m_gridLayout->addWidget(item, m_items.count() / MIN_ITEM_COUNT, m_items.count() % MIN_ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}


MusicLrcPhotoManagerWidget::MusicLrcPhotoManagerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcPhotoManagerWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->addButton->setStyleSheet(MusicUIObject::PushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(MusicUIObject::PushButtonStyle04);
    m_ui->exportButton->setStyleSheet(MusicUIObject::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->addButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deleteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->exportButton->setFocusPolicy(Qt::NoFocus);
#endif

    MusicLrcPhotoWidget *w = new MusicLrcPhotoWidget(this);
    MusicUtils::Widget::generateVScrollAreaFormat(m_ui->viewArea, w);
}

MusicLrcPhotoManagerWidget::~MusicLrcPhotoManagerWidget()
{
    delete m_ui;
}

void MusicLrcPhotoManagerWidget::selectButtonClicked()
{
    const QString &path = MusicUtils::File::getOpenFileName(this);
    if(path.isEmpty())
    {
        return;
    }

//    const QPixmap pix(path);
//    if(pix.width() < WINDOW_WIDTH_MIN || pix.height() < WINDOW_HEIGHT_MIN)
//    {
//        m_ui->stateLabel->show();
//        m_ui->uploadButton->hide();
//        m_ui->closeButton->hide();
//    }
//    else
//    {
//        m_ui->stateLabel->hide();
//        m_ui->uploadButton->show();
//        m_ui->closeButton->show();
//        m_ui->introduceLabel->hide();
//        m_ui->selectButton->hide();
//        m_ui->imageLabel->setImagePath(path);
//    }
}

void MusicLrcPhotoManagerWidget::uploadButtonClicked()
{
//    const QDir dir(BACKGROUND_DIR_FULL);
//    int count = 0;
//    const QString &name = m_ui->artSearchEdit->text().trimmed();
//    if(name.isEmpty())
//    {
//        MusicToastLabel::popup(tr("The art is empty!"));
//        return;
//    }

//    for(const QFileInfo &fin : dir.entryInfoList())
//    {
//        if(fin.fileName().contains(name))
//        {
//            ++count;
//        }
//    }

//    const QString &fileName = QString("%1%2%3").arg(BACKGROUND_DIR_FULL, name).arg(count);
//    m_ui->imageLabel->saveImagePath(fileName + JPG_FILE);
//    QFile::rename(fileName + JPG_FILE, fileName + SKN_FILE);
//    close();
}
