#include "musiclrcposterwidget.h"
#include "ui_musiclrcposterwidget.h"
#include "musicitemdelegate.h"
#include "musicbackgroundmanager.h"
#include "musicstringutils.h"
#include "musicuiobject.h"

#include <QPainter>
#include <QScrollBar>
#include <QDebug>

#define ITEM_SCROLL_WIDTH   8
#define ITEM_BORDER         3
#define ITEM_WIDTH          275
#define ITEM_HEIGHT         330

MusicLrcPosterItemWidget::MusicLrcPosterItemWidget(QWidget *parent)
    : QWidget(parent)
{
    m_type = Type_01;
    m_pixPath = M_BACKGROUND_PTR->getArtPhotoPathNoIndex();
}

QString MusicLrcPosterItemWidget::getClassName()
{
    return staticMetaObject.className();
}

bool MusicLrcPosterItemWidget::hasScroll() const
{
     return height() > ITEM_HEIGHT;
}

void MusicLrcPosterItemWidget::setImagePath(const QString &path)
{
    m_pixPath = path;
    update();
}

void MusicLrcPosterItemWidget::setCurrentSongName(const QString &name)
{
    m_title = name;
}

void MusicLrcPosterItemWidget::textChanged(const QStringList &data)
{
    m_data = data;
    update();
}

void MusicLrcPosterItemWidget::currentTypeChanged(int type)
{
    m_type = MStatic_cast(Type, type);
    update();
}

void MusicLrcPosterItemWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    switch(m_type)
    {
        case Type_01: drawTheme1(&painter); break;
        case Type_02: drawTheme2(&painter); break;
    }
}

void MusicLrcPosterItemWidget::drawTheme1(QPainter *painter)
{
    QPixmap pix(m_pixPath);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);

    QFontMetrics fm(font());
    int lineHeight = fm.height();
    int offset = pix.height() + 5*ITEM_BORDER + lineHeight;
    int v = 1;
    //////////////////////////////////////////////////////////////////////////////
    painter->setPen(QColor(0x66, 0x66, 0x66));
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 4*ITEM_BORDER) + 1;
        offset = pix.height() + 5*ITEM_BORDER*(i+1) + v*lineHeight*(i+1);
        painter->drawText(2*ITEM_BORDER, offset, width() - 4*ITEM_BORDER, v*lineHeight, Qt::TextWrapAnywhere, m_data[i]);
    }
    //////////////////////////////////////////////////////////////////////////////
    if(!m_data.isEmpty())
    {
        offset += 5*ITEM_BORDER + v*lineHeight;
    }

    QString title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title))
                                          .arg(MusicUtils::String::songName(m_title));
    v = fm.width(title)/(ITEM_WIDTH - 4*ITEM_BORDER) + 1;
    painter->drawText(2*ITEM_BORDER, offset, width() - 4*ITEM_BORDER, v*lineHeight, Qt::AlignRight | Qt::TextWrapAnywhere, title);
    //////////////////////////////////////////////////////////////////////////////
    offset += 5*ITEM_BORDER + v*lineHeight;
    painter->drawText(2*ITEM_BORDER, offset, width() - 4*ITEM_BORDER, lineHeight, Qt::AlignCenter, tr("TTKMusicPlayer"));
    offset += 3*ITEM_BORDER;
    //////////////////////////////////////////////////////////////////////////////
    setFixedHeight(offset >= ITEM_HEIGHT ? offset + 2*lineHeight : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme2(QPainter *painter)
{
    QFontMetrics fm(font());
    int lineHeight = fm.height();
    int offset = 5*ITEM_BORDER + lineHeight;
    int v = 1;
    //////////////////////////////////////////////////////////////////////////////
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        offset = 5*ITEM_BORDER*(i+1) + v*lineHeight*(i+1);
    }
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //////////////////////////////////////////////////////////////////////////////
    QString title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title))
                                          .arg(MusicUtils::String::songName(m_title));
    v = fm.width(title)/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
    offset += 18*ITEM_BORDER + v*lineHeight;
    if(offset < ITEM_HEIGHT)
    {
        offset = ITEM_HEIGHT;
    }
    painter->drawText(8*ITEM_BORDER, offset - 8*ITEM_BORDER - v*lineHeight, ITEM_WIDTH - 16*ITEM_BORDER, v*lineHeight, Qt::AlignRight | Qt::TextWrapAnywhere, title);
    //////////////////////////////////////////////////////////////////////////////
    painter->setPen(QPen(QColor(0xBB, 0xBB, 0xBB), ITEM_BORDER));
    painter->drawRect(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, offset - 10*ITEM_BORDER);
    setFixedHeight(offset);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //////////////////////////////////////////////////////////////////////////////
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 4*ITEM_BORDER) + 1;
        offset = 3*ITEM_BORDER + 5*ITEM_BORDER*(i+1) + v*lineHeight*(i+1);
        painter->drawText(8*ITEM_BORDER, offset, width() - 16*ITEM_BORDER, v*lineHeight, Qt::TextWrapAnywhere, m_data[i]);
    }
}



