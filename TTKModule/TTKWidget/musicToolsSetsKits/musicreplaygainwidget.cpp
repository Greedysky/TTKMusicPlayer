#include "musicreplaygainwidget.h"
#include "ui_musicreplaygainwidget.h"
#include "musicuiobject.h"
#include "musicsemaphoreloop.h"
#include "musicfileutils.h"
#include "musicwidgetheaders.h"
#include "musicsinglemanager.h"
#include "musicqmmputils.h"
#include "musictoastlabel.h"

#include <QProcess>
#include <QPluginLoader>

///qmmp incldue
#include "lightfactory.h"

#define GAIN_DEFAULT 89
#define GAIN_TRACKDB "Recommended \"Track\" dB change:"
#define GAIN_ALBUMDB "Recommended \"Album\" dB change for all files:"

MusicReplayGainTableWidget::MusicReplayGainTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 332);
    headerview->resizeSection(1, 60);
    headerview->resizeSection(2, 60);
    headerview->resizeSection(3, 60);
    headerview->resizeSection(4, 60);
}

MusicReplayGainTableWidget::~MusicReplayGainTableWidget()
{

}

void MusicReplayGainTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}


MusicReplayGainWidget::MusicReplayGainWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicReplayGainWidget), m_process(nullptr)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->mainViewWidget->setStyleSheet(MusicUIObject::MQSSTabWidgetStyle01);

    m_ui->addFileButton->setIcon(QIcon(":/toolSets/btn_gain_add_file"));
    m_ui->addFileButton->setIconSize(QSize(40, 40));
    m_ui->addFileButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->addFileButton->setToolTip(tr("addFile"));
    m_ui->addFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->addFilesButton->setIcon(QIcon(":/toolSets/btn_gain_add_files"));
    m_ui->addFilesButton->setIconSize(QSize(40, 40));
    m_ui->addFilesButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->addFilesButton->setToolTip(tr("addFiles"));
    m_ui->addFilesButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->rmFileButton->setIcon(QIcon(":/toolSets/btn_gain_rm_file"));
    m_ui->rmFileButton->setIconSize(QSize(40, 40));
    m_ui->rmFileButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->rmFileButton->setToolTip(tr("rmFile"));
    m_ui->rmFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->rmFilesButton->setIcon(QIcon(":/toolSets/btn_gain_rm_files"));
    m_ui->rmFilesButton->setIconSize(QSize(40, 40));
    m_ui->rmFilesButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->rmFilesButton->setToolTip(tr("rmFiles"));
    m_ui->rmFilesButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->analysisButton->setIcon(QIcon(":/toolSets/btn_analysis"));
    m_ui->analysisButton->setIconSize(QSize(40, 40));
    m_ui->analysisButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->analysisButton->setToolTip(tr("analysis"));
    m_ui->analysisButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->applyButton->setIcon(QIcon(":/toolSets/btn_analysis_apply"));
    m_ui->applyButton->setIconSize(QSize(50, 50));
    m_ui->applyButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->applyButton->setToolTip(tr("apply"));
    m_ui->applyButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->volumeLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->volumeLineEdit->setValidator(new QRegExpValidator(QRegExp("-?[0-9]+$"), this));

    m_ui->progressBar->setStyleSheet(MusicUIObject::MQSSProgressBar01);
    m_ui->progressBarAll->setStyleSheet(MusicUIObject::MQSSProgressBar01);

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    m_replayGainWidget = nullptr;
    m_currentIndex = -1;

    initialize();

    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(analysisOutput()));
    connect(m_ui->addFileButton, SIGNAL(clicked()), SLOT(addFileButtonClicked()));
    connect(m_ui->addFilesButton, SIGNAL(clicked()), SLOT(addFilesButtonClicked()));
    connect(m_ui->rmFileButton, SIGNAL(clicked()), SLOT(rmFileButtonClicked()));
    connect(m_ui->rmFilesButton, SIGNAL(clicked()), SLOT(rmFilesButtonClicked()));
    connect(m_ui->analysisButton, SIGNAL(clicked()), SLOT(analysisButtonClicked()));
    connect(m_ui->applyButton, SIGNAL(clicked()), SLOT(applyButtonClicked()));
    connect(m_ui->volumeLineEdit, SIGNAL(textChanged(QString)), SLOT(lineTextChanged(QString)));
    connect(m_ui->selectedAreaWidget, SIGNAL(confirmChanged()), SLOT(confirmDataChanged()));
}

