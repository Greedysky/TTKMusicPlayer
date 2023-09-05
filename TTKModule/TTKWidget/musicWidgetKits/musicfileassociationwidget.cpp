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

void MusicFileAssociationTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem *it = nullptr;

    it = item(row, 0);
    const Qt::CheckState status = TTKStaticCast(Qt::CheckState, it->data(TTK_CHECKED_ROLE).toInt());
    it->setData(TTK_CHECKED_ROLE, status == Qt::Checked ? Qt::Unchecked : Qt::Checked);

    it = item(row, 1);
    it->setForeground(status == Qt::Checked ? Qt::gray : QColor(0xE6, 0x73, 0x00));
}

void MusicFileAssociationTableWidget::checkedItemsStatus(bool checked)
{
    MusicFillItemTableWidget::checkedItemsStatus(checked);

    for(int i = 0; i < rowCount(); ++i)
    {
        item(i, 1)->setForeground(!checked ? Qt::gray : QColor(0xE6, 0x73, 0x00));
    }
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
        const bool v = unsupports.contains(formats[i]);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, v ? Qt::Unchecked : Qt::Checked);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(formats[i]);
        item->setForeground(v ? Qt::gray : QColor(0xE6, 0x73, 0x00));
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



QStringList TTK::supportAssociations()
{
    QStringList formats = MusicFormats::supportMusicFormats();

    for(const QString &unsupport : TTK::unsupportAssociations())
    {
        if(formats.contains(unsupport))
        {
            formats.removeOne(unsupport);
        }
    }
    return formats;
}

QStringList TTK::unsupportAssociations()
{
    return G_SETTING_PTR->value(MusicSettingManager::FileAssociationValue).toString().split(",");
}
