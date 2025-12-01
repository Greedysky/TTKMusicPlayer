#include <QProgressBar>
#include <QThreadPool>
#include <qmmp/metadatamanager.h>
#include <taglib/mpegfile.h>
#include <taglib/apetag.h>
#include <taglib/flacfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/wavpackfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/mp4file.h>
#include <taglib/opusfile.h>

#include "replaygainner.h"
#include "gain_analysis.h"
#include "replaygainwidget.h"

#define FILE_SKIPPED (Qt::UserRole + 1)

struct ReplayGainInfoItem
{
    QMap<Qmmp::ReplayGainKey, double> info;
    QString url;
    GainHandle_t *handle;
};

ReplayGainWidget::ReplayGainWidget(QWidget *parent)
    : Light(parent),
      m_scannerIndex(0)
{
    m_ui.setupUi(this);
    m_ui.tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 3);
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    m_ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else
    m_ui.tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
#endif

#ifdef Q_OS_UNIX
    m_ui.skipScannedCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui.trackCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui.albumCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui.calculateButton->setFocusPolicy(Qt::NoFocus);
    m_ui.writeButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui.calculateButton, SIGNAL(clicked()), SLOT(calculateButtonClicked()));
    connect(m_ui.writeButton, SIGNAL(clicked()), SLOT(writeButtonClicked()));
}

ReplayGainWidget::~ReplayGainWidget()
{
    stop();
    qDeleteAll(m_replayGainItemList);
    m_replayGainItemList.clear();
}

void ReplayGainWidget::calculateButtonClicked()
{
    m_ui.writeButton->setEnabled(false);
    for(int i = 0; i < m_ui.tableWidget->rowCount(); ++i)
    {
        const QString &url = m_ui.tableWidget->item(i, 0)->data(Qt::UserRole).toString();
        ReplayGainner *scanner = new ReplayGainner();
        m_ui.tableWidget->item(i, 0)->setData(FILE_SKIPPED, false);

        if(!scanner->prepare(url))
        {
            m_ui.tableWidget->setItem(i, 2, new QTableWidgetItem(tr("Error")));
            delete scanner;
            continue;
        }

        if(m_ui.skipScannedCheckBox->isChecked() && !scanner->oldReplayGainInfo().isEmpty())
        {
            qDebug("ReplayGainWidget: skipping scanned file..");
            m_ui.tableWidget->item(i, 0)->setData(FILE_SKIPPED, true);
            const QMap<Qmmp::ReplayGainKey, double> &rg = scanner->oldReplayGainInfo();
            m_ui.tableWidget->setItem(i, 2, new QTableWidgetItem(tr("%1 dB").arg(rg.value(Qmmp::REPLAYGAIN_TRACK_GAIN))));
            m_ui.tableWidget->setItem(i, 3, new QTableWidgetItem(tr("%1 dB").arg(rg.value(Qmmp::REPLAYGAIN_ALBUM_GAIN))));
            m_ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(rg.value(Qmmp::REPLAYGAIN_TRACK_PEAK))));
            m_ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(rg.value(Qmmp::REPLAYGAIN_ALBUM_PEAK))));
            delete scanner;
            continue;
        }

        scanner->setAutoDelete(false);
        m_scanners.append(scanner);
        connect(scanner, SIGNAL(progress(int)), m_ui.tableWidget->cellWidget(i, 1), SLOT(setValue(int)));
        connect(scanner, SIGNAL(finished(QString)), SLOT(scanFinished(QString)));
        QThreadPool::globalInstance()->start(scanner);
    }
}

