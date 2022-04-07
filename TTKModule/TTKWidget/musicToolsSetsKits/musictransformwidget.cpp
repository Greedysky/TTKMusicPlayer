#include "musictransformwidget.h"
#include "ui_musictransformwidget.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"
#include "musicwidgetheaders.h"

#include <QSound>
#include <QProcess>

MusicTransformWidget::MusicTransformWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicTransformWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    
    m_process = new QProcess(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->inputButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->outputButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

    MusicUtils::Widget::generateComboBoxFormat(m_ui->formatCombo);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->kbpsCombo);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->hzCombo);
    MusicUtils::Widget::generateComboBoxFormat(m_ui->msCombo);

    m_ui->transformButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->inputLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->outputLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);

    m_ui->inputButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->outputButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->transformButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_ui->inputButton, SIGNAL(clicked()), SLOT(initInputPath()));
    connect(m_ui->outputButton, SIGNAL(clicked()), SLOT(initOutputPath()));
    connect(m_ui->transformButton, SIGNAL(clicked()), SLOT(startTransform()));
    connect(m_process, SIGNAL(finished(int)), SLOT(transformFinish()));

    m_ui->folderBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    connect(m_ui->folderBox, SIGNAL(clicked(bool)), SLOT(folderBoxChecked()));
    m_ui->krc2lrcBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    connect(m_ui->krc2lrcBox, SIGNAL(clicked(bool)), SLOT(krc2lrcBoxChecked(bool)));

#ifdef Q_OS_UNIX
    m_ui->inputButton->setFocusPolicy(Qt::NoFocus);
    m_ui->outputButton->setFocusPolicy(Qt::NoFocus);
    m_ui->transformButton->setFocusPolicy(Qt::NoFocus);
    m_ui->folderBox->setFocusPolicy(Qt::NoFocus);
    m_ui->krc2lrcBox->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->loadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_currentType = Music;

    initialize();
}

MusicTransformWidget::~MusicTransformWidget()
{
    m_process->kill();
    delete m_process;
    delete m_ui;
}

void MusicTransformWidget::initialize() const
{
    m_ui->formatCombo->addItems({"MP3", "WAV", "WMA", "OGG", "FLAC", "AC3", "AAC"});
    m_ui->kbpsCombo->addItems({"32", "48", "56", "64", "80", "96", "112", "128", "192", "224", "256", "320"});
    m_ui->hzCombo->addItems({"8000", "12050", "16000", "22050", "24000", "32000", "44100", "48000"});
    m_ui->msCombo->addItems({"Mono", "Stereo"});

    m_ui->kbpsCombo->setCurrentIndex(7);
    m_ui->hzCombo->setCurrentIndex(6);
}

void MusicTransformWidget::initInputPath()
{
    QString path;
    QStringList supportedFormat;
    (m_currentType == Music) ? (supportedFormat << "mp3" << "wav" << "wma" << "ogg" << "flac" << "ac3" << "aac") : supportedFormat << "krc";
    if(!m_ui->folderBox->isChecked())
    {
        QString filter = "Files (";
        for(int i = 0; i < supportedFormat.count(); ++i)
        {
            filter += QString("*.%1 ").arg(supportedFormat[i]);
        }
        filter = filter.trimmed() + ")";

        path = MusicUtils::File::openFileDialog(this, filter);
        if(path.isEmpty() || m_path.contains(path))
        {
            return;
        }

        m_ui->listWidget->addItem(MusicUtils::Widget::elidedText(font(), path, Qt::ElideLeft, LINE_WIDTH));
        m_ui->listWidget->setToolTip(path);
        m_path << path;
    }
    else
    {
        path = MusicUtils::File::openDirectoryDialog(this);
        if(!path.isEmpty())
        {
            for(const QFileInfo &fin : MusicUtils::File::fileListByPath(path))
            {
                if(!m_path.contains(fin.absoluteFilePath()) && supportedFormat.contains(FILE_SUFFIX(fin)))
                {
                    m_path << fin.absoluteFilePath();
                    m_ui->listWidget->addItem(MusicUtils::Widget::elidedText(font(), m_path.back(), Qt::ElideLeft, LINE_WIDTH));
                    m_ui->listWidget->setToolTip(m_path.back());
                }
            }
        }
    }

    if(!path.isEmpty())
    {
        m_ui->inputLineEdit->setText(path);
    }
}

void MusicTransformWidget::initOutputPath()
{
    const QString &path = MusicUtils::File::openDirectoryDialog(this);
    if(!path.isEmpty())
    {
        m_ui->outputLineEdit->setText(path);
    }
}

