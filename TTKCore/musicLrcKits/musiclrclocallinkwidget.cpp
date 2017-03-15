#include "musiclrclocallinkwidget.h"
#include "ui_musiclrclocallinkwidget.h"
#include "musicconnectionpool.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#include "musicdownloadstatusobject.h"

#include <QDir>
#include <QFileDialog>

MusicLrcLocalLinkTableWidget::MusicLrcLocalLinkTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 132);
    headerview->resizeSection(1, 200);
}

QString MusicLrcLocalLinkTableWidget::getClassName()
{
    return staticMetaObject.className();
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

void MusicLrcLocalLinkTableWidget::createAllItems(const LocalDataItems &items)
{
    int count = rowCount();
    setRowCount(count + items.count());
    for(int i=0; i<items.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), items[i].m_name, Qt::ElideRight, 128));
        item->setToolTip( items[i].m_name );
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(count + i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), items[i].m_path, Qt::ElideRight, 195));
        item->setToolTip( items[i].m_path );
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
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->fuzzyButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->localSearchButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->commitButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->previewButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->titleEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

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

QString MusicLrcLocalLinkWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcLocalLinkWidget::setCurrentSongName(const QString &name)
{
    m_ui->titleEdit->setText(name);
    searchInLocalMLrc();
}

void MusicLrcLocalLinkWidget::searchInLocalMLrc()
{
    QString title = m_ui->titleEdit->text().trimmed();
    if(title.isEmpty())
    {
        return;
    }

    m_ui->fuzzyButton->isChecked();
    QStringList list = QDir(MusicUtils::Core::lrcPrefix()).entryList(QDir::Files |  QDir::Hidden |
                                                       QDir::NoSymLinks | QDir::NoDotAndDotDot);
    LocalDataItems items;
    foreach(const QString &var, list)
    {
        if(var.contains(title, m_ui->fuzzyButton->isChecked() ? Qt::CaseInsensitive : Qt::CaseSensitive))
        {
            LocalDataItem item;
            item.m_name = var;
            item.m_path = MusicUtils::Core::lrcPrefix() + var;
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
    QString picPath = QFileDialog::getOpenFileName(
                      this, QString(), "./", "LRC (*.lrc)");
    if(picPath.isEmpty() || m_ui->searchedTable->contains(picPath))
    {
        return;
    }

    LocalDataItems items;
    LocalDataItem item;
    item.m_name = QFileInfo(picPath).fileName();
    item.m_path = picPath;
    items << item;
    m_ui->searchedTable->createAllItems(items);
}

void MusicLrcLocalLinkWidget::deleteFoundLrc()
{
    int row = m_ui->searchedTable->currentRow();
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
        return;
    }

    bool state = QFile::remove(m_ui->searchedTable->item(row, 1)->toolTip());
    MusicMessageBox message;
    message.setText(tr("Remove select lrc %1").arg(state ? tr("success") : tr("failed")));
    message.exec();

    if(state)
    {
        m_ui->searchedTable->removeRow(row);
    }
}

void MusicLrcLocalLinkWidget::confirmButtonClicked()
{
    int row = m_ui->searchedTable->currentRow();
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
        return;
    }

    QString path = m_ui->searchedTable->item(row, 1)->toolTip();
    QFile fileIn(path);
    if(!fileIn.open(QIODevice::ReadOnly))
    {
        M_LOGGER_ERROR("Lrc Input File Error!");
        fileIn.close();
        close();
        return;
    }
    QFile fileOut(path.left(path.lastIndexOf("/") + 1) + m_ui->titleEdit->text() + LRC_FILE);
    if(!fileOut.open(QIODevice::WriteOnly))
    {
        M_LOGGER_ERROR("Lrc Output File Error!");
        fileOut.close();
        close();
        return;
    }
    fileOut.write(fileIn.readAll());
    fileOut.flush();
    fileOut.close();
    fileIn.close();

    emit currentLrcChanged("Lrc");
    close();
}

int MusicLrcLocalLinkWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
