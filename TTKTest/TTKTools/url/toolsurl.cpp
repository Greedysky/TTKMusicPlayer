#include "toolsurl.h"
#include "ui_toolsurl.h"
#include "musicglobal.h"
#include "musicalgorithmutils.h"

ToolsUrl::ToolsUrl(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::ToolsUrl)
{
    m_ui->setupUi(this);

    setFixedSize(500, 300);

    connect(m_ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->encodeButton, SIGNAL(clicked()), SLOT(encodeClicked()));
    connect(m_ui->decodeButton, SIGNAL(clicked()), SLOT(decodeClicked()));
}

ToolsUrl::~ToolsUrl()
{
    delete m_ui;
}

void ToolsUrl::encodeClicked()
{
    QString key = m_ui->keyLineEdit->text().trimmed();
    if(key.isEmpty())
    {
        key = ALG_URL_KEY;
    }

    QString input = m_ui->inputLineEdit->text().trimmed();
    QString output = MusicUtils::Algorithm::mdII(input, key, true);
    m_ui->outPutLineEdit->setText(output);
}

void ToolsUrl::decodeClicked()
{
    QString key = m_ui->keyLineEdit->text().trimmed();
    if(key.isEmpty())
    {
        key = ALG_URL_KEY;
    }

    QString input = m_ui->inputLineEdit->text().trimmed();
    QString output = MusicUtils::Algorithm::mdII(input, key, false);
    m_ui->outPutLineEdit->setText(output);
}