void ReplayGainWidget::scanFinished(const QString &url)
{
    for(int i = 0; i < m_ui.tableWidget->rowCount(); ++i)
    {
        if(url != m_ui.tableWidget->item(i, 0)->data(Qt::UserRole).toString())
        {
            continue;
        }

        const ReplayGainner *scanner = findScannerByUrl(url);
        if(!scanner)
        {
            qFatal("ReplayGainWidget: unable to find scanner by URL!");
        }

        m_ui.tableWidget->setItem(i, 2, new QTableWidgetItem(tr("%1 dB").arg(scanner->gain())));
        m_ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scanner->peak())));
        break;
    }

    if(++m_scannerIndex == m_scanners.count())
    {
        qDebug("ReplayGainWidget: all threads are finished");
        QThreadPool::globalInstance()->waitForDone();
        QMultiMap<QString, ReplayGainInfoItem*> itemGroupMap; //items grouped  by album

        //group by album name
        for(ReplayGainner *scanner : qAsConst(m_scanners))
        {
            if(!scanner->hasValues())
            {
                continue;
            }

            ReplayGainInfoItem *item = new ReplayGainInfoItem;
            item->info[Qmmp::REPLAYGAIN_TRACK_GAIN] = scanner->gain();
            item->info[Qmmp::REPLAYGAIN_TRACK_PEAK] = scanner->peak();
            item->url = scanner->url();
            item->handle = scanner->handle();
            itemGroupMap.insert(albumName(item->url), item);
        }
        //calculate album peak and gain
        for(const QString &album : itemGroupMap.keys())
        {
            const QList<ReplayGainInfoItem*> &items = itemGroupMap.values(album);
            GainHandle_t **a = (GainHandle_t **) malloc(items.count()*sizeof(GainHandle_t *));
            double album_peak = 0;

            for(int i = 0; i < items.count(); ++i)
            {
                a[i] = items[i]->handle;
                album_peak = qMax(items[i]->info[Qmmp::REPLAYGAIN_TRACK_PEAK], album_peak);
            }

            const double album_gain = GetAlbumGain(a, items.count());
            free(a);
			
            for(ReplayGainInfoItem *item : qAsConst(items))
            {
                item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK] = album_peak;
                item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN] = album_gain;
            }
        }

        m_scannerIndex = 0;
        //clear scanners
        qDeleteAll(m_scanners);
        m_scanners.clear();
        //clear previous replaygain information
        qDeleteAll(m_replayGainItemList);
        m_replayGainItemList.clear();
        //update table
        m_replayGainItemList = itemGroupMap.values();

        for(int i = 0; i < m_ui.tableWidget->rowCount(); ++i)
        {
            const QString &url = m_ui.tableWidget->item(i, 0)->data(Qt::UserRole).toString();
            bool found = false;
            for(const ReplayGainInfoItem *item : qAsConst(m_replayGainItemList))
            {
                if(item->url == url)
                {
                    found = true;
                    const double album_gain = item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN];
                    const double album_peak = item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK];
                    m_ui.tableWidget->setItem(i, 3, new QTableWidgetItem(tr("%1 dB").arg(album_gain)));
                    m_ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(album_peak)));
                }
            }

            if(!found && !m_ui.tableWidget->item(i, 0)->data(FILE_SKIPPED).toBool())
            {
                m_ui.tableWidget->setItem(i, 3, new QTableWidgetItem(tr("Error")));
            }
        }

        //clear items
        itemGroupMap.clear();
        m_ui.writeButton->setEnabled(true);
    }
}

void ReplayGainWidget::open(const QString &track)
{
    //skip streams
    if(track.contains("://"))
    {
        return;
    }

    //skip unsupported files
    if(!MetaDataManager::instance()->supports(track))
    {
        return;
    }

    const QString &ext = track.section(".", -1).toLower();
    if((ext == "mp3") || //mpeg 1 layer 3
        ext == "flac" || //native flac
        ext == "oga" || //ogg flac
        ext == "ogg" ||  //ogg vorbis
        ext == "wv" || //wavpack
        ext == "m4a" || //aac (mp4 container)
        ext == "opus")
    {
        QTableWidgetItem *item = new QTableWidgetItem(track);
        item->setData(Qt::UserRole, track);
        item->setData(Qt::ToolTipRole, track);

        const int row = m_ui.tableWidget->rowCount();
        m_ui.tableWidget->insertRow(row);
        m_ui.tableWidget->setRowHeight(row, 26);
        m_ui.tableWidget->setItem(row, 0, item);

        QProgressBar *progressBar = new QProgressBar(this);
        progressBar->setRange(0, 100);
        m_ui.tableWidget->setCellWidget(row, 1, progressBar);
    }

    m_ui.writeButton->setEnabled(false);
}

void ReplayGainWidget::start()
{

}

void ReplayGainWidget::stop()
{
    if(m_scanners.isEmpty())
    {
        return;
    }

    for(ReplayGainner *scaner : qAsConst(m_scanners))
    {
        scaner->stop();
    }

    QThreadPool::globalInstance()->waitForDone();
    m_scannerIndex = 0;
    qDeleteAll(m_scanners);
    m_scanners.clear();
}

ReplayGainner *ReplayGainWidget::findScannerByUrl(const QString &url)
{
    for(ReplayGainner *scanner : qAsConst(m_scanners))
    {
        if(scanner->url() == url)
        {
            return scanner;
        }
    }
    return nullptr;
}

QString ReplayGainWidget::albumName(const QString &url)
{
    QList<TrackInfo*> playlist = MetaDataManager::instance()->createPlayList(url);
    if(playlist.isEmpty())
    {
        return QString();
    }

    const QString &album = playlist.first()->value(Qmmp::ALBUM);
    qDeleteAll(playlist);
    return album;
}

TagLib::String ReplayGainWidget::gainToString(double value)
{
    return QStringToTString(QString("%1 dB").arg(value, 0, 'f', 2));
}

TagLib::String ReplayGainWidget::peakToString(double value)
{
    return QStringToTString(QString("%1").arg(value, 0, 'f', 6));
}

TagLib::StringList ReplayGainWidget::gainToStringList(double value)
{
   return TagLib::StringList(gainToString(value));
}

TagLib::StringList ReplayGainWidget::peakToStringList(double value)
{
    return TagLib::StringList(peakToString(value));
}

