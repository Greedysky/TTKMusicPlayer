#include "musictransformwidget.h"
#include "ui_musictransformwidget.h"
#include "musicmessagebox.h"
#include "musicfileutils.h"
#include "musiccoreutils.h"
#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"

#include <QSound>
#include <QProcess>
#include <QStyledItemDelegate>

MusicTransformWidget::MusicTransformWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicTransformWidget)
{
    m_ui->setupUi(this);
    
    m_process = new QProcess(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->inputButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->outputButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->formatCombo->setItemDelegate(new QStyledItemDelegate(m_ui->formatCombo));
    m_ui->formatCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->formatCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->kbpsCombo->setItemDelegate(new QStyledItemDelegate(m_ui->kbpsCombo));
    m_ui->kbpsCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->kbpsCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->hzCombo->setItemDelegate(new QStyledItemDelegate(m_ui->hzCombo));
    m_ui->hzCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->hzCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->msCombo->setItemDelegate(new QStyledItemDelegate(m_ui->msCombo));
    m_ui->msCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->msCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->transformButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->inputLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->outputLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

    m_ui->inputButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->outputButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->transformButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_ui->inputButton, SIGNAL(clicked()), SLOT(initInputPath()));
    connect(m_ui->outputButton, SIGNAL(clicked()), SLOT(initOutputPath()));
    connect(m_ui->transformButton, SIGNAL(clicked()), SLOT(startTransform()));
    connect(m_process, SIGNAL(finished(int)), SLOT(transformFinish()));

    m_ui->folderBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(m_ui->folderBox, SIGNAL(clicked(bool)), SLOT(folderBoxChecked()));
    m_ui->krc2lrcBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
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

    initControlParameter();
}

MusicTransformWidget::~MusicTransformWidget()
{
    m_process->kill();
    delete m_process;
    delete m_ui;
}

void MusicTransformWidget::initControlParameter() const
{
    m_ui->formatCombo->addItems(QStringList() << "MP3" << "WAV" << "WMA" << "OGG" << "FLAC" << "AC3" << "AAC");
    m_ui->kbpsCombo->addItems(QStringList() << "32" << "48" << "56" << "64" << "80"
                                   << "96" << "112" << "128" << "192" << "224" << "256" << "320");
    m_ui->hzCombo->addItems(QStringList() << "8000" << "12050" << "16000" << "22050"
                                   << "24000" << "32000" << "44100" << "48000");
    m_ui->msCombo->addItems(QStringList() << "Mono" << "Stereo");

    m_ui->kbpsCombo->setCurrentIndex(7);
    m_ui->hzCombo->setCurrentIndex(6);
}

void MusicTransformWidget::initInputPath()
{
    QString path;
    QStringList supportedFormat;
    (m_currentType == Music) ? supportedFormat << "mp3" <<"wav" <<"wma" << "ogg" << "flac" << "ac3" << "aac" : supportedFormat << "krc";
    if(!m_ui->folderBox->isChecked())
    {
        QString filter = "Files (";
        for(int i=0; i<supportedFormat.count(); ++i)
        {
            filter += QString("*.%1 ").arg(supportedFormat[i]);
        }
        filter = filter.trimmed() + ")";

        path = MusicUtils::File::getOpenFileDialog(this, filter);
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
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setViewMode(QFileDialog::Detail);
        if(dialog.exec())
        {
            path = dialog.directory().absolutePath();
            foreach(const QFileInfo &var, MusicUtils::File::getFileListByDir(path, true))
            {
                if(!m_path.contains(var.absoluteFilePath()) && supportedFormat.contains(var.suffix()))
                {
                    m_path << var.absoluteFilePath();
                    m_ui->listWidget->addItem(MusicUtils::Widget::elidedText(font(), m_path.last(), Qt::ElideLeft, LINE_WIDTH));
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
    const QString &path = QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        m_ui->outputLineEdit->setText(path);
    }
}

QString MusicTransformWidget::getTransformSongName() const
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
        foreach(const QString &path, m_path)
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

bool MusicTransformWidget::processTransform(const QString &para) const
{
    if(m_path.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("the input is empty!"));
        message.exec();
        return false;
    }

    const QString &in = m_path[0].trimmed();
    const QString &out = m_ui->outputLineEdit->text().trimmed();

    if(in.isEmpty() || out.isEmpty() )
    {
        MusicMessageBox message;
        message.setText(tr("the out is empty!"));
        message.exec();
        return false;
    }

    if(m_currentType == Music)
    {
        if(m_ui->formatCombo->currentText() == "OGG")
        {
            m_ui->msCombo->setCurrentIndex(1);
        }

        M_LOGGER_INFO(QString("%1%2%3%4").arg(m_ui->formatCombo->currentText()).arg(m_ui->kbpsCombo->currentText())
                                         .arg(m_ui->hzCombo->currentText()).arg(m_ui->msCombo->currentIndex() + 1));

        m_process->start(para, QStringList() << "-i" << in << "-y"
                         << "-ab" << m_ui->kbpsCombo->currentText() + "k"
                         << "-ar" << m_ui->hzCombo->currentText()
                         << "-ac" << QString::number(m_ui->msCombo->currentIndex() + 1)
                         << QString("%1/%2-Transed.%3").arg(out).arg(getTransformSongName()).arg(m_ui->formatCombo->currentText().toLower()));
    }
    else
    {
        M_LOGGER_INFO(QString("%1%2%3").arg(para).arg(in).arg(out));
        m_process->start(para, QStringList() << in << QString("%1/%2%3").arg(out).arg(getTransformSongName()).arg(LRC_FILE));
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
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return -1;
    }

    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