MusicLrcPosterTableWidget::MusicLrcPosterTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 237);
}

QString MusicLrcPosterTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcPosterTableWidget::createAllItems(const QStringList &lrcs)
{
    setRowCount(lrcs.count());
    for(int i=0; i<lrcs.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(lrcs[i]);
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, item);
    }
}

void MusicLrcPosterTableWidget::listCellClicked(int row, int column)
{
    MusicFillItemTableWidget::listCellClicked(row, column);

    QStringList data;
    for(int i=0; i<rowCount(); ++i)
    {
        if(item(i, 0)->data(MUSIC_CHECK_ROLE).toBool())
        {
            data << item(i, 1)->text();
        }
    }

    emit textChanged(data);
}



MusicLrcPosterThemeListWidget::MusicLrcPosterThemeListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setIconSize(QSize(40, 40));
    setFrameShape(QFrame::NoFrame);
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setFlow(QListView::TopToBottom);
    setSelectionMode(QListWidget::SingleSelection);
    setFocusPolicy(Qt::ClickFocus);
    setHorizontalScrollMode(QListWidget::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    addListWidgetItem();
}

QString MusicLrcPosterThemeListWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcPosterThemeListWidget::addListWidgetItem()
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/tools/lb_localmanager") ,tr("Default"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/tools/lb_localmanager") ,tr("Plain"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

    setFixedWidth(70*count());
}



MusicLrcPosterWidget::MusicLrcPosterWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcPosterWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_itemWidget = new MusicLrcPosterItemWidget(this);
    m_ui->viewArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->viewArea->setWidgetResizable(true);
    m_ui->viewArea->setFrameShape(QFrame::NoFrame);
    m_ui->viewArea->setFrameShadow(QFrame::Plain);
    m_ui->viewArea->setAlignment(Qt::AlignVCenter);
    m_ui->viewArea->setWidget(m_itemWidget);
    m_ui->viewArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);
    connect(m_ui->lrcArea, SIGNAL(textChanged(QStringList)), m_itemWidget, SLOT(textChanged(QStringList)));

    m_themeWidget = new MusicLrcPosterThemeListWidget(this);
    m_ui->listArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->listArea->setWidgetResizable(true);
    m_ui->listArea->setFrameShape(QFrame::NoFrame);
    m_ui->listArea->setFrameShadow(QFrame::Plain);
    m_ui->listArea->setAlignment(Qt::AlignVCenter);
    m_ui->listArea->setWidget(m_themeWidget);
    m_ui->listArea->horizontalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle04);

    m_ui->openButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->saveButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->openButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->openButton, SIGNAL(clicked()), SLOT(openButtonClicked()));
    connect(m_ui->saveButton, SIGNAL(clicked()), SLOT(saveButtonClicked()));
    connect(m_themeWidget, SIGNAL(currentRowChanged(int)), m_itemWidget, SLOT(currentTypeChanged(int)));
}

MusicLrcPosterWidget::~MusicLrcPosterWidget()
{
    delete m_itemWidget;
    delete m_themeWidget;
    delete m_ui;
}

QString MusicLrcPosterWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcPosterWidget::setCurrentLrcs(const QStringList &lrcs, const QString &name)
{
    m_itemWidget->setCurrentSongName(name);
    m_ui->lrcArea->createAllItems(lrcs);
}

void MusicLrcPosterWidget::openButtonClicked()
{
    QString picPath = MusicUtils::Widget::getOpenFileDialog(this);
    if(picPath.isEmpty())
    {
        return;
    }

    m_itemWidget->setImagePath(picPath);
}

void MusicLrcPosterWidget::saveButtonClicked()
{
    QString filename = MusicUtils::Widget::getSaveFileDialog(this, "Jpeg(*.jpg)");
    if(!filename.isEmpty())
    {
        QRect rect = m_itemWidget->rect();
        if(m_itemWidget->hasScroll())
        {
            rect.setWidth(rect.width() + ITEM_SCROLL_WIDTH);
        }
        m_itemWidget->grab(rect).save(filename, JPG_FILE_PREFIX);
    }
}

int MusicLrcPosterWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
