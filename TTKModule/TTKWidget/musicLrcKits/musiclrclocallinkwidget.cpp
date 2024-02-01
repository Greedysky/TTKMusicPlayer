#include "musiclrclocallinkwidget.h"
#include "ui_musiclrclocallinkwidget.h"
#include "musicdownloadstatusmodule.h"
#include "musicconnectionpool.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"
#include "musicnetworkdefines.h"

#include <QDir>

MusicLrcLocalLinkTableWidget::MusicLrcLocalLinkTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 147);
    headerView->resizeSection(1, 235);
}

bool MusicLrcLocalLinkTableWidget::contains(const QString &string)
{
    for(int i = 0; i < rowCount(); ++i)
    {
        if(item(i, 1)->toolTip() == string)
        {
            return true;
        }
    }
    return false;
}

void MusicLrcLocalLinkTableWidget::addCellItems(const MusicLocalDataItemList &items)
{
    const int count = rowCount();
    setRowCount(count + items.count());

    QHeaderView *headerView = horizontalHeader();
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicLocalDataItem &v = items[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(v.m_name);
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(0) - 20));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(count + i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.m_path);
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 20));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(count + i, 1, item);
    }
}



MusicLrcLocalLinkWidget::MusicLrcLocalLinkWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcLocalLinkWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->fuzzyButton->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->localSearchButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->commitButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->previewButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->titleEdit->setStyleSheet(TTK::UI::LineEditStyle01);

#ifdef Q_OS_UNIX
    m_ui->fuzzyButton->setFocusPolicy(Qt::NoFocus);
    m_ui->localSearchButton->setFocusPolicy(Qt::NoFocus);
    m_ui->commitButton->setFocusPolicy(Qt::NoFocus);
    m_ui->previewButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deleteButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_ui->fuzzyButton->setChecked(true);

    connect(m_ui->fuzzyButton, SIGNAL(clicked(bool)), SLOT(fuzzyStateChanged()));
    connect(m_ui->previewButton, SIGNAL(clicked()), SLOT(findInLocalFile()));
    connect(m_ui->localSearchButton, SIGNAL(clicked()), SLOT(fuzzyStateChanged()));
    connect(m_ui->deleteButton, SIGNAL(clicked()), SLOT(deleteFoundLrc()));
    connect(m_ui->commitButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(className(), MusicDownloadStatusModule::className());
}

MusicLrcLocalLinkWidget::~MusicLrcLocalLinkWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    delete m_ui;
}

void MusicLrcLocalLinkWidget::setCurrentSongName(const QString &name)
{
    m_currentName = name;
    m_ui->titleEdit->setText(name);
    searchInLocalLrc();
}

void MusicLrcLocalLinkWidget::searchInLocalLrc()
{
    const QString &title = m_ui->titleEdit->text().trimmed();
    if(title.isEmpty())
    {
        return;
    }

    m_ui->fuzzyButton->isChecked();
    const QStringList &list = QDir(TTK::String::lrcDirPrefix()).entryList(QDir::Files |  QDir::Hidden | QDir::NoDotAndDotDot);

    MusicLocalDataItemList items;
    for(const QString &var : qAsConst(list))
    {
        if(var.contains(title, m_ui->fuzzyButton->isChecked() ? Qt::CaseInsensitive : Qt::CaseSensitive))
        {
            MusicLocalDataItem item;
            item.m_name = var;
            item.m_path = TTK::String::lrcDirPrefix() + var;
            items << item;
        }
    }
    m_ui->searchedTable->addCellItems(items);
}

void MusicLrcLocalLinkWidget::fuzzyStateChanged()
{
    m_ui->searchedTable->removeItems();
    searchInLocalLrc();
}

void MusicLrcLocalLinkWidget::findInLocalFile()
{
    const QString &path = TTK::File::getOpenFileName(this, "LRC Files (*.lrc)");
    if(path.isEmpty() || m_ui->searchedTable->contains(path))
    {
        return;
    }

    MusicLocalDataItem item;
    item.m_name = QFileInfo(path).fileName();
    item.m_path = path;

    m_ui->searchedTable->addCellItems({item});
}

void MusicLrcLocalLinkWidget::deleteFoundLrc()
{
    const int row = m_ui->searchedTable->currentRow();
    if(row < 0)
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    m_ui->searchedTable->removeRow(row);
}

void MusicLrcLocalLinkWidget::confirmButtonClicked()
{
    const int row = m_ui->searchedTable->currentRow();
    if(row < 0)
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    const QString &path = m_ui->searchedTable->item(row, 1)->toolTip();
    QFile fileIn(path);
    if(!fileIn.open(QIODevice::ReadOnly))
    {
        TTK_ERROR_STREAM("Lrc Input File Error");
        close();
        return;
    }

    const QByteArray data(fileIn.readAll());
    fileIn.close();

    QFile fileOut(QString("%1%2%3").arg(TTK::String::lrcDirPrefix(), m_currentName, LRC_FILE));
    if(!fileOut.open(QIODevice::WriteOnly))
    {
        TTK_ERROR_STREAM("Lrc Output File Error");
        close();
        return;
    }

    fileOut.write(data);
    fileOut.flush();
    fileOut.close();

    Q_EMIT currentLrcChanged(DOWNLOAD_KEY_LRC);
    close();
}
