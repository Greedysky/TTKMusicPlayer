#include "musicsongstoolboxwidget.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicclickedlabel.h"
#include "musicuiobject.h"
#include "musictinyuiobject.h"
#include "musicsong.h"
#include "musicwidgetutils.h"
#include "musicapplication.h"
#include "musictopareawidget.h"

#include <QMenu>
#include <QPainter>
#include <QScrollBar>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPushButton>

MusicSongsToolBoxTopWidget::MusicSongsToolBoxTopWidget(int index, const QString &text, QWidget *parent)
    : QWidget(parent)
{
    m_renameLine = nullptr;
    m_index = index;
    setFixedHeight(35);

    QHBoxLayout *topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(3, 0, 0, 0);
    topLayout->setSpacing(0);
    m_labelIcon = new QLabel(this);
    m_labelIcon->setPixmap(QPixmap(":/tiny/lb_arrow_up_normal"));
    m_labelText = new QLabel(this);
    m_labelText->setText(text);
    MusicUtils::Widget::setLabelFontStyle(m_labelText, MusicObject::FT_Bold);

    QPushButton *enhanceButton = new QPushButton(this);
    enhanceButton->setToolTip(tr("enhanceLossless"));
    enhanceButton->setStyleSheet(MusicUIObject::MKGTinyBtnEnhanceLossless);
    enhanceButton->setCursor(QCursor(Qt::PointingHandCursor));
    enhanceButton->setGeometry(240, 10, 16, 16);

    QPushButton *shareListButton = new QPushButton(this);
    shareListButton->setToolTip(tr("shareList"));
    shareListButton->setStyleSheet(MusicUIObject::MKGTinyBtnShare);
    shareListButton->setCursor(QCursor(Qt::PointingHandCursor));
    shareListButton->setGeometry(265, 10, 16, 16);

    QPushButton *menuButton = new QPushButton(this);
    menuButton->setToolTip(tr("listMenu"));
    menuButton->setStyleSheet(MusicUIObject::MKGTinyBtnListMenu);
    menuButton->setCursor(QCursor(Qt::PointingHandCursor));
    menuButton->setGeometry(290, 10, 16, 16);
    connect(menuButton, SIGNAL(clicked()), SLOT(showMenu()));

    topLayout->addWidget(m_labelIcon);
    topLayout->addWidget(m_labelText);
    topLayout->addStretch(1);

    setLayout(topLayout);
}

MusicSongsToolBoxTopWidget::~MusicSongsToolBoxTopWidget()
{
    delete m_renameLine;
    delete m_labelIcon;
    delete m_labelText;
}

QString MusicSongsToolBoxTopWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolBoxTopWidget::setItemExpand(bool expand)
{
    m_labelIcon->setPixmap(QPixmap(expand ? ":/tiny/lb_arrow_down_normal" : ":/tiny/lb_arrow_up_normal"));
}

void MusicSongsToolBoxTopWidget::setTitle(const QString &text)
{
    m_labelText->setText(text);
}

QString MusicSongsToolBoxTopWidget::getTitle(bool suffix)
{
    QString text = m_labelText->text().trimmed();
    if(!suffix)
    {
        int index = text.lastIndexOf("[");
        m_suffixString = text.right(text.count() - index);
        text = text.left( index );
    }
    return text;
}

void MusicSongsToolBoxTopWidget::deleteRowItem()
{
    emit deleteRowItem(m_index);
}

void MusicSongsToolBoxTopWidget::deleteRowItemAll()
{
    emit deleteRowItemAll(m_index);
}

void MusicSongsToolBoxTopWidget::changRowItemName()
{
    if(!m_renameLine)
    {
        m_renameLine = new MusicSongsToolItemRenamedWidget(getTitle(), this);
        connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(setChangItemName(QString)));
        m_renameLine->setGeometry(m_labelIcon->width(), 3, 250, height() - 6);
    }
    m_renameLine->show();
}

