#include "musicusermanagerdialog.h"
#include "ui_musicusermanagerdialog.h"
#include "musicuiobject.h"
#include "musicusermodel.h"
#include "musicuserconfigmanager.h"
#include "musicuserrecordwidget.h"
#include "musicnumberdefine.h"

#include <QMenu>

MusicUserManagerDialog::MusicUserManagerDialog(QWidget *parent)
     : QDialog(parent),
       ui(new Ui::MusicUserManagerDialog)
{
    ui->setupUi(this);
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    createButtonPopMenu();
    m_userModel = new MusicUserModel(this);

    connect(ui->logoffButton, SIGNAL(clicked()), SLOT(musicUserLogoff()));
    connect(ui->userIcon, SIGNAL(clicked()), SLOT(popupUserRecordWidget()));
    connect(ui->username, SIGNAL(clicked()), SLOT(popupUserRecordWidget()));
}

MusicUserManagerDialog::~MusicUserManagerDialog()
{
    if(!m_currentUserUID.isEmpty())
    {
        m_userModel->updateUser(m_currentUserUID, QString(), QString(), ui->username->text(),
                                QString::number(m_userModel->getUserLogTime(m_currentUserUID)
                                .toLongLong() + m_time.elapsed()/(MT_S2MS*30) ));
    }
    delete m_userModel;
    delete ui;
}

QString MusicUserManagerDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicUserManagerDialog::setUserUID(const QString &uid)
{
    m_currentUserUID = uid;
    ui->username->setText(m_userModel->getUserName(uid));
    ui->userIcon->setPixmap(QPixmap(m_userModel->getUserIcon(uid)).scaled(ui->userIcon->size()));
    createUserTime();
    m_time.start();
}

void MusicUserManagerDialog::createUserTime() const
{
    qlonglong time = m_userModel->getUserLogTime(m_currentUserUID).toLongLong();
    ui->totalTimeLabel->setText(QString::number(time));
}

void MusicUserManagerDialog::createButtonPopMenu()
{
    m_popMenu.addAction(tr("Modifies"), this, SLOT(popupUserRecordWidget()));
    m_popMenu.addAction(tr("Switches"), this, SLOT(musicUserLogoff()));
    m_popMenu.addAction(tr("Spacing"));
    m_popMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    ui->musicSettingButton->setMenu(&m_popMenu);
}

void MusicUserManagerDialog::musicUserLogoff()
{
    m_userModel->updateUser(m_currentUserUID, QString(), QString(), ui->username->text(),
                            QString::number(m_userModel->getUserLogTime(m_currentUserUID)
                            .toLongLong() + m_time.elapsed()/(MT_S2MS*30) ));

    MusicUserConfigManager xml;
    if(!xml.readUserXMLConfig())
    {
        return;
    }
    MusicUserRecords records;
    xml.readUserConfig( records );

    int index = -1;
    for(int i=0; i<records.count(); ++i)
    {
        if(records[i].m_name == m_currentUserUID)
        {
            index = i;
        }
    }
    if(index != -1)
    {
        records[index].m_al = "0";  //auto login flag
    }
    xml.writeUserXMLConfig( records );

    m_currentUserUID.clear();
    emit userStateChanged(QString(), QString());
    close();
}

int MusicUserManagerDialog::exec()
{
    QWidget *pa = MStatic_cast(QWidget*, parent());
    QPoint point = pa->mapToGlobal(QPoint(0, 0));
    move(point.x(), point.y() + 27);
    return QDialog::exec();
}

void MusicUserManagerDialog::leaveEvent(QEvent *event)
{
    QDialog::leaveEvent(event);
    close();
}

void MusicUserManagerDialog::popupUserRecordWidget()
{
#ifndef MUSIC_GREATER_NEW
    close();
#endif
    MusicUserRecordWidget record;
    connect(&record, SIGNAL(resetUserName(QString)), SLOT(resetUserName(QString)));
    connect(&record, SIGNAL(userIconChanged(QString,QString)),
                     SIGNAL(userStateChanged(QString,QString)));
    record.setUserModel(m_userModel, m_currentUserUID);
    record.exec();
}

void MusicUserManagerDialog::resetUserName(const QString &name)
{
    ui->username->setText(name);
}
