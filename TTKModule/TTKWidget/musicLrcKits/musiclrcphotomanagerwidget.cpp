#include "musiclrcphotomanagerwidget.h"
#include "ui_musiclrcphotomanagerwidget.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"

MusicLrcPhotoManagerWidget::MusicLrcPhotoManagerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcPhotoManagerWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->artSearchEdit->setStyleSheet(MusicUIObject::LineEditStyle01);
}

MusicLrcPhotoManagerWidget::~MusicLrcPhotoManagerWidget()
{
    delete m_ui;
}

void MusicLrcPhotoManagerWidget::selectButtonClicked()
{
    const QString &path = MusicUtils::File::getOpenFileName(this);
    if(path.isEmpty())
    {
        return;
    }

//    const QPixmap pix(path);
//    if(pix.width() < WINDOW_WIDTH_MIN || pix.height() < WINDOW_HEIGHT_MIN)
//    {
//        m_ui->stateLabel->show();
//        m_ui->uploadButton->hide();
//        m_ui->closeButton->hide();
//    }
//    else
//    {
//        m_ui->stateLabel->hide();
//        m_ui->uploadButton->show();
//        m_ui->closeButton->show();
//        m_ui->introduceLabel->hide();
//        m_ui->selectButton->hide();
//        m_ui->imageLabel->setImagePath(path);
//    }
}

void MusicLrcPhotoManagerWidget::uploadButtonClicked()
{
//    const QDir dir(BACKGROUND_DIR_FULL);
//    int count = 0;
//    const QString &name = m_ui->artSearchEdit->text().trimmed();
//    if(name.isEmpty())
//    {
//        MusicToastLabel::popup(tr("The art is empty!"));
//        return;
//    }

//    for(const QFileInfo &fin : dir.entryInfoList())
//    {
//        if(fin.fileName().contains(name))
//        {
//            ++count;
//        }
//    }

//    const QString &fileName = QString("%1%2%3").arg(BACKGROUND_DIR_FULL, name).arg(count);
//    m_ui->imageLabel->saveImagePath(fileName + JPG_FILE);
//    QFile::rename(fileName + JPG_FILE, fileName + SKN_FILE);
//    close();
}