MusicReplayGainWidget::~MusicReplayGainWidget()
{
    G_SINGLE_MANAGER_PTR->removeObject(getClassName());
    delete m_process;
    delete m_ui;
}

void MusicReplayGainWidget::initialize()
{
    QPluginLoader loader;
    loader.setFileName(MusicUtils::QMMP::pluginPath("Light", "lightreplaygain"));
    const QObject *obj = loader.instance();
    LightFactory *factory = nullptr;
    if(obj && (factory = TTKObject_cast(LightFactory*, obj)))
    {
        m_replayGainWidget = factory->create(this);
        m_replayGainWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04 + MusicUIObject::MQSSCheckBoxStyle01 + \
                                          MusicUIObject::MQSSScrollBarStyle03 + MusicUIObject::MQSSProgressBar01 + \
                                          MusicUIObject::MQSSHeaderView01);
        m_ui->replayGainLayout->addWidget(m_replayGainWidget);
    }
    else
    {
        MusicToastLabel::popup(tr("ReplayGain Init Error!"));
    }
}

void MusicReplayGainWidget::createItemFinished(const QString &track, const QString &album)
{
    if(m_currentIndex >= m_paths.count())
    {
        return;
    }

    const int row = m_ui->tableWidget->rowCount();
    m_ui->tableWidget->setRowCount(row + 1);
    QHeaderView *headerview = m_ui->tableWidget->horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setToolTip(m_paths[m_currentIndex]);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(0) - 15));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 0, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(GAIN_DEFAULT - track.toDouble()));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 1, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(m_ui->volumeLineEdit->text().toDouble() - GAIN_DEFAULT + track.toDouble()));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 2, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(GAIN_DEFAULT - album.toDouble()));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 3, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(m_ui->volumeLineEdit->text().toDouble() - GAIN_DEFAULT + album.toDouble()));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 4, item);
}

void MusicReplayGainWidget::setControlEnabled(bool enable)
{
    m_ui->addFileButton->setEnabled(enable);
    m_ui->addFilesButton->setEnabled(enable);
    m_ui->rmFileButton->setEnabled(enable);
    m_ui->rmFilesButton->setEnabled(enable);
    m_ui->analysisButton->setEnabled(enable);
    m_ui->applyButton->setEnabled(enable);
}

void MusicReplayGainWidget::addFileButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters(QStringList() << "All File(*.*)" << "MP3 File(*.mp3)");
    if(dialog.exec())
    {
        setControlEnabled(false);
        const int orcount = m_paths.count();
        for(const QString &path : dialog.selectedFiles())
        {
            if(!m_paths.contains(path))
            {
                m_paths << path;
            }
        }
        for(int i=orcount; i<m_paths.count(); ++i)
        {
            m_currentIndex = i;
            MusicSemaphoreLoop loop(this);
            connect(m_process, SIGNAL(finished(int)), &loop, SLOT(quit()));
            m_process->start(MAKE_GAIN_FULL, QStringList() << m_paths[i]);
            loop.exec();
        }
        setControlEnabled(true);
    }
}

void MusicReplayGainWidget::addFilesButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        setControlEnabled(false);
        for(const QFileInfo &info : MusicUtils::File::getFileListByDir(dialog.directory().absolutePath(), true))
        {
            if(QString(MP3_FILE_PREFIX).contains(info.suffix().toLower()) && !m_paths.contains(info.absoluteFilePath()))
            {
                m_currentIndex = m_paths.count();
                m_paths << info.absoluteFilePath();

                MusicSemaphoreLoop loop(this);
                connect(m_process, SIGNAL(finished(int)), &loop, SLOT(quit()));
                m_process->start(MAKE_GAIN_FULL, QStringList() << m_paths.last());
                loop.exec();
            }
        }
        setControlEnabled(true);
    }
}

