#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicsourceupdaterequest.h"
#include "musicsettingmanager.h"
#include "musicalgorithmutils.h"
#include "musiccoreutils.h"
#include "musicurlutils.h"

#include <QBoxLayout>

MusicSourceUpdateNotifyWidget::MusicSourceUpdateNotifyWidget(QWidget *parent)
    : MusicAbstractMovePlainWidget(parent)
{
    hide();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet(TTK::UI::BackgroundStyle10);
    setMouseTracking(true);
    blockOption(true);

    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    setGeometry(windowSize.width() - 240, windowSize.height() - 150, 220, 100);

    QVBoxLayout *vLayout = new QVBoxLayout(m_container);
    vLayout->setContentsMargins(5, 5, 5, 5);

    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(TTK::UI::ColorStyle03);
    m_textLabel->setAlignment(Qt::AlignCenter);

    QWidget *contain = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout(contain);
    hLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *updateButton = new QPushButton(tr("Update"), contain);
    QPushButton *nextTimeButton = new QPushButton(tr("Close"), contain);
    updateButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    nextTimeButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    updateButton->setFixedSize(85, 27);
    nextTimeButton->setFixedSize(85, 27);
    hLayout->addWidget(updateButton);
    hLayout->addWidget(nextTimeButton);
    contain->setLayout(hLayout);

#ifdef Q_OS_UNIX
    updateButton->setFocusPolicy(Qt::NoFocus);
    nextTimeButton->setFocusPolicy(Qt::NoFocus);
#endif

    vLayout->addWidget(m_textLabel);
    vLayout->addWidget(contain);
    m_container->setLayout(vLayout);

    connect(updateButton, SIGNAL(clicked()), SLOT(updateSourceClicked()));
    connect(nextTimeButton, SIGNAL(clicked()), SLOT(close()));

    MusicSourceUpdateRequest *req = new MusicSourceUpdateRequest(this);
    connect(req, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
    req->startToRequest();
}

MusicSourceUpdateNotifyWidget::~MusicSourceUpdateNotifyWidget()
{
    delete m_textLabel;
}

void MusicSourceUpdateNotifyWidget::updateSourceClicked()
{
    close();
    MusicSourceUpdateWidget().exec();
}

void MusicSourceUpdateNotifyWidget::downLoadFinished()
{
    MusicSourceUpdateRequest *req = TTKObjectCast(MusicSourceUpdateRequest*, sender());
    if(!req)
    {
        close();
        return;
    }

    const QString &version = req->version();
    if(TTK::Core::appVersionCheck(TTK_VERSION_STR, version))
    {
        show();
        m_textLabel->setText(tr("New version found") + TTK_WLINEFEED + version);
    }
    else
    {
        close();
    }
}



MusicSourceUpdateWidget::MusicSourceUpdateWidget(QWidget *parent)
    : TTKAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSourceUpdateWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->upgradeButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    m_ui->upgradeButton->setEnabled(false);
#ifdef Q_OS_UNIX
    m_ui->upgradeButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->upgradeButton, SIGNAL(clicked()), SLOT(upgradeButtonClicked()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
}

MusicSourceUpdateWidget::~MusicSourceUpdateWidget()
{
    delete m_ui;
}

void MusicSourceUpdateWidget::start()
{
    MusicSourceUpdateRequest *req = new MusicSourceUpdateRequest(this);
    connect(req, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
    req->startToRequest();
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
    TTK::Url::openUrl(TTK::Algorithm::mdII(RELEASE_DATA_URL, false), false);
}

void MusicSourceUpdateWidget::downLoadFinished()
{
    MusicSourceUpdateRequest *req = TTKObjectCast(MusicSourceUpdateRequest*, sender());
    if(!req)
    {
        return;
    }

    const QString &version = req->version();
    const QString &description = req->description();

    QString text;
    if(TTK::Core::appVersionCheck(TTK_VERSION_STR, version))
    {
        text.append("v" + version);
        text.append(TTK_WLINEFEED);
        text.append(description);

        m_ui->upgradeButton->setEnabled(true);
        m_ui->titleLable->move(50, 2);
        m_ui->titleLable->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    }
    else
    {
        text.append(tr("Current version is updated"));
        m_ui->titleLable->move(0, 0);
        m_ui->titleLable->setAlignment(Qt::AlignCenter);
    }

    m_ui->titleLable->setText(text);
}

int MusicSourceUpdateWidget::exec()
{
    start();
    return TTKAbstractMoveDialog::exec();
}
