#include "musicuploadfilewidget.h"
#include "musicclickedlabel.h"
#include "musicutils.h"

#include <QGridLayout>

MusicUploadFileWidget::MusicUploadFileWidget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *uploadFileIcon = new QLabel(this);
    uploadFileIcon->setPixmap(QPixmap(":/tiny/btn_open_file"));
    MusicClickedLabel *uploadFile = new MusicClickedLabel(this);
    uploadFile->setText(tr("uploadFile"));
    MusicUtils::UWidget::setLabelFontStyle(uploadFile, MusicObject::FT_Underline);
    connect(uploadFile, SIGNAL(clicked()), SIGNAL(uploadFileClicked()));

    QLabel *uploadDirIcon = new QLabel(this);
    uploadDirIcon->setPixmap(QPixmap(":/tiny/btn_open_files"));
    MusicClickedLabel *uploadDir = new MusicClickedLabel(this);
    uploadDir->setText(tr("uploadFiles"));
    MusicUtils::UWidget::setLabelFontStyle(uploadDir, MusicObject::FT_Underline);
    connect(uploadDir, SIGNAL(clicked()), SIGNAL(uploadFilesClicked()));

    layout->addWidget(uploadFileIcon, 0, 0, Qt::AlignRight);
    layout->addWidget(uploadFile, 0, 1, Qt::AlignLeft);
    layout->addWidget(uploadDirIcon, 1, 0, Qt::AlignRight);
    layout->addWidget(uploadDir, 1, 1, Qt::AlignLeft);
    setLayout(layout);

    resize(150, 50);
}

QString MusicUploadFileWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicUploadFileWidget::adjustRect(int w, int h)
{
    int x = (w - width() - 60)/2;
    int y = (h - height())/2;
    move(x, y);
}
