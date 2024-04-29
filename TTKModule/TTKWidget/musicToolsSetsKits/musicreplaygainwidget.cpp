#include "musicreplaygainwidget.h"
#include "ui_musicreplaygainwidget.h"
#include "musicfileutils.h"
#include "musicwidgetheaders.h"
#include "musicqmmputils.h"
#include "musictoastlabel.h"
#include "ttksemaphoreloop.h"

#include <QProcess>
#include <QPluginLoader>

#include <qmmp/lightfactory.h>

static constexpr int GAIN_DEFAULT = 89;
static constexpr const char *GAIN_TRACKDB = "Recommended \"Track\" dB change:";
static constexpr const char *GAIN_ALBUMDB = "Recommended \"Album\" dB change for all files:";

MusicReplayGainTableWidget::MusicReplayGainTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(5);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 332);
    headerView->resizeSection(1, 60);
    headerView->resizeSection(2, 60);
    headerView->resizeSection(3, 60);
    headerView->resizeSection(4, 60);
}



MusicReplayGainWidget::MusicReplayGainWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicReplayGainWidget),
      m_replayGainWidget(nullptr),
      m_currentIndex(-1)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->mainViewWidget->setStyleSheet(TTK::UI::TabWidgetStyle01);

    m_ui->addFileButton->setIcon(QIcon(":/toolSets/btn_gain_add_file"));
    m_ui->addFileButton->setIconSize(QSize(40, 40));
    m_ui->addFileButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->addFileButton->setToolTip(tr("Add File"));
    m_ui->addFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->addFilesButton->setIcon(QIcon(":/toolSets/btn_gain_add_files"));
    m_ui->addFilesButton->setIconSize(QSize(40, 40));
    m_ui->addFilesButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->addFilesButton->setToolTip(tr("Add Files"));
    m_ui->addFilesButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->rmFileButton->setIcon(QIcon(":/toolSets/btn_gain_rm_file"));
    m_ui->rmFileButton->setIconSize(QSize(40, 40));
    m_ui->rmFileButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->rmFileButton->setToolTip(tr("Remove File"));
    m_ui->rmFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->rmFilesButton->setIcon(QIcon(":/toolSets/btn_gain_rm_files"));
    m_ui->rmFilesButton->setIconSize(QSize(40, 40));
    m_ui->rmFilesButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->rmFilesButton->setToolTip(tr("Remove Files"));
    m_ui->rmFilesButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->analysisButton->setIcon(QIcon(":/toolSets/btn_analysis"));
    m_ui->analysisButton->setIconSize(QSize(40, 40));
    m_ui->analysisButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->analysisButton->setToolTip(tr("Analysis"));
    m_ui->analysisButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->applyButton->setIcon(QIcon(":/toolSets/btn_analysis_apply"));
    m_ui->applyButton->setIconSize(QSize(50, 50));
    m_ui->applyButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->applyButton->setToolTip(tr("Apply"));
    m_ui->applyButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->volumeLineEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->volumeLineEdit->setValidator(new QRegExpValidator(QRegExp("-?[0-9]+$"), this));

    m_ui->progressBar->setStyleSheet(TTK::UI::ProgressBar01);
    m_ui->progressBarAll->setStyleSheet(TTK::UI::ProgressBar01);

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);

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
    TTKRemoveSingleWidget(className());
    delete m_process;
    delete m_ui;
}

void MusicReplayGainWidget::initialize()
{
    QPluginLoader loader;
    loader.setFileName(TTK::TTKQmmp::pluginPath("Light", "lightreplaygain"));
    const QObject *obj = loader.instance();

    LightFactory *factory = nullptr;
    if(obj && (factory = TTKObjectCast(LightFactory*, obj)))
    {
        m_replayGainWidget = factory->create(this);
        m_replayGainWidget->setStyleSheet(TTK::UI::PushButtonStyle04 + TTK::UI::CheckBoxStyle01 +
                                          TTK::UI::ScrollBarStyle03 + TTK::UI::ProgressBar01 +
                                          TTK::UI::HeaderView01);
        m_ui->replayGainLayout->addWidget(m_replayGainWidget);
    }
    else
    {
        MusicToastLabel::popup(tr("ReplayGain init error"));
    }
}

void MusicReplayGainWidget::addItemFinished(const QString &track, const QString &album)
{
    if(m_currentIndex >= m_paths.count())
    {
        return;
    }

    const int row = m_ui->tableWidget->rowCount();
    m_ui->tableWidget->setRowCount(row + 1);
    QHeaderView *headerView = m_ui->tableWidget->horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setToolTip(m_paths[m_currentIndex]);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(0) - 15));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 0, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(GAIN_DEFAULT - track.toDouble()));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 1, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(m_ui->volumeLineEdit->text().toDouble() - GAIN_DEFAULT + track.toDouble()));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 2, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(GAIN_DEFAULT - album.toDouble()));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 3, item);

                      item = new QTableWidgetItem;
    item->setText(QString::number(m_ui->volumeLineEdit->text().toDouble() - GAIN_DEFAULT + album.toDouble()));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->tableWidget->setItem(row, 4, item);
}

