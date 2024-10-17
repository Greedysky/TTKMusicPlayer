#include "musicadvancedsearchedwidget.h"
#include "musicabstractqueryrequest.h"
#include "musicitemsearchedit.h"
#include "musicrightareawidget.h"
#include "musicsongsharingwidget.h"
#include "musictoastlabel.h"
#include "musicwidgetheaders.h"
#include "ttktabbutton.h"

MusicAdvancedSearchedWidget::MusicAdvancedSearchedWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(TTK::UI::BackgroundStyle10);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);

    m_tabWidget = new TTKTabButton(mainWidget);
    m_tabWidget->addButtons({tr("Song"), tr("Artist"), tr("Album"), tr("Playlist"), tr("Movie")});
    connect(m_tabWidget, SIGNAL(clicked(int)), SLOT(typeIndexChanged(int)));

    QWidget *containerWidget = new QWidget(mainWidget);
    containerWidget->setFixedHeight(250);
    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    m_searchLabel = new QLabel(containerWidget);
    m_searchLabel->setStyleSheet(TTK::UI::FontStyle04);
    m_searchEdit = new MusicItemSearchEdit(containerWidget);
    m_searchEdit->setFixedHeight(40);

    containerLayout->setSpacing(10);
    containerLayout->addStretch(1);
    containerLayout->addWidget(m_searchLabel);
    containerLayout->addWidget(m_searchEdit);
    containerLayout->addStretch(1);
    containerWidget->setLayout(containerLayout);

    mainLayout->addStretch(1);
    mainLayout->addWidget(m_tabWidget);
    mainLayout->addWidget(containerWidget);

    QPushButton *button = new QPushButton(tr("Search"), mainWidget);
    button->setFixedSize(100, 35);
    button->setStyleSheet(TTK::UI::PushButtonStyle03);
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button, SIGNAL(clicked()), SLOT(searchButtonClicked()));
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif
    mainLayout->addWidget(button, 0, Qt::AlignCenter);
    mainLayout->addStretch(1);

    typeIndexChanged(0);
}

MusicAdvancedSearchedWidget::~MusicAdvancedSearchedWidget()
{
    delete m_tabWidget;
}

void MusicAdvancedSearchedWidget::typeIndexChanged(int index)
{
    switch(index)
    {
        case 0:
        {
            m_searchLabel->setText(tr("Input song id or url link"));
            switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
            {
                case MusicAbstractQueryRequest::QueryServer::WY: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(WY_SG_SHARE, MDII_SHR_KEY, false).arg("28830412")); break;
                case MusicAbstractQueryRequest::QueryServer::KW: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KW_SG_SHARE, MDII_SHR_KEY, false).arg("23983067")); break;
                case MusicAbstractQueryRequest::QueryServer::KG: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KG_SG_SHARE, MDII_SHR_KEY, false).arg("f9b7637d5a482015b9bdb4fbea704e8e")); break;
                default: break;
            }
            break;
        }
        case 1:
        {
            m_searchLabel->setText(tr("Input artist id or url link"));
            switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
            {
                case MusicAbstractQueryRequest::QueryServer::WY: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(WY_AR_SHARE, MDII_SHR_KEY, false).arg("964486")); break;
                case MusicAbstractQueryRequest::QueryServer::KW: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KW_AR_SHARE, MDII_SHR_KEY, false).arg("373")); break;
                case MusicAbstractQueryRequest::QueryServer::KG: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KG_AR_SHARE, MDII_SHR_KEY, false).arg("832532")); break;
                default: break;
            }
            break;
        }
        case 2:
        {
            m_searchLabel->setText(tr("Input album id or url link"));
            switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
            {
                case MusicAbstractQueryRequest::QueryServer::WY: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(WY_AL_SHARE, MDII_SHR_KEY, false).arg("2901140")); break;
                case MusicAbstractQueryRequest::QueryServer::KW: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KW_AL_SHARE, MDII_SHR_KEY, false).arg("2698608")); break;
                case MusicAbstractQueryRequest::QueryServer::KG: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KG_AL_SHARE, MDII_SHR_KEY, false).arg("41169925")); break;
                default: break;
            }
            break;
        }
        case 3:
        {
            m_searchLabel->setText(tr("Input playlist id or url link"));
            switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
            {
                case MusicAbstractQueryRequest::QueryServer::WY: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(WY_PL_SHARE, MDII_SHR_KEY, false).arg("988604820")); break;
                case MusicAbstractQueryRequest::QueryServer::KW: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KW_PL_SHARE, MDII_SHR_KEY, false).arg("3565578939")); break;
                case MusicAbstractQueryRequest::QueryServer::KG: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KG_PL_SHARE, MDII_SHR_KEY, false).arg("3339907")); break;
                default: break;
            }
            break;
        }
        case 4:
        {
            m_searchLabel->setText(tr("Input movie id or url link"));
            switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
            {
                case MusicAbstractQueryRequest::QueryServer::WY: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(WY_MV_SHARE, MDII_SHR_KEY, false).arg("5343487")); break;
                case MusicAbstractQueryRequest::QueryServer::KW: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KW_MV_SHARE, MDII_SHR_KEY, false).arg("156505081")); break;
                case MusicAbstractQueryRequest::QueryServer::KG: m_searchEdit->editor()->setPlaceholderText(TTK::Algorithm::mdII(KG_MV_SHARE, MDII_SHR_KEY, false).arg("g9ai88")); break;
                default: break;
            }
            break;
        }
        default: break;
    }
}

