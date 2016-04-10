#include "musicconnecttransferwidget.h"
#include "ui_musicconnecttransferwidget.h"
#include "musicbackgroundmanager.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"

#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QButtonGroup>

MusicConnectTransferWidget::MusicConnectTransferWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicConnectTransferWidget)
{
    ui->setupUi(this);

    m_currentIndex = -1;

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle02);
    ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    ui->transferButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->transferButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->transferButton, SIGNAL(clicked()), SLOT(startToTransferFiles()));

    M_CONNECTION->setValue("MusicConnectTransferWidget", this);
    M_CONNECTION->poolConnect("MusicConnectTransferWidget", "MusicSongsSummarizied");
}

MusicConnectTransferWidget::~MusicConnectTransferWidget()
{
    M_CONNECTION->poolDisConnect("MusicConnectTransferWidget");
    delete ui;
}

void MusicConnectTransferWidget::initColumns()
{
    MusicSongsList songs;
    QStringList names;
    emit getMusicLists(songs, names);

    QButtonGroup *group = new QButtonGroup(this);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(currentPlayListSelected(int)));
    for(int i=0; i<songs.count(); ++i)
    {
        QPushButton *button = new QPushButton(QString("%1(%2)")
                                              .arg(names[i]).arg(songs[i].count()), this);
        button->setStyleSheet(MusicUIObject::MPushButtonStyle05);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        button->setGeometry(32, 100 + 50*i, 90, 25);
        group->addButton(button, i);
    }

    QString path = M_SETTING->value(MusicSettingManager::MobileDevicePathChoiced).toString();
    ui->textLabel->setText(QString("( %1 )").arg(path));
    ui->transferButton->setEnabled( !path.isEmpty() );
}

void MusicConnectTransferWidget::currentPlayListSelected(int index)
{
    MusicSongsList songs;
    QStringList names;
    emit getMusicLists(songs, names);
    if(index >= songs.count())
    {
        return;
    }

    m_currentIndex = index;
    ui->playListTableWidget->clear();
    if(ui->allSelectedcheckBox->isChecked())
    {
        ui->allSelectedcheckBox->click();
    }
    ui->playListTableWidget->setRowCount(songs[index].count());
    for(int i=0; i<songs[index].count(); ++i)
    {
        MusicSong song = songs[index][i];
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        ui->playListTableWidget->setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(QFontMetrics(font()).elidedText(song.getMusicName(), Qt::ElideRight, 280));
        item->setToolTip(song.getMusicPath());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->playListTableWidget->setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setText(QFontMetrics(font()).elidedText(song.getMusicTime(), Qt::ElideRight, 40));
        item->setToolTip(song.getMusicTime());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->playListTableWidget->setItem(i, 2, item);
    }
}

void MusicConnectTransferWidget::selectedAllItems(bool check)
{
    for(int i=0; i<ui->playListTableWidget->rowCount(); ++i)
    {
        ui->playListTableWidget->item(i, 0)->setData(MUSIC_CHECK_ROLE, check);
    }

    if(!check)
    {
        ui->allSelectedcheckBox->setText(tr("allselected"));
        ui->playListTableWidget->clearSelection();
        ui->playListTableWidget->setCurrentIndex(QModelIndex());
    }
    else
    {
        ui->allSelectedcheckBox->setText(tr("allcanceled"));
        ui->playListTableWidget->selectAll();
    }
}

void MusicConnectTransferWidget::startToTransferFiles()
{
    MIntList list(ui->playListTableWidget->getSelectedItems());
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
        return;
    }

    MusicSongsList songs;
    QStringList names;
    emit getMusicLists(songs, names);
    if(m_currentIndex == -1 || m_currentIndex >= songs.count())
    {
        return;
    }

    names.clear();
    MusicSongs song = songs[m_currentIndex];
    QString path = M_SETTING->value(MusicSettingManager::MobileDevicePathChoiced).toString();
    foreach(int index, list)
    {
        names << song[index].getMusicPath();
    }

    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Copy File Mode"));
    progress.setRange(0, names.count());
    for(int i=0; i<names.count(); ++i)
    {
        QString last = names[i].split("/").last();
        QFile::copy(names[i], path + "/" + last);
        progress.setValue(i);
    }

    ui->allSelectedcheckBox->setChecked(false);
    ui->playListTableWidget->clearSelection();
}

int MusicConnectTransferWidget::exec()
{
    initColumns();
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
