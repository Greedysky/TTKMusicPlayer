#include "musiclrcartphotouploadwidget.h"
#include "ui_musiclrcartphotouploadwidget.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicmessagebox.h"

#include <QFileDialog>

MusicLrcArtPhotoUploadWidget::MusicLrcArtPhotoUploadWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcArtPhotoUploadWidget)
{
    m_ui->setupUi(this);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->artSearchEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->uploadButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->closeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->selectButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->uploadButton->setFocusPolicy(Qt::NoFocus);
    m_ui->closeButton->setFocusPolicy(Qt::NoFocus);
    m_ui->selectButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->uploadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->selectButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->stateLabel->setStyleSheet(MusicUIObject::MBackgroundStyle05 +
                                  MusicUIObject::MColorStyle07);

    m_ui->uploadButton->hide();
    m_ui->closeButton->hide();
    m_ui->stateLabel->hide();

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->selectButton, SIGNAL(clicked()), SLOT(selectButtonClicked()));
    connect(m_ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->uploadButton, SIGNAL(clicked()), SLOT(uploadButtonClicked()));
}

MusicLrcArtPhotoUploadWidget::~MusicLrcArtPhotoUploadWidget()
{
    delete m_ui;
}

QString MusicLrcArtPhotoUploadWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcArtPhotoUploadWidget::selectButtonClicked()
{
    QString picPath = QFileDialog::getOpenFileName(
                      this, QString(), "./", "Images (*.png *.bmp *.jpg)");
    if(picPath.isEmpty())
    {
        return;
    }

    QPixmap pix(picPath);
    if(pix.width() < WINDOW_WIDTH_MIN || pix.height() < WINDOW_HEIGHT_MIN)
    {
        m_ui->stateLabel->show();
        m_ui->uploadButton->hide();
        m_ui->closeButton->hide();
    }
    else
    {
        m_ui->stateLabel->hide();
        m_ui->uploadButton->show();
        m_ui->closeButton->show();
        m_ui->introduceLabel->hide();
        m_ui->selectButton->hide();
        m_ui->imageLabel->setImagePath(picPath);
    }
}

void  MusicLrcArtPhotoUploadWidget::uploadButtonClicked()
{
    QDir bgDir(BACKGROUND_DIR_FULL);
    int count = 0;
    QString name = m_ui->artSearchEdit->text().trimmed();
    if(name.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("the art is empty!"));
        message.exec();
        return;
    }

    foreach(const QFileInfo &f, bgDir.entryInfoList())
    {
        if(f.fileName().contains( name ))
        {
            ++count;
        }
    }

    QString fileName = QString("%1%2%3").arg(BACKGROUND_DIR_FULL).arg(name).arg(count);
    m_ui->imageLabel->saveImagePath(fileName + JPG_FILE);
    QFile::rename(fileName + JPG_FILE, fileName + SKN_FILE );
    close();
}

int MusicLrcArtPhotoUploadWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