void MusicAdvancedSearchedWidget::searchButtonClicked()
{
    const int server = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    if(server > 3 || server < 0)
    {
        MusicToastLabel::popup(tr("Current server is not support search"));
        return;
    }

    const QString &key = !m_searchEdit->editor()->text().isEmpty() ? m_searchEdit->editor()->text() : m_searchEdit->editor()->placeholderText();
    switch(m_tabWidget->currentIndex())
    {
        case 0: MusicRightAreaWidget::instance()->showSingleSearchedFound(searchedKeyWork(0, key)); break;
        case 1: MusicRightAreaWidget::instance()->showArtistFound({}, searchedKeyWork(1, key)); break;
        case 2: MusicRightAreaWidget::instance()->showAlbumFound({}, searchedKeyWork(2, key)); break;
        case 3: MusicRightAreaWidget::instance()->showPlaylistFound(searchedKeyWork(3, key)); break;
        case 4: MusicRightAreaWidget::instance()->showVideoSearchedFound({}, searchedKeyWork(4, key)); break;
        default: break;
    }
}

QString MusicAdvancedSearchedWidget::searchedKeyWork(int type, const QString &url)
{
    QString key;
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
    {
        case MusicAbstractQueryRequest::QueryServer::WY:
        {
            const QRegExp regx("id=(\\d+)");
            key = (regx.indexIn(url) != -1) ? regx.cap(1) : url;
            break;
        }
        case MusicAbstractQueryRequest::QueryServer::KW:
        {
            QRegExp regx;
            switch(type)
            {
                case 1: regx.setPattern("id=(\\d+)"); break;
                case 3: regx.setPattern("pid=(\\d+)"); break;
                default: regx.setPattern("/(\\d+)"); break;
            }

            key = (regx.indexIn(url) != -1) ? regx.cap(1) : url;
            break;
        }
        case MusicAbstractQueryRequest::QueryServer::KG:
        {
            QRegExp regx;
            switch(type)
            {
                case 0: regx.setPattern("hash=(\\w+)"); break;
                case 4: regx.setPattern("/mv_(\\w+)"); break;
                default: regx.setPattern("/(\\d+)"); break;
            }

            key = (regx.indexIn(url) != -1) ? regx.cap(1) : url;
            break;
        }
        default: break;
    }
    return key;
}
