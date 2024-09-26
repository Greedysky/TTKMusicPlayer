#include "toolurl.h"
#include "ui_toolurl.h"
#include "musicalgorithmutils.h"

ToolUrl::ToolUrl(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::ToolUrl)
{
    m_ui->setupUi(this);

    setFixedSize(500, 300);

    connect(m_ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->encodeButton, SIGNAL(clicked()), SLOT(encodeClicked()));
    connect(m_ui->decodeButton, SIGNAL(clicked()), SLOT(decodeClicked()));
}

ToolUrl::~ToolUrl()
{
    delete m_ui;
}

void ToolUrl::encodeClicked()
{
    QString key = m_ui->keyLineEdit->text();
    if(key.isEmpty())
    {
        key = MDII_URL_KEY;
    }

    const QString &input = m_ui->inputLineEdit->text();
    const QString &output = TTK::Algorithm::mdII(input, key.toStdString().c_str(), true);
    m_ui->outPutLineEdit->setText(output);
}

void ToolUrl::decodeClicked()
{
    QString key = m_ui->keyLineEdit->text();
    if(key.isEmpty())
    {
        key = MDII_URL_KEY;
    }

    const QString &input = m_ui->inputLineEdit->text();
    const QString &output = TTK::Algorithm::mdII(input, key.toStdString().c_str(), false);
    m_ui->outPutLineEdit->setText(output);
}
