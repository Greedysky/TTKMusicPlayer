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

void MusicLrcPosterItemWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    QPixmap pix(m_pixPath);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    painter.drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);

    QFontMetrics fm(font());
    int lineHeight = fm.height();
    int offset = pix.height() + 5*ITEM_BORDER + lineHeight;

    painter.setPen(QColor(0x66, 0x66, 0x66));
    for(int i=0; i<m_data.count(); ++i)
    {
        offset = pix.height() + 5*ITEM_BORDER*(i + 1) + lineHeight*(i + 1);
        painter.drawText(2*ITEM_BORDER, offset, m_data[i]);
    }

    if(!m_data.isEmpty())
    {
        offset += 5*ITEM_BORDER + lineHeight;
    }

    QString title = "---" + MusicUtils::String::artistName(m_title) + "●" + MusicUtils::String::songName(m_title);
    painter.drawText(2*ITEM_BORDER, offset, width() - 3*ITEM_BORDER, lineHeight, Qt::AlignRight, title);

    setFixedHeight(offset >= ITEM_HEIGHT ? offset + 2*lineHeight : ITEM_HEIGHT);
}



MusicLrcPosterTableWidget::MusicLrcPosterTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);

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
    m_ui->viewArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);
    connect(m_ui->lrcArea, SIGNAL(textChanged(QStringList)), m_itemWidget, SLOT(textChanged(QStringList)));

    m_ui->openButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->saveButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->openButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->openButton, SIGNAL(clicked()), SLOT(openButtonClicked()));
    connect(m_ui->saveButton, SIGNAL(clicked()), SLOT(saveButtonClicked()));

}

MusicLrcPosterWidget::~MusicLrcPosterWidget()
{
    delete m_itemWidget;
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