void MusicReplayGainWidget::rmFileButtonClicked()
{
    const int row = m_ui->tableWidget->currentRow();
    if(row < 0)
    {
        MusicToastLabel::popup(tr("Please Select One Item First!"));
        return;
    }
    m_ui->tableWidget->removeRow(row);
}

void MusicReplayGainWidget::rmFilesButtonClicked()
{
    m_paths.clear();
    m_ui->tableWidget->clear();
    m_currentIndex = -1;
}

void MusicReplayGainWidget::analysisButtonClicked()
{
    ///do nothing here
}

void MusicReplayGainWidget::applyButtonClicked()
{
    if(m_paths.isEmpty())
    {
        MusicToastLabel::popup(tr("Music gain list is empty!"));
        return;
    }

    disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(analysisOutput()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(applyOutput()));

    setControlEnabled(false);
    m_ui->progressBarAll->setRange(0, m_ui->tableWidget->rowCount());
    for(int i=0; i<m_ui->tableWidget->rowCount(); ++i)
    {
        MusicSemaphoreLoop loop(this);
        connect(m_process, SIGNAL(finished(int)), &loop, SLOT(quit()));
        m_process->start(MAKE_GAIN_FULL, QStringList() << "-g" << m_ui->tableWidget->item(i, 2)->text() << m_paths[i]);
        m_ui->progressBarAll->setValue(i + 1);
        loop.exec();
    }
    setControlEnabled(true);
    rmFilesButtonClicked();

    disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(applyOutput()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(analysisOutput()));

    MusicToastLabel::popup(tr("Music gain finished!"));
}

void MusicReplayGainWidget::lineTextChanged(const QString &text)
{
    const double d = text.toDouble();
    for(int i=0; i<m_ui->tableWidget->rowCount(); ++i)
    {
        QString v = m_ui->tableWidget->item(i, 1)->text();
        m_ui->tableWidget->item(i, 2)->setText(QString::number(d - v.toDouble()));
                v = m_ui->tableWidget->item(i, 3)->text();
        m_ui->tableWidget->item(i, 4)->setText(QString::number(d - v.toDouble()));
    }
}

void MusicReplayGainWidget::analysisOutput()
{
    QString track, album;
    while(m_process->canReadLine())
    {
        QByteArray data = m_process->readLine();
        if(data.contains(GAIN_TRACKDB))
        {
            data.replace(GAIN_TRACKDB, QByteArray());
            data.replace("\r\n", QByteArray());
            track = QString(data.trimmed());
        }
        if(data.contains(GAIN_ALBUMDB))
        {
            data.replace(GAIN_ALBUMDB, QByteArray());
            data.replace("\r\n", QByteArray());
            album = QString(data.trimmed());
        }
    }

    if(!track.isEmpty() || !album.isEmpty())
    {
        createItemFinished(track, album);
    }
}

void MusicReplayGainWidget::applyOutput()
{
    while(m_process->canReadLine())
    {
        const QByteArray &data = m_process->readLine();
        if(data.contains("Applying gain change"))
        {
            m_ui->progressBar->setValue(40);
        }
        else if(data.contains("done"))
        {
            m_ui->progressBar->setValue(100);
        }
        else
        {
            m_ui->progressBar->setValue(80);
        }
    }
}

void MusicReplayGainWidget::confirmDataChanged()
{
    const MusicSongs songs(m_ui->selectedAreaWidget->getSelectedSongItems());
    for(const MusicSong &song : qAsConst(songs))
    {
        if(m_replayGainWidget)
        {
            m_replayGainWidget->open(song.getMusicPath());
        }
    }
}

void MusicReplayGainWidget::show()
{
    if(!QFile::exists(MAKE_GAIN_FULL))
    {
        MusicToastLabel::popup(tr("Lack of plugin file!"));
        return;
    }

    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
