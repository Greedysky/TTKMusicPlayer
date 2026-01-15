#include "musictransformwidget.h"
#include "ui_musictransformwidget.h"
#include "musicwidgetheaders.h"
#include "musiclrcparser.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"

#include <QProcess>
#if TTK_QT_VERSION_CHECK(6,2,0)
#  include <QSoundEffect>
#elif !TTK_QT_VERSION_CHECK(6,0,0)
#  include <QSound>
#endif

static constexpr int LINE_WIDTH = 420;

MusicTransformWidget::MusicTransformWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicTransformWidget),
      m_currentType(Module::Music)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);
    
    m_process = new QProcess(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    TTK::Widget::generateComboBoxStyle(m_ui->formatCombo);
    TTK::Widget::generateComboBoxStyle(m_ui->kbpsCombo);
    TTK::Widget::generateComboBoxStyle(m_ui->hzCombo);
    TTK::Widget::generateComboBoxStyle(m_ui->msCombo);

    m_ui->inputButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->outputButton->setStyleSheet(TTK::UI::PushButtonStyle04);

    m_ui->transformButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->inputLineEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->outputLineEdit->setStyleSheet(TTK::UI::LineEditStyle01);

    m_ui->listWidget->setStyleSheet(TTK::UI::ListWidgetStyle01);
    m_ui->listWidget->verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    m_ui->inputButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->outputButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->transformButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->folderBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->tabButton->addButtons({tr("Music"), tr("Krc")});

#ifdef Q_OS_UNIX
    m_ui->inputButton->setFocusPolicy(Qt::NoFocus);
    m_ui->outputButton->setFocusPolicy(Qt::NoFocus);
    m_ui->transformButton->setFocusPolicy(Qt::NoFocus);
    m_ui->folderBox->setFocusPolicy(Qt::NoFocus);
#endif

    initialize();

    QtProcessFinishVoidConnect(m_process, this, transformFinish, TTK_SLOT);
    connect(m_ui->inputButton, SIGNAL(clicked()), SLOT(initInputPath()));
    connect(m_ui->outputButton, SIGNAL(clicked()), SLOT(initOutputPath()));
    connect(m_ui->transformButton, SIGNAL(clicked()), SLOT(startTransform()));
    connect(m_ui->folderBox, SIGNAL(clicked(bool)), SLOT(folderBoxChecked()));
    connect(m_ui->tabButton, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));
}

MusicTransformWidget::~MusicTransformWidget()
{
    m_process->kill();
    delete m_process;
    delete m_ui;
}

