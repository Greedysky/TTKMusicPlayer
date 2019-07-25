#include "musiclrclocallinkwidget.h"
#include "ui_musiclrclocallinkwidget.h"
#include "musicconnectionpool.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicfileutils.h"
#include "musicdownloadstatusobject.h"

#include <QDir>

MusicLrcLocalLinkTableWidget::MusicLrcLocalLinkTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 147);
    headerview->resizeSection(1, 215);
}

bool MusicLrcLocalLinkTableWidget::contains(const QString &string)
{
    for(int i=0; i<rowCount(); ++i)
    {
        if(item(i, 1)->toolTip() == string)
        {
            return true;
        }
    }
    return false;
}

void MusicLrcLocalLinkTableWidget::createAllItems(const MusicLocalDataItems &items)
{
    const int count = rowCount();
    setRowCount(count + items.count());

    QHeaderView *headerview = horizontalHeader();
    for(int i=0; i<items.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip( items[i].m_name );
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(0) - 20));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(count + i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip( items[i].m_path );
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(count + i, 1, item);
    }
}

void MusicLrcLocalLinkTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}



MusicLrcLocalLinkWidget::MusicLrcLocalLinkWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcLocalLinkWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->fuzzyButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->localSearchButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->commitButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->previewButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->titleEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

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

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicDownloadStatusObject::getClassName());
}

MusicLrcLocalLinkWidget::~MusicLrcLocalLinkWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_ui;
}

void MusicLrcLocalLinkWidget::setCurrentSongName(const QString &name)
{
    m_currentName = name;
    m_ui->titleEdit->setText(name);
    searchInLocalMLrc();
}

void MusicLrcLocalLinkWidget::searchInLocalMLrc()
{
    const QString &title = m_ui->titleEdit->text().trimmed();
    if(title.isEmpty())
    {
        return;
    }

    m_ui->fuzzyButton->isChecked();
    const QStringList &list = QDir(MusicUtils::String::lrcPrefix()).entryList(QDir::Files |  QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    MusicLocalDataItems items;
    foreach(const QString &var, list)
    {
        if(var.contains(title, m_ui->fuzzyButton->isChecked() ? Qt::CaseInsensitive : Qt::CaseSensitive))
        {
            MusicLocalDataItem item;
            item.m_name = var;
            item.m_path = MusicUtils::String::lrcPrefix() + var;
            items << item;
        }
    }
    m_ui->searchedTable->createAllItems(items);
}

void MusicLrcLocalLinkWidget::fuzzyStateChanged()
{
    m_ui->searchedTable->clear();
    searchInLocalMLrc();
}

void MusicLrcLocalLinkWidget::findInLocalFile()
{
    const QString &picPath = MusicUtils::File::getOpenFileDialog(this, "LRC (*.lrc)");
    if(picPath.isEmpty() || m_ui->searchedTable->contains(picPath))
    {
        return;
    }

    MusicLocalDataItem item;
    item.m_name = QFileInfo(picPath).fileName();
    item.m_path = picPath;

    m_ui->searchedTable->createAllItems(MusicLocalDataItems() << item);
}

void MusicLrcLocalLinkWidget::deleteFoundLrc()
{
    const int row = m_ui->searchedTable->currentRow();
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
        return;
    }

    m_ui->searchedTable->removeRow(row);
}

void MusicLrcLocalLinkWidget::confirmButtonClicked()
{
    const int row = m_ui->searchedTable->currentRow();
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
        return;
    }

    const QString &path = m_ui->searchedTable->item(row, 1)->toolTip();
    QFile fileIn(path);
    if(!fileIn.open(QIODevice::ReadOnly))
    {
        M_LOGGER_ERROR("Lrc Input File Error!");
        fileIn.close();
        close();
        return;
    }

    const QByteArray dataIn(fileIn.readAll());
    fileIn.close();

    QFile fileOut(QString("%1%2%3").arg(MusicUtils::String::lrcPrefix()).arg(m_currentName).arg(LRC_FILE));
    if(!fileOut.open(QIODevice::WriteOnly))
    {
        M_LOGGER_ERROR("Lrc Output File Error!");
        fileOut.close();
        close();
        return;
    }

    fileOut.write(dataIn);
    fileOut.flush();
    fileOut.close();

    emit currentLrcChanged("DownloadLrc");
    close();
}

int MusicLrcLocalLinkWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