void MusicReplayGainWidget::setControlEnabled(bool enabled)
{
    m_ui->addFileButton->setEnabled(enabled);
    m_ui->addFilesButton->setEnabled(enabled);
    m_ui->rmFileButton->setEnabled(enabled);
    m_ui->rmFilesButton->setEnabled(enabled);
    m_ui->analysisButton->setEnabled(enabled);
    m_ui->applyButton->setEnabled(enabled);
}

void MusicReplayGainWidget::addFileButtonClicked()
{
    const QStringList &files = TTK::File::getOpenFileNames(this, "MP3 File(*.mp3)");
    if(!files.isEmpty())
    {
        setControlEnabled(false);
        const int orcount = m_paths.count();

        for(const QString &path : qAsConst(files))
        {
            if(!m_paths.contains(path))
            {
                m_paths << path;
            }
        }

        for(int i = orcount; i < m_paths.count(); ++i)
        {
            m_currentIndex = i;
            TTKSemaphoreLoop loop;
            connect(m_process, SIGNAL(finished(int)), &loop, SLOT(quit()));
            m_process->start(MAKE_GAIN_PATH_FULL, {m_paths[i]});
            loop.exec();
        }
        setControlEnabled(true);
    }
}

void MusicReplayGainWidget::addFilesButtonClicked()
{
    const QString &path = TTK::File::getExistingDirectory(this);
    if(!path.isEmpty())
    {
        setControlEnabled(false);
        for(const QFileInfo &fin : TTK::File::fileInfoListByPath(path))
        {
            if(TTK_FILE_SUFFIX(fin) == MP3_FILE_SUFFIX && !m_paths.contains(fin.absoluteFilePath()))
            {
                m_currentIndex = m_paths.count();
                m_paths << fin.absoluteFilePath();

                TTKSemaphoreLoop loop;
                connect(m_process, SIGNAL(finished(int)), &loop, SLOT(quit()));
                m_process->start(MAKE_GAIN_PATH_FULL, {m_paths.back()});
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
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }
    m_ui->tableWidget->removeRow(row);
}

void MusicReplayGainWidget::rmFilesButtonClicked()
{
    m_paths.clear();
    m_ui->tableWidget->removeItems();
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
        MusicToastLabel::popup(tr("Music gain list is empty"));
        return;
    }

    disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(analysisOutput()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(applyOutput()));

    setControlEnabled(false);
    m_ui->progressBarAll->setRange(0, m_ui->tableWidget->rowCount());

    for(int i = 0; i < m_ui->tableWidget->rowCount(); ++i)
    {
        TTKSemaphoreLoop loop;
        connect(m_process, SIGNAL(finished(int)), &loop, SLOT(quit()));
        m_process->start(MAKE_GAIN_PATH_FULL, {"-g", m_ui->tableWidget->item(i, 2)->text(), m_paths[i]});
        m_ui->progressBarAll->setValue(i + 1);
        loop.exec();
    }

    setControlEnabled(true);
    rmFilesButtonClicked();

    disconnect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(applyOutput()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(analysisOutput()));

    MusicToastLabel::popup(tr("Music gain finished"));
}

void MusicReplayGainWidget::lineTextChanged(const QString &text)
{
    const double d = text.toDouble();
    for(int i = 0; i < m_ui->tableWidget->rowCount(); ++i)
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
            data.replace(GAIN_TRACKDB, {});
            data.replace(TTK_WLINEFEED, {});
            track = QString(data.trimmed());
        }

        if(data.contains(GAIN_ALBUMDB))
        {
            data.replace(GAIN_ALBUMDB, {});
            data.replace(TTK_WLINEFEED, {});
            album = QString(data.trimmed());
        }
    }

    if(!track.isEmpty() || !album.isEmpty())
    {
        addItemFinished(track, album);
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
    const MusicSongList songs(m_ui->selectedAreaWidget->selectedSongItems());
    for(const MusicSong &song : qAsConst(songs))
    {
        if(m_replayGainWidget)
        {
            m_replayGainWidget->open(song.path());
        }
    }
}

void MusicReplayGainWidget::show()
{
    if(!QFile::exists(MAKE_GAIN_PATH_FULL))
    {
        MusicToastLabel::popup(tr("Lack of plugin file"));
        return;
    }

    MusicAbstractMoveWidget::show();
}
