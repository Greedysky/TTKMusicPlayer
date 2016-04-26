#include "musictransformwidget.h"
#include "ui_musictransformwidget.h"
#include "musicbackgroundmanager.h"
#include "musicmessagebox.h"

#include <QMovie>
#include <QSound>
#include <QProcess>
#include <QFileDialog>
#include <QStyledItemDelegate>

MusicTransformWidget::MusicTransformWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicTransformWidget), m_movie(nullptr)
{
    ui->setupUi(this);
    
    m_process = new QProcess(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->inputButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->outputButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->formatCombo->setItemDelegate(new QStyledItemDelegate(ui->formatCombo));
    ui->formatCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->formatCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->kbpsCombo->setItemDelegate(new QStyledItemDelegate(ui->kbpsCombo));
    ui->kbpsCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->kbpsCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->hzCombo->setItemDelegate(new QStyledItemDelegate(ui->hzCombo));
    ui->hzCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->hzCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->msCombo->setItemDelegate(new QStyledItemDelegate(ui->msCombo));
    ui->msCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->msCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->transformButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->inputLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->outputLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);

    ui->inputButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->outputButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->transformButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->inputButton, SIGNAL(clicked()), SLOT(initInputPath()));
    connect(ui->outputButton, SIGNAL(clicked()), SLOT(initOutputPath()));
    connect(ui->transformButton, SIGNAL(clicked()), SLOT(startTransform()));
    connect(m_process, SIGNAL(finished(int)), SLOT(transformFinish()));

    ui->folderBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(ui->folderBox, SIGNAL(clicked(bool)), SLOT(folderBoxChecked()));
    ui->krc2lrcBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(ui->krc2lrcBox, SIGNAL(clicked(bool)), SLOT(krc2lrcBoxChecked(bool)));

    m_currentType = Music;
    initControlParameter();

}

MusicTransformWidget::~MusicTransformWidget()
{
    m_process->kill();
    delete m_movie;
    delete m_process;
    delete ui;
}

void MusicTransformWidget::initControlParameter() const
{
    ui->formatCombo->addItems(QStringList() << "MP3" << "WAV" << "WMA"
                                   << "OGG" << "FLAC" << "AC3" << "AAC");
    ui->kbpsCombo->addItems(QStringList() << "32" << "48" << "56" << "64" << "80"
                     << "96" << "112" << "128" << "192" << "224" << "256" << "320");
    ui->hzCombo->addItems(QStringList() << "8000" << "12050" << "16000" << "22050"
                            << "24000" << "32000" << "44100" << "48000");
    ui->msCombo->addItems(QStringList() << "Mono" << "Stereo");

    ui->kbpsCombo->setCurrentIndex(7);
    ui->hzCombo->setCurrentIndex(6);
}

void MusicTransformWidget::initInputPath()
{
    QString path;
    QStringList supportedFormat;
    (m_currentType == Music) ? supportedFormat << "mp3" <<"wav" <<"wma" << "ogg" << "flac" << "ac3" << "aac"
                             : supportedFormat << "krc";
    if(!ui->folderBox->isChecked())
    {
        QString filter = "Files (";
        for(int i=0; i<supportedFormat.count(); ++i)
        {
            filter += QString("*.%1 ").arg(supportedFormat[i]);
        }
        filter = filter.trimmed() + ")";

        path = QFileDialog::getOpenFileName( this, QString(), "./", filter);
        if(path.isEmpty() || m_path.contains(path))
        {
            return;
        }

        ui->listWidget->addItem(QFontMetrics(font()).elidedText(path, Qt::ElideLeft, LINE_WIDTH));
        ui->listWidget->setToolTip(path);
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
            foreach(QFileInfo var, getFileList(path))
            {
                if(!m_path.contains(var.absoluteFilePath()) && supportedFormat.contains(var.suffix()))
                {
                    m_path << var.absoluteFilePath();
                    ui->listWidget->addItem(QFontMetrics(font()).elidedText(m_path.last(), Qt::ElideLeft, LINE_WIDTH));
                    ui->listWidget->setToolTip(m_path.last());
                }
            }
        }
    }

    if(!path.isEmpty())
    {
        ui->inputLineEdit->setText(path);
    }
}

