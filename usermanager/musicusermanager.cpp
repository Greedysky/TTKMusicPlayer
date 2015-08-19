#include "musicusermanager.h"
#include "MusicUIObject.h"
#include "ui_musicusermanager.h"
#include "musicusermodel.h"
#include "musicuserconfigmanager.h"
#include <QDebug>

MusicUserManager::MusicUserManager(QWidget *parent)
     : QDialog(parent),ui(new Ui::MusicUserManager)
{
    ui->setupUi(this);
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint);

    m_userModel = new MusicUserModel(this);
    ui->userIcon->setStyleSheet("image:url(:/share/defaultArt)");
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->logoffButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->modifiedName->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(musicUserCancel()));
    connect(ui->logoffButton,SIGNAL(clicked()),SLOT(musicUserLogoff()));
    connect(ui->modifiedName,SIGNAL(clicked()),SLOT(modifiedUserName()));
}

MusicUserManager::~MusicUserManager()
{
    delete m_userModel;
    delete ui;
}

bool MusicUserManager::checkUser(const QString& uid)
{
    return m_userModel->databaseSelectedFilter(uid);
}

void MusicUserManager::setUserName(const QString& name)
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
        return;
    MStringsListMap parameter;
    xml.readUserConfig(parameter);
    QStringList values = parameter.value(m_currentUser);
    values[0] = "0";//auto login flag
    parameter.insert(m_currentUser,values);
    xml.writeUserXMLConfig(parameter);

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

void MusicUserManager::createUserTime()
{
    qlonglong time = m_userModel->getUserLogTime(m_currentUser).toLongLong();
    ui->totalTimeLabel->setText(QString(tr("%1day %2hour %3min"))
       .arg(time/86400).arg((time%86400)/3600).arg((time%86400%3600)/60));
}

void MusicUserManager::createUserLevel()
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