void MusicSongsToolBoxTopWidget::setChangItemName(const QString &name)
{
    m_labelText->setText(name + m_suffixString);
    m_labelText->setToolTip(name + m_suffixString);
    emit renameFinished(m_index, name);

    m_renameLine->deleteLater();
    m_renameLine = nullptr;
}

void MusicSongsToolBoxTopWidget::addNewFiles()
{
    emit addNewFiles(m_index);
}

void MusicSongsToolBoxTopWidget::addNewDir()
{
    emit addNewDir(m_index);
}

void MusicSongsToolBoxTopWidget::exportSongsItemList()
{
   MusicApplication::instance()->musicExportSongsItemList(m_index);
}

void MusicSongsToolBoxTopWidget::showMenu()
{
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("addNewItem"), this, SIGNAL(addNewRowItem()));
    menu.addSeparator();

    QMenu musicAddNewFiles(tr("addNewFiles"), &menu);
    bool disable = !(m_index == MUSIC_LOVEST_LIST || m_index == MUSIC_NETWORK_LIST || m_index == MUSIC_RECENT_LIST);
    menu.addMenu(&musicAddNewFiles)->setEnabled(disable);
    musicAddNewFiles.addAction(tr("openOnlyFiles"), this, SLOT(addNewFiles()));
    musicAddNewFiles.addAction(tr("openOnlyDir"), this, SLOT(addNewDir()));
    menu.addAction(tr("playLater"));
    menu.addAction(tr("addToPlayList"));
    menu.addAction(tr("collectAll"));
    menu.addAction(tr("exportList"), this, SLOT(exportSongsItemList()));
    menu.addSeparator();

    disable = !(m_index == MUSIC_NORMAL_LIST || m_index == MUSIC_LOVEST_LIST || m_index == MUSIC_NETWORK_LIST ||
                m_index == MUSIC_RECENT_LIST);
    menu.addAction(tr("deleteAll"), this, SLOT(deleteRowItemAll()));
    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("deleteItem"), this, SLOT(deleteRowItem()))->setEnabled(disable);
    menu.addAction(tr("changItemName"), this, SLOT(changRowItemName()))->setEnabled(disable);

    menu.exec(QCursor::pos());
}

void MusicSongsToolBoxTopWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        emit mousePressAt(m_index);
    }
}

void MusicSongsToolBoxTopWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 0.1, Qt::SolidLine));
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height(), width(), height());
}

void MusicSongsToolBoxTopWidget::contextMenuEvent(QContextMenuEvent *event)
{
//    QWidget::contextMenuEvent(event);
    Q_UNUSED(event);
    showMenu();
}


MusicSongsToolBoxMaskWidget::MusicSongsToolBoxMaskWidget(QWidget *parent)
    : MusicSongsToolBoxTopWidget(-1, QString(), parent)
{
    move(0, 0);
    setFixedWidth(320);

    hide();
}

QString MusicSongsToolBoxMaskWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolBoxMaskWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QLinearGradient gradient;
    gradient.setStart(0, 32);
    gradient.setFinalStop(0, height());
    gradient.setColorAt(0.1, QColor(150, 150, 150, 150));
    gradient.setColorAt(0.9, QColor(180, 180, 180, 50));

    painter.setPen(Qt::transparent);
    painter.setBrush(gradient);
    painter.drawRect(0, 32, width(), height());

    QPixmap pix(MusicTopAreaWidget::instance()->getBgSkinPixmap());
    painter.drawPixmap(0, 0, width(), height() - 3, pix.copy(51, 51, width(), height() - 3));
    painter.fillRect(QRect(0, 0, width(), height() - 3),
                     QColor(255, 255, 255, 2.55*MusicTopAreaWidget::instance()->getListBgSkinAlpha()));
}


