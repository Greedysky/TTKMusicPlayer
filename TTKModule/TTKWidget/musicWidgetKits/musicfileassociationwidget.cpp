#include "musicfileassociationwidget.h"
#include "ui_musicfileassociationwidget.h"
#include "musicsettingwidget.h"
#include "musicformats.h"

MusicFileAssociationTableWidget::MusicFileAssociationTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(2);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
#ifdef Q_OS_UNIX
    headerview->resizeSection(1, 219);
#else
    headerview->resizeSection(1, 222);
#endif
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);

    addCellItems();
}

void MusicFileAssociationTableWidget::uncheckedDataList() const
{
    QStringList formats;
    const TTKIntList &unsupports = uncheckedIndexList();

    for(int i = 0; i < unsupports.count(); ++i)
    {
        const QTableWidgetItem *it = item(unsupports[i], 1);
        if(it)
        {
            formats << it->text();
        }
    }

    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationValue, formats.join(","));
}

void MusicFileAssociationTableWidget::addCellItems()
{
    QStringList formats = MusicFormats::supportMusicFormats();
    setRowCount(formats.count());
    std::sort(formats.begin(), formats.end());

    const QString &fileAssociationValue = G_SETTING_PTR->value(MusicSettingManager::FileAssociationValue).toString();
    const QStringList &unsupports = fileAssociationValue.split(",");

    for(int i = 0; i < formats.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, unsupports.contains(formats[i]) ? Qt::Unchecked : Qt::Checked);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(formats[i]);
//        item->setForeground(QColor(0xE6, 0x73, 0x00));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
    }
}



MusicFileAssociationWidget::MusicFileAssociationWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicFileAssociationWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->selectAllCheckButton->setStyleSheet(TTK::UI::CheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->selectAllCheckButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_ui->selectAllCheckButton, SIGNAL(clicked(bool)), m_ui->itemTableWidget, SLOT(checkedItemsStatus(bool)));
}

void MusicFileAssociationWidget::confirmButtonClicked()
{
    m_ui->itemTableWidget->uncheckedDataList();
    accept();
}