void ReplayGainWidget::writeAPETag(TagLib::APE::Tag *tag, ReplayGainInfoItem *item)
{
    if(m_ui.trackCheckBox->isChecked())
    {
        tag->addValue("REPLAYGAIN_TRACK_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        tag->addValue("REPLAYGAIN_TRACK_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
    }

    if(m_ui.albumCheckBox->isChecked())
    {
        tag->addValue("REPLAYGAIN_ALBUM_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        tag->addValue("REPLAYGAIN_ALBUM_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
    }
}

void ReplayGainWidget::writeID3v2Tag(TagLib::ID3v2::Tag *tag, ReplayGainInfoItem *item)
{
    tag->removeFrames("TXXX");
    if(m_ui.trackCheckBox->isChecked())
    {
        TagLib::ID3v2::UserTextIdentificationFrame *frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        TagLib::StringList fields;
        fields.append("REPLAYGAIN_TRACK_GAIN");
        fields.append(gainToString(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        frame->setText(fields);
        tag->addFrame(frame);

        fields.clear();
        frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        fields.append("REPLAYGAIN_TRACK_PEAK");
        fields.append(peakToString(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
        frame->setText(fields);
        tag->addFrame(frame);
    }

    if(m_ui.albumCheckBox->isChecked())
    {
        TagLib::ID3v2::UserTextIdentificationFrame *frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        TagLib::StringList fields;
        fields.append("REPLAYGAIN_ALBUM_GAIN");
        fields.append(gainToString(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        frame->setText(fields);
        tag->addFrame(frame);

        fields.clear();
        frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        fields.append("REPLAYGAIN_ALBUM_PEAK");
        fields.append(peakToString(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
        frame->setText(fields);
        tag->addFrame(frame);
    }
}

void ReplayGainWidget::writeVorbisComment(TagLib::Ogg::XiphComment *tag, ReplayGainInfoItem *item)
{
    if(m_ui.trackCheckBox->isChecked())
    {
        tag->addField("REPLAYGAIN_TRACK_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        tag->addField("REPLAYGAIN_TRACK_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
    }

    if(m_ui.albumCheckBox->isChecked())
    {
        tag->addField("REPLAYGAIN_ALBUM_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        tag->addField("REPLAYGAIN_ALBUM_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
    }
}

void ReplayGainWidget::writeMP4Tag(TagLib::MP4::Tag *tag, ReplayGainInfoItem *item)
{
    if(m_ui.trackCheckBox->isChecked())
    {
        tag->setItem("----:com.apple.iTunes:replaygain_track_gain", gainToStringList(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        tag->setItem("----:com.apple.iTunes:replaygain_track_peak", gainToStringList(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
    }

    if(m_ui.albumCheckBox->isChecked())
    {
        tag->setItem("----:com.apple.iTunes:replaygain_album_gain", gainToStringList(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        tag->setItem("----:com.apple.iTunes:replaygain_album_peak", gainToStringList(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
    }
}

void ReplayGainWidget::writeButtonClicked()
{
    if(m_replayGainItemList.isEmpty())
    {
        return;
    }

    qDebug("ReplayGainWidget: writing ReplayGain values...");

    for(ReplayGainInfoItem *item : qAsConst(m_replayGainItemList))
    {
        const QString &ext = item->url.section(".", -1).toLower();
        if(ext == "mp3") //mpeg 1 layer 3
        {
            TagLib::MPEG::File file(qPrintable(item->url));
            writeAPETag(file.APETag(true), item);
            writeID3v2Tag(file.ID3v2Tag(true), item);
#if TAGLIB_MAJOR_VERSION == 1 && TAGLIB_MINOR_VERSION <= 11
            file.save(TagLib::MPEG::File::APE | TagLib::MPEG::File::ID3v2, false, 3);
#else
            file.save(TagLib::MPEG::File::APE | TagLib::MPEG::File::ID3v2, TagLib::File::StripNone, TagLib::ID3v2::Version::v3);
#endif
        }
        else if(ext == "flac") //flac
        {
            TagLib::FLAC::File file(qPrintable(item->url));
            writeVorbisComment(file.xiphComment(true), item);
            file.save();
        }
        else if(ext == "oga") //ogg flac
        {
            TagLib::Ogg::FLAC::File file(qPrintable(item->url));
            writeVorbisComment(file.tag(), item);
            file.save();
        }
        else if(ext == "ogg") //ogg vorbis
        {
            TagLib::Ogg::Vorbis::File file(qPrintable(item->url));
            writeVorbisComment(file.tag(), item);
            file.save();
        }
        else if(ext == "wv") //wavpack
        {
            TagLib::WavPack::File file(qPrintable(item->url));
            writeAPETag(file.APETag(true), item);
            file.save();
        }
        else if(ext == "m4a") //MPEG-4 Part 14
        {
            TagLib::MP4::File file(qPrintable(item->url));
            writeMP4Tag(file.tag(), item);
            file.save();
        }
        else if(ext == "opus")
        {
            TagLib::Ogg::Opus::File file(qPrintable(item->url));
            writeVorbisComment(file.tag(), item);
            file.save();
        }
    }
}
