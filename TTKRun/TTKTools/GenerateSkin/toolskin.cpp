#include "toolskin.h"
#include "ui_toolskin.h"
#include "musicextractmanager.h"
#include "musicfileutils.h"
#include "musiccolordialog.h"
#include "musicbackgroundconfigmanager.h"

ToolSkin::ToolSkin(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::ToolSkin),
      m_pixmap(QPixmap(1, 1))
{
    m_ui->setupUi(this);
    setFixedSize(630, 400);

    m_pixmap.fill(Qt::white);
    m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));

    connect(m_ui->localButton, SIGNAL(clicked()), SLOT(imageClicked()));
    connect(m_ui->paletteButton, SIGNAL(clicked()), SLOT(paletteClicked()));
    connect(m_ui->readButton, SIGNAL(clicked()), SLOT(readClicked()));
    connect(m_ui->writeButton, SIGNAL(clicked()), SLOT(writeClicked()));
}

ToolSkin::~ToolSkin()
{
    delete m_ui;
}

void ToolSkin::imageClicked()
{
    const QString &path = TTK::File::getOpenFileName(this);
    if(path.isEmpty())
    {
        return;
    }

    m_pixmap = QPixmap(path);
    m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));
}

void ToolSkin::paletteClicked()
{
    const QColor &color = MusicColorDialog::popup(this);
    if(!color.isValid())
    {
        return;
    }

    m_pixmap = QPixmap(1, 1);
    m_pixmap.fill(color);
    m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));
}

void ToolSkin::readClicked()
{
    const QString &path = TTK::File::getOpenFileName(this, "TKM Files (*.tkm)");
    if(path.isEmpty())
    {
        return;
    }

    MusicBackgroundImage image;
    if(MusicExtractManager::outputSkin(&image, path))
    {
        m_pixmap = image.m_pix;
        m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));
        m_ui->nameLineEdit->setText(image.m_item.m_name);
        m_ui->countLineEdit->setText(QString::number(image.m_item.m_useCount));
    }
}

void ToolSkin::writeClicked()
{
    const QString &path = TTK::File::getSaveFileName(this, "TKM Files (*.tkm)");
    if(path.isEmpty())
    {
        return;
    }

    MusicBackgroundImage image;
    image.m_pix = m_pixmap;
    image.m_item.m_name = m_ui->nameLineEdit->text().trimmed();
    image.m_item.m_useCount = m_ui->countLineEdit->text().trimmed().toInt();
    TTK_INFO_STREAM("Save state " << MusicExtractManager::inputSkin(&image, path));
}
