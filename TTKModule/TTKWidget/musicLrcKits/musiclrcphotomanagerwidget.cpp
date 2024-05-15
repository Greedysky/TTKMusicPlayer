#include "musiclrcphotomanagerwidget.h"
#include "ui_musiclrcphotomanagerwidget.h"
#include "musicbackgroundmanager.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"
#include "ttktime.h"

MusicLrcPhotoItem::MusicLrcPhotoItem(QWidget *parent)
    : QLabel(parent),
      m_isSelected(false)
{
    setFixedSize(137, 100);
    setCursor(Qt::PointingHandCursor);
}

void MusicLrcPhotoItem::updatePixmap(const QString &path)
{
    m_path = path;
    setPixmap(QPixmap(path).scaled(size()));
}

void MusicLrcPhotoItem::setSelected(bool v)
{
    m_isSelected = v;
}

void MusicLrcPhotoItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isSelected = !m_isSelected;
    update();
}

void MusicLrcPhotoItem::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if(m_isSelected)
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 0, 0, 155));
        painter.drawPixmap(width() - 16 - 4, height() - 16 - 4, 16, 16, QPixmap(":/lrc/lb_photo_checked"));
    }
}


MusicLrcPhotoWidget::MusicLrcPhotoWidget(QWidget *parent)
    : QWidget(parent)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_gridLayout->setContentsMargins(7, 0, 7, 0);
    setLayout(m_gridLayout);

    initialize();
}

MusicLrcPhotoWidget::~MusicLrcPhotoWidget()
{
    qDeleteAll(m_items);
    delete m_gridLayout;
}

void MusicLrcPhotoWidget::addCellItem(const QString &path)
{
    MusicLrcPhotoItem *item = new MusicLrcPhotoItem(this);
    item->updatePixmap(path);

    m_gridLayout->addWidget(item, m_items.count() / MIN_ITEM_COUNT, m_items.count() % MIN_ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void MusicLrcPhotoWidget::addButtonClicked()
{
    if(m_items.count() >= MAX_IMAGE_COUNT)
    {
        MusicToastLabel::popup(tr("Exceeded the maximum number limit"));
        return;
    }

    const QStringList &files = TTK::File::getOpenFileNames(this);
    if(files.isEmpty())
    {
        return;
    }

    int offset = MAX_IMAGE_COUNT - m_items.count();
    if(files.count() < offset)
    {
        offset = files.count();
    }

    for(int i = 0; i < offset; ++i)
    {
        const QString &name = G_BACKGROUND_PTR->artistName();
        const QString &path = BACKGROUND_DIR_FULL + name + QString::number(m_items.count()) + SKN_FILE;
        QFile::copy(files[i], path);

        addCellItem(path);
    }

    G_BACKGROUND_PTR->updateArtistImageList();
}

void MusicLrcPhotoWidget::deleteButtonClicked()
{
    if(!isValid())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    for(int i = 0; i < m_items.count(); ++i)
    {
        MusicLrcPhotoItem *item = m_items[i];
        if(item->isSelected())
        {
            QFile::remove(item->path());
            m_gridLayout->removeWidget(item);
            m_items.takeAt(i--)->deleteLater();
        }
    }

    for(int i = 0; i < m_items.count(); ++i)
    {
        MusicLrcPhotoItem *item = m_items[i];
        const QString &before = item->path();
        const QString &after = item->pathRef().replace(before.length() - 4 - 1, 1, QString::number(i));
        QFile::rename(before, after);
        m_gridLayout->addWidget(item, i / MIN_ITEM_COUNT, i % MIN_ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    }

    G_BACKGROUND_PTR->updateArtistImageList();
    MusicToastLabel::popup(tr("Delete current file success"));
}

void MusicLrcPhotoWidget::exportButtonClicked()
{
    if(!isValid())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    const QString &path = TTK::File::getExistingDirectory(this);
    if(path.isEmpty())
    {
        return;
    }

    const QString &dir = path + "/images/";
    QDir().mkpath(dir);

    for(MusicLrcPhotoItem *item : qAsConst(m_items))
    {
        if(item->isSelected())
        {
            QFile::copy(item->path(), dir + QString::number(TTKDateTime::currentTimestamp()) + JPG_FILE);
            TTK::Core::sleep(TTK_DN_MS);
        }
    }

    MusicToastLabel::popup(tr("Export current file success"));
}

void MusicLrcPhotoWidget::initialize()
{
    const QDir dir(BACKGROUND_DIR_FULL);
    const QString &name = G_BACKGROUND_PTR->artistName();

    for(const QFileInfo &fin : dir.entryInfoList())
    {
        const QString &v = fin.fileName();
        if(v.length() > name.length() && v.startsWith(name) && v[name.length() + 1] == '.')
        {
            addCellItem(dir.filePath(v));
        }
    }
}

bool MusicLrcPhotoWidget::isValid() const
{
    for(MusicLrcPhotoItem *item : qAsConst(m_items))
    {
        if(item->isSelected())
        {
            return true;
        }
    }
    return false;
}


MusicLrcPhotoManagerWidget::MusicLrcPhotoManagerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcPhotoManagerWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->addButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->exportButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->okButton->setStyleSheet(TTK::UI::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->addButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deleteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->exportButton->setFocusPolicy(Qt::NoFocus);
    m_ui->okButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_ui->artTextLabel->setText(G_BACKGROUND_PTR->artistName());
    m_ui->artTextLabel->setStyleSheet(TTK::UI::ColorStyle07);

    m_widget = new MusicLrcPhotoWidget(this);
    TTK::Widget::generateVScrollAreaFormat(m_ui->viewArea, m_widget);

    connect(m_ui->addButton, SIGNAL(clicked()), m_widget, SLOT(addButtonClicked()));
    connect(m_ui->deleteButton, SIGNAL(clicked()), m_widget, SLOT(deleteButtonClicked()));
    connect(m_ui->exportButton, SIGNAL(clicked()), m_widget, SLOT(exportButtonClicked()));
    connect(m_ui->okButton, SIGNAL(clicked()), SLOT(close()));
}

MusicLrcPhotoManagerWidget::~MusicLrcPhotoManagerWidget()
{
    delete m_ui;
    delete m_widget;
}