MusicSongsToolBoxWidgetItem::MusicSongsToolBoxWidgetItem(int index, const QString &text, QWidget *parent)
    : QWidget(parent)
{
    m_topWidget = new MusicSongsToolBoxTopWidget(index, text, this);
    connect(m_topWidget, SIGNAL(mousePressAt(int)), parent, SLOT(mousePressAt(int)));
    connect(m_topWidget, SIGNAL(addNewRowItem()), SIGNAL(addNewRowItem()));
    connect(m_topWidget, SIGNAL(deleteRowItem(int)), SIGNAL(deleteRowItem(int)));
    connect(m_topWidget, SIGNAL(deleteRowItemAll(int)), SIGNAL(deleteRowItemAll(int)));
    connect(m_topWidget, SIGNAL(renameFinished(int,QString)), SIGNAL(changRowItemName(int,QString)));
    connect(m_topWidget, SIGNAL(addNewFiles(int)), SIGNAL(addNewFiles(int)));
    connect(m_topWidget, SIGNAL(addNewDir(int)), SIGNAL(addNewDir(int)));

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_topWidget);
    setLayout(m_layout);
}

MusicSongsToolBoxWidgetItem::~MusicSongsToolBoxWidgetItem()
{
    delete m_topWidget;
    delete m_layout;
}

QString MusicSongsToolBoxWidgetItem::getClassName()
{
    return staticMetaObject.className();
}

QWidget *MusicSongsToolBoxWidgetItem::item(int index)
{
    if(index < 0 || index >= m_itemList.count())
    {
        return nullptr;
    }
    return m_itemList[index];
}

void MusicSongsToolBoxWidgetItem::addItem(QWidget *item)
{
    m_itemList.append(item);
    m_layout->addWidget(item);
}

void MusicSongsToolBoxWidgetItem::removeItem(QWidget *item)
{
    m_itemList.removeAll(item);
    m_layout->removeWidget(item);
}

void MusicSongsToolBoxWidgetItem::setTitle(const QString &text)
{
    m_topWidget->setTitle(text);
}

QString MusicSongsToolBoxWidgetItem::getTitle() const
{
    return m_topWidget->getTitle();
}

void MusicSongsToolBoxWidgetItem::setItemExpand(bool expand)
{
    m_topWidget->setItemExpand(expand);
    foreach(QWidget *w, m_itemList)
    {
        w->setVisible(expand);
    }
}

bool MusicSongsToolBoxWidgetItem::itemExpand() const
{
    if(!m_itemList.isEmpty())
    {
        return m_itemList.first()->isVisible();
    }
    return false;
}

int MusicSongsToolBoxWidgetItem::count() const
{
    return m_itemList.count();
}

void MusicSongsToolBoxWidgetItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicSongsToolBoxWidgetItem::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}


MusicSongsToolBoxWidget::MusicSongsToolBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    m_currentIndex = -1;
    m_itemIndexRaise = 0;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_contentsWidget = new QWidget(this);
    m_contentsWidget->setObjectName("contentsWidget");
    m_contentsWidget->setStyleSheet(QString("#contentsWidget{%1}").arg(MusicUIObject::MBackgroundStyle01));

    m_layout = new QVBoxLayout(m_contentsWidget);
    m_layout->setContentsMargins(0, 0, 0 ,0);
    m_layout->setSpacing(0);
    m_contentsWidget->setLayout(m_layout);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setFrameShadow(QFrame::Plain);
    m_scrollArea->setAlignment(Qt::AlignLeft);
    m_scrollArea->setWidget(m_contentsWidget);

    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);
}

MusicSongsToolBoxWidget::~MusicSongsToolBoxWidget()
{
    while(!m_itemList.isEmpty())
    {
        delete m_itemList.takeLast().m_widgetItem;
    }
    delete m_layout;
    delete m_scrollArea;
}

