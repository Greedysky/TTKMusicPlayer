#include "musicusermanager.h"
#include "MusicUIObject.h"
#include "ui_musicusermanager.h"
#include "musicusermodel.h"
#include "musicuserconfigmanager.h"

MusicUserManager::MusicUserManager(QWidget *parent)
     : QDialog(parent),ui(new Ui::MusicUserManager)
{
    ui->setupUi(this);
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint);

    m_userModel = new MusicUserModel(this);
    ui->userIcon->setStyleSheet(MusicUIObject::MCustomStyle28);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->logoffButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->modifiedName->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(musicUserCancel()));
    connect(ui->logoffButton,SIGNAL(clicked()),SLOT(musicUserLogoff()));
    connect(ui->modifiedName,SIGNAL(clicked()),SLOT(modifiedUserName()));
}

MusicUserManager::~MusicUserManager()
{
    delete m_userModel;
    delete ui;
}

bool MusicUserManager::checkUser(const QString &uid) const
{
    return m_userModel->databaseSelectedFilter(uid);
}

void MusicUserManager::setUserName(const QString &name)
{
    m_currentUser = name;
    ui->username->setText(m_userModel->getUserName(m_currentUser));
    ui->username->setEnabled(false);
    createUserTime();
    createUserLevel();
    m_time.start();
}

void MusicUserManager::musicUserLogoff()
{
    m_userModel->updateUser(m_currentUser,"","",ui->username->text(),
               QString::number(m_userModel->getUserLogTime(m_currentUser)
                               .toLongLong() + m_time.elapsed()/1000 ));

    MusicUserConfigManager xml;
    if(!xml.readUserXMLConfig())
    {
        return;
    }
    MusicUserRecord record;
    xml.readUserConfig( record );
    int index = record.m_names.indexOf(m_currentUser);
    record.m_als[index] = "0";  //auto login flag
    xml.writeUserXMLConfig( record );

    m_currentUser.clear();
    emit userStateChanged("");
    close();
}

void MusicUserManager::musicUserCancel()
{
    ui->username->setEnabled(false);
    close();
}

void MusicUserManager::modifiedUserName()
{
    ui->username->setEnabled(!ui->username->isEnabled());
}

void MusicUserManager::createUserTime() const
{
    qlonglong time = m_userModel->getUserLogTime(m_currentUser).toLongLong();
    ui->totalTimeLabel->setText(QString(tr("%1day %2hour %3min"))
       .arg(time/86400).arg((time%86400)/3600).arg((time%86400%3600)/60));
}

void MusicUserManager::createUserLevel() const
{
    qlonglong time = m_userModel->getUserLogTime(m_currentUser).toLongLong();
    QString level;
    if(time <= 100) level = tr("level1");
    else if(100 < time && time <= 300) level = tr("level2");
    else if(300 < time && time <= 1800) level = tr("level3");
    else if(1800 < time && time <= 4200) level = tr("level4");
    else if(4200 < time && time <= 20000) level = tr("level5");
    else if(20000 < time && time <= 86400) level = tr("level6");
    else if(86400 < time && time <= 1728000) level = tr("level7");
    else if(time > 1728000) level = tr("level8");
    ui->totalLevelLabel->setText(level);
}