void MusicTransformWidget::initInputPath()
{
    QString path;
    QStringList supportedFormat;
    (m_currentType == Module::Music) ? (supportedFormat << "mp3" << "wav" << "wma" << "ogg" << "flac" << "ac3" << "aac") : supportedFormat << "krc";

    if(!m_ui->folderBox->isChecked())
    {
        QString filter = "Audio Files (";
        for(int i = 0; i < supportedFormat.count(); ++i)
        {
            filter += QString("*.%1 ").arg(supportedFormat[i]);
        }
        filter = filter.trimmed() + ")";

        path = TTK::File::getOpenFileName(this, filter);
        if(path.isEmpty() || m_path.contains(path))
        {
            return;
        }

        m_ui->listWidget->addItem(TTK::Widget::elidedText(font(), path, Qt::ElideLeft, LINE_WIDTH));
        m_ui->listWidget->setToolTip(path);
        m_path << path;
    }
    else
    {
        path = TTK::File::getExistingDirectory(this);
        if(!path.isEmpty())
        {
            for(const QFileInfo &fin : TTK::File::fileInfoListByPath(path))
            {
                if(!m_path.contains(fin.absoluteFilePath()) && supportedFormat.contains(TTK_FILE_SUFFIX(fin)))
                {
                    m_path << fin.absoluteFilePath();
                    m_ui->listWidget->addItem(TTK::Widget::elidedText(font(), m_path.last(), Qt::ElideLeft, LINE_WIDTH));
                    m_ui->listWidget->setToolTip(m_path.last());
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
    const QString &path = TTK::File::getExistingDirectory(this);
    if(!path.isEmpty())
    {
        m_ui->outputLineEdit->setText(path);
    }
}

void MusicTransformWidget::startTransform()
{
    if(!processTransform())
    {
        return;
    }

    m_ui->loadingLabel->show();
    m_ui->loadingLabel->start();
    setCheckedControl(false);
}

void MusicTransformWidget::transformFinish()
{
#if TTK_QT_VERSION_CHECK(6,2,0)
    QSoundEffect *sound = new QSoundEffect(this);
    sound->setSource(QUrl::fromLocalFile(":/data/sound"));
    sound->play();
#elif !TTK_QT_VERSION_CHECK(6,0,0)
    QSound::play(":/data/sound");
#endif

    m_path.removeFirst();
    m_ui->listWidget->clear();

    if(!m_path.isEmpty())
    {
        for(const QString &path : qAsConst(m_path))
        {
            m_ui->listWidget->addItem(TTK::Widget::elidedText(font(), path, Qt::ElideLeft, LINE_WIDTH));
            m_ui->listWidget->setToolTip(path);
        }

        if(processTransform())
        {
            return;
        }
    }

    setCheckedControl(true);
    m_ui->loadingLabel->execute(false);
}

void MusicTransformWidget::folderBoxChecked()
{
    m_ui->inputLineEdit->clear();
    m_ui->listWidget->clear();
    m_path.clear();
}

void MusicTransformWidget::buttonClicked(int index)
{
    m_currentType = TTKStaticCast(Module, index);
    const bool musicMode = Module::Music == m_currentType;

    m_path.clear();
    m_ui->listWidget->clear();
    m_ui->inputLineEdit->clear();
    m_ui->outputLineEdit->clear();

    m_ui->label_8->setVisible(!musicMode);
    m_ui->label_3->setVisible(musicMode);
    m_ui->label_4->setVisible(musicMode);
    m_ui->label_5->setVisible(musicMode);
    m_ui->label_6->setVisible(musicMode);
    m_ui->formatCombo->setVisible(musicMode);
    m_ui->hzCombo->setVisible(musicMode);
    m_ui->kbpsCombo->setVisible(musicMode);
    m_ui->msCombo->setVisible(musicMode);
}

int MusicTransformWidget::exec()
{
    if(!QFile::exists(MAKE_TRANSFORM_PATH_FULL))
    {
        MusicToastLabel::popup(tr("Lack of plugin file"));
        return -1;
    }

    return MusicAbstractMoveDialog::exec();
}

QString MusicTransformWidget::transformSongName() const
{
    return m_path.isEmpty() ? QString() : QFileInfo(m_path.first()).completeBaseName();
}

void MusicTransformWidget::initialize()
{
    m_ui->formatCombo->addItems({"MP3", "WAV", "WMA", "OGG", "FLAC", "AC3", "AAC"});
    m_ui->kbpsCombo->addItems({"32", "48", "56", "64", "80", "96", "112", "128", "192", "224", "256", "320"});
    m_ui->hzCombo->addItems({"8000", "12050", "16000", "22050", "24000", "32000", "44100", "48000"});
    m_ui->msCombo->addItems({"Mono", "Stereo"});

    m_ui->kbpsCombo->setCurrentIndex(7);
    m_ui->hzCombo->setCurrentIndex(6);

    m_ui->label_8->hide();
    m_ui->loadingLabel->setType(MusicGifLabelWidget::Module::CicleBlue);
}

bool MusicTransformWidget::processTransform()
{
    if(m_path.isEmpty())
    {
        MusicToastLabel::popup(tr("The input file path is empty"));
        return false;
    }

    const QString &in = m_path.first().trimmed();
    const QString &out = m_ui->outputLineEdit->text().trimmed();

    if(in.isEmpty() || out.isEmpty())
    {
        MusicToastLabel::popup(tr("The output file path is empty"));
        return false;
    }

    if(m_currentType == Module::Music)
    {
        if(m_ui->formatCombo->currentText() == "OGG")
        {
            m_ui->msCombo->setCurrentIndex(1);
        }

        TTK_INFO_STREAM(QString("%1 %2 %3 %4").arg(m_ui->formatCombo->currentText(), m_ui->kbpsCombo->currentText(), m_ui->hzCombo->currentText())
                                              .arg(m_ui->msCombo->currentIndex() + 1));

        m_process->start(MAKE_TRANSFORM_PATH_FULL, {"-i", in, "-y",
                                                    "-ab", m_ui->kbpsCombo->currentText() + "k",
                                                    "-ar", m_ui->hzCombo->currentText(),
                                                    "-ac", QString::number(m_ui->msCombo->currentIndex() + 1),
                                                    QString("%1%2-new.%3").arg(out, transformSongName(), m_ui->formatCombo->currentText().toLower())});
    }
    else
    {
        MusicLrcFromKrc krc;
        TTK_INFO_STREAM("Krc to lrc state: " << krc.decode(in, QString("%1%2.%3").arg(out, transformSongName(), LRC_FILE_SUFFIX)));
        transformFinish();
    }
    return true;
}

void MusicTransformWidget::setCheckedControl(bool enabled)
{
    m_ui->inputButton->setEnabled(enabled);
    m_ui->outputButton->setEnabled(enabled);
    m_ui->folderBox->setEnabled(enabled);
    m_ui->transformButton->setEnabled(enabled);
    m_ui->tabButton->setButtonEnabled(enabled);
}