QString MusicSongsToolBoxWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolBoxWidget::addItem(QWidget *item, const QString &text)
{
    int count = m_layout->count();
    if(count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }

    //hide before widget
    for(int i=0; i<m_itemList.count(); ++i)
    {
        m_itemList[i].m_widgetItem->setItemExpand(false);
    }

    // Add item and make sure it stretches the remaining space.
    MusicSongsToolBoxWidgetItem *it = new MusicSongsToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addItem(item);
    it->setItemExpand(true);

    MusicToolBoxWidgetItem widgetItem;
    widgetItem.m_widgetItem = it;
    widgetItem.m_itemIndex = m_itemIndexRaise++;
    m_itemList.append(widgetItem);

    m_currentIndex = m_itemList.count() - 1;

    m_layout->addWidget(it);
    m_layout->addStretch(5);
}

void MusicSongsToolBoxWidget::removeItem(QWidget *item)
{
    for(int i=0; i<m_itemList.count(); ++i)
    {
        MusicSongsToolBoxWidgetItem *it = m_itemList[i].m_widgetItem;
        for(int j=0; j<it->count(); ++j)
        {
            if(it->item(j) == item)
            {
                m_layout->removeWidget(item);
                m_itemList.takeAt(i).m_widgetItem->deleteLater();
                m_currentIndex = MUSIC_NORMAL_LIST;
                return;
            }
        }
    }
}

void MusicSongsToolBoxWidget::setTitle(QWidget *item, const QString &text)
{
    for(int i=0; i<m_itemList.count(); ++i)
    {
        MusicSongsToolBoxWidgetItem *it = m_itemList[i].m_widgetItem;
        for(int j=0; j<it->count(); ++j)
        {
            if(it->item(j) == item)
            {
                it->setTitle(text);
                return;
            }
        }
    }
}

QString MusicSongsToolBoxWidget::getTitle(QWidget *item) const
{
    for(int i=0; i<m_itemList.count(); ++i)
    {
        MusicSongsToolBoxWidgetItem *it = m_itemList[i].m_widgetItem;
        for(int j=0; j<it->count(); ++j)
        {
            if(it->item(j) == item)
            {
                return it->getTitle();
            }
        }
    }
    return QString();
}

void MusicSongsToolBoxWidget::resizeScrollIndex(int index) const
{
    QScrollBar *bar = m_scrollArea->verticalScrollBar();
    if(bar)
    {
        bar->setSliderPosition(index);
    }
}

int MusicSongsToolBoxWidget::currentIndex() const
{
    return m_currentIndex;
}

int MusicSongsToolBoxWidget::count() const
{
    return m_itemList.count();
}


void MusicSongsToolBoxWidget::setCurrentIndex(int index)
{
    m_currentIndex = index;
    for(int i=0; i<m_itemList.count(); ++i)
    {
        m_itemList[i].m_widgetItem->setItemExpand( i == index );
    }
}

void MusicSongsToolBoxWidget::mousePressAt(int index)
{
    m_currentIndex = foundMappingIndex(index);
    for(int i=0; i<m_itemList.count(); ++i)
    {
        bool hide = (i == m_currentIndex) ? !m_itemList[i].m_widgetItem->itemExpand() : false;
        m_itemList[i].m_widgetItem->setItemExpand(hide);
    }
}

void MusicSongsToolBoxWidget::setTransparent(int alpha)
{
    QString alphaStr = QString("background:rgba(255, 255, 255, %1)").arg(2.55*alpha);
    QWidget *view = m_scrollArea->viewport();
    view->setObjectName("viewport");
    view->setStyleSheet(QString("#viewport{%1}").arg(alphaStr));

    m_scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01 +
                                QString("QScrollBar{ background:rgba(255, 255, 255, %1);}").arg(alpha*2.55) + "\
                                QScrollBar::handle:vertical{ background:#888888;} \
                                QScrollBar::handle:vertical:hover{ background:#666666;}");
}

void MusicSongsToolBoxWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicSongsToolBoxWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

int MusicSongsToolBoxWidget::foundMappingIndex(int index)
{
    int id = -1;
    for(int i=0; i<m_itemList.count(); ++i)
    {
        if(m_itemList[i].m_itemIndex == index)
        {
            id = i;
            break;
        }
    }
    return id;
}