QString MusicTransformWidget::transformSongName() const
{
    if(m_path.isEmpty())
    {
        return QString();
    }
    return QFileInfo(m_path[0]).completeBaseName();
}

void MusicTransformWidget::transformFinish()
{
    QSound::play(":/data/sound");

    m_path.removeAt(0);
    m_ui->listWidget->clear();

    if(!m_path.isEmpty())
    {
        for(const QString &path : qAsConst(m_path))
        {
            m_ui->listWidget->addItem(MusicUtils::Widget::elidedText(font(), path, Qt::ElideLeft, LINE_WIDTH));
            m_ui->listWidget->setToolTip(path);
        }

        if(!processTransform((m_currentType == Music) ? MAKE_TRANSFORM_FULL : MAKE_KRC2LRC_FULL))
        {
            return;
        }
    }
    else
    {
        setCheckedControl(true);
        if(m_currentType == Lrc)
        {
            setMusicCheckedControl(false);
        }

        m_ui->inputLineEdit->clear();
        m_ui->loadingLabel->run(false);
    }
}

bool MusicTransformWidget::processTransform(const QString &para)
{
    if(m_path.isEmpty())
    {
        MusicToastLabel::popup(tr("The input file path is empty!"));
        return false;
    }

    const QString &in = m_path[0].trimmed();
    const QString &out = m_ui->outputLineEdit->text().trimmed();

    if(in.isEmpty() || out.isEmpty())
    {
        MusicToastLabel::popup(tr("The output file path is empty!"));
        return false;
    }

    if(m_currentType == Music)
    {
        if(m_ui->formatCombo->currentText() == "OGG")
        {
            m_ui->msCombo->setCurrentIndex(1);
        }

        TTK_LOGGER_INFO(QString("%1%2%3%4").arg(m_ui->formatCombo->currentText(), m_ui->kbpsCombo->currentText(), m_ui->hzCombo->currentText())
                                           .arg(m_ui->msCombo->currentIndex() + 1));

        m_process->start(para, QStringList() << "-i" << in << "-y"
                                << "-ab" << m_ui->kbpsCombo->currentText() + "k"
                                << "-ar" << m_ui->hzCombo->currentText()
                                << "-ac" << QString::number(m_ui->msCombo->currentIndex() + 1)
                                << QString("%1/%2-Transed.%3").arg(out, transformSongName(), m_ui->formatCombo->currentText().toLower()));
    }
    else
    {
        TTK_LOGGER_INFO(QString("%1%2%3").arg(para, in, out));
        m_process->start(para, QStringList() << in << QString("%1/%2%3").arg(out, transformSongName(), LRC_FILE));
    }

    return true;
}

void MusicTransformWidget::startTransform()
{
    const QString &func = (m_currentType == Music) ? MAKE_TRANSFORM_FULL : MAKE_KRC2LRC_FULL;
    if(!QFile(func).exists() || !processTransform(func))
    {
        return;
    }
    //
    m_ui->loadingLabel->show();
    m_ui->loadingLabel->start();
    setCheckedControl(false);
}

void MusicTransformWidget::folderBoxChecked()
{
    m_ui->inputLineEdit->clear();
    m_ui->listWidget->clear();
    m_path.clear();
}

void MusicTransformWidget::krc2lrcBoxChecked(bool check)
{
    m_currentType = check ? Lrc : Music;
    folderBoxChecked();
    setMusicCheckedControl(!check);
}

void MusicTransformWidget::setMusicCheckedControl(bool enable)
{
    m_ui->formatCombo->setEnabled(enable);
    m_ui->kbpsCombo->setEnabled(enable);
    m_ui->hzCombo->setEnabled(enable);
    m_ui->msCombo->setEnabled(enable);
}

void MusicTransformWidget::setCheckedControl(bool enable)
{
    setMusicCheckedControl(enable);
    m_ui->inputButton->setEnabled(enable);
    m_ui->inputLineEdit->setEnabled(enable);
    m_ui->outputButton->setEnabled(enable);
    m_ui->outputLineEdit->setEnabled(enable);
    m_ui->folderBox->setEnabled(enable);
    m_ui->krc2lrcBox->setEnabled(enable);
    m_ui->transformButton->setEnabled(enable);
}

int MusicTransformWidget::exec()
{
    if(!QFile::exists(MAKE_TRANSFORM_FULL) || !QFile::exists(MAKE_KRC2LRC_FULL))
    {
        MusicToastLabel::popup(tr("Lack of plugin file!"));
        return -1;
    }

    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