QFileInfoList MusicTransformWidget::getFileList(const QString &path)
{
    QDir dir(path);

    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(QFileInfo fileInfo, folderList)
    {
        fileList.append( getFileList(fileInfo.absoluteFilePath()) );
    }
    return fileList;
}

void MusicTransformWidget::initOutputPath()
{
    QString path =  QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        ui->outputLineEdit->setText(path);
    }
}

QString MusicTransformWidget::getTransformSongName() const
{
    if(m_path.isEmpty())
    {
        return QString();
    }
    QString str = m_path[0];
#ifdef Q_OS_WIN
    str.replace("\\", "/");
#endif
    str = str.split('/').back().split('.').front();
    return str;
}

void MusicTransformWidget::transformFinish()
{
    QSound::play("sound.wav");
    m_path.removeAt(0);
    ui->listWidget->clear();

    if(!m_path.isEmpty())
    {
        foreach(QString path, m_path)
        {
            ui->listWidget->addItem(QFontMetrics(font()).elidedText(path, Qt::ElideLeft, LINE_WIDTH));
            ui->listWidget->setToolTip(path);
        }
        if(!processTransform((m_currentType == Music) ? MAKE_TRANSFORM_AL : MAKE_KRC2LRC_AL))
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
        ui->inputLineEdit->clear();
        ui->loadingLabel->hide();
        delete m_movie;
        m_movie = nullptr;
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

    QString in = m_path[0].trimmed();
    QString out = ui->outputLineEdit->text().trimmed();

    if(in.isEmpty() || out.isEmpty() )
    {
        MusicMessageBox message;
        message.setText(tr("the out is empty!"));
        message.exec();
        return false;
    }

    if(m_currentType == Music)
    {
        if(ui->formatCombo->currentText() == "OGG")
        {
            ui->msCombo->setCurrentIndex(1);
        }

        M_LOGGER_INFO(QString("%1%2%3%4").arg(ui->formatCombo->currentText())
                                         .arg(ui->kbpsCombo->currentText())
                                         .arg(ui->hzCombo->currentText())
                                         .arg(ui->msCombo->currentIndex() + 1));

        m_process->start(para, QStringList() << "-i" << in << "-y"
                         << "-ab" << ui->kbpsCombo->currentText() + "k"
                         << "-ar" << ui->hzCombo->currentText()
                         << "-ac" << QString::number(ui->msCombo->currentIndex() + 1)
                         << QString("%1/%2-Transed.%3").arg(out).arg(getTransformSongName())
                            .arg(ui->formatCombo->currentText().toLower()) );
    }
    else
    {
        M_LOGGER_INFO(QString("%1%2%3").arg(para).arg(in).arg(out));
        m_process->start(para, QStringList() << in <<
                         QString("%1/%2%3").arg(out).arg(getTransformSongName()).arg(LRC_FILE));
    }

    return true;
}

void MusicTransformWidget::startTransform()
{
    QString func = (m_currentType == Music) ? MAKE_TRANSFORM_AL : MAKE_KRC2LRC_AL;
    if(!QFile(func).exists() || !processTransform(func))
    {
        return;
    }
    ///////////////////////////////////////////////////////////
    ui->loadingLabel->show();
    ui->loadingLabel->setMovie(m_movie = new QMovie(":/toolSets/loading", QByteArray(), this));
    m_movie->start();
    setCheckedControl(false);
}

void MusicTransformWidget::folderBoxChecked()
{
    ui->inputLineEdit->clear();
    ui->listWidget->clear();
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
    ui->formatCombo->setEnabled(enable);
    ui->kbpsCombo->setEnabled(enable);
    ui->hzCombo->setEnabled(enable);
    ui->msCombo->setEnabled(enable);
}

void MusicTransformWidget::setCheckedControl(bool enable)
{
    setMusicCheckedControl(enable);
    ui->inputButton->setEnabled(enable);
    ui->inputLineEdit->setEnabled(enable);
    ui->outputButton->setEnabled(enable);
    ui->outputLineEdit->setEnabled(enable);
    ui->folderBox->setEnabled(enable);
    ui->krc2lrcBox->setEnabled(enable);
    ui->transformButton->setEnabled(enable);
}

int MusicTransformWidget::exec()
{
    if(!QFile::exists(MAKE_TRANSFORM_AL) || !QFile::exists(MAKE_KRC2LRC_AL))
    {
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return -1;
    }

    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
