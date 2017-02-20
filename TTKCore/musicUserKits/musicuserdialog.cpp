#include "musicuserdialog.h"
#include "musicuiobject.h"
#include "ui_musicuserdialog.h"
#include "musicuserlineedit.h"
#include "musicusermodel.h"
#include "musicmessagebox.h"
#include "musictime.h"

#include <QValidator>
#include <QButtonGroup>
#include <QStyledItemDelegate>

MusicUserDialog::MusicUserDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicUserDialog)
{
    m_ui->setupUi(this);

    m_userModel = new MusicUserModel(this);

    MusicTime::timeSRand();
    changeVerificationCode();

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    firstStatckWidget();
    secondStatckWidget();
    thirdStatckWidget();

    m_ui->userComboBox->addItems(m_userModel->getAllUsers());
    connect(m_ui->userComboBox, SIGNAL(currentIndexChanged(QString)),
                              SLOT(userComboBoxChanged(QString)));
    connect(m_ui->userComboBox, SIGNAL(editTextChanged(QString)),
                              SLOT(userEditTextChanged(QString)));
    m_userName = m_ui->userComboBox->currentText();
    readFromUserConfig();

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_ui->useTencentLogin, 0);
    buttonGroup->addButton(m_ui->useRenrenLogin, 1);
    buttonGroup->addButton(m_ui->useWechatLogin, 2);
    buttonGroup->addButton(m_ui->useSinaLogin, 3);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
}

MusicUserDialog::~MusicUserDialog()
{
    delete m_userModel;
    delete m_ui;
}

QString MusicUserDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicUserDialog::readFromUserConfig()
{
    MusicUserConfigManager xml;
    if(!xml.readUserXMLConfig())
    {
        return;
    }
    xml.readUserConfig(m_records);
    readFromUserSettings();
}

void MusicUserDialog::writeToUserConfig()
{
    MusicUserConfigManager xml;
    writeToUserSettings();
    xml.writeUserXMLConfig(m_records);
}

void MusicUserDialog::firstStatckWidget()
{
    m_ui->userComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->userComboBox));
    m_ui->userComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->userComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->passwLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->loginButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->forgotPwdButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    m_ui->registerButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    m_ui->registerButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->forgotPwdButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->loginButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->registerButton, SIGNAL(clicked()), SLOT(registerUser()));
    connect(m_ui->forgotPwdButton, SIGNAL(clicked()), SLOT(userForgotPasswd()));
    connect(m_ui->loginButton, SIGNAL(clicked()), SLOT(checkUserLogin()));
}

void MusicUserDialog::secondStatckWidget()
{
    m_ui->registerUserLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->registerMailLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->registerPwdLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->registerPwdCLine->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->agreementButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(checkRegisterUser()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx, this);
    m_ui->registerUserLine->setValidator(valid);
    m_ui->registerMailLine->setValidator(valid);
    m_ui->registerPwdLine->setValidator(valid);
    m_ui->registerPwdCLine->setValidator(valid);

    m_ui->registerUserLine->setLabel(MusicUserLineEdit::User, m_ui->registerUserLineR, m_ui->labelRight);
    m_ui->registerMailLine->setLabel(MusicUserLineEdit::Mail, m_ui->registerMailLineR, m_ui->labelRigh_2);
    m_ui->registerPwdLine->setLabel(MusicUserLineEdit::Passwd, m_ui->registerPwdLineR, m_ui->labelRigh_3);
    m_ui->registerPwdCLine->setLabel(MusicUserLineEdit::PwdConfirm, m_ui->registerPwdCLineR, m_ui->labelRigh_4);
}

void MusicUserDialog::thirdStatckWidget()
{
    m_ui->userLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->pwdLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->mailLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->verificationCodeEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->confirmButton_2->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton_2->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    m_ui->confirmButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->confirmButton_2, SIGNAL(clicked()), SLOT(checkUserForgotPasswd()));
    connect(m_ui->cancelButton_2, SIGNAL(clicked()), SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx, this);
    m_ui->userLineEdit->setValidator(valid);
    m_ui->pwdLineEdit->setValidator(valid);
    m_ui->mailLineEdit->setValidator(valid);

    m_ui->pwdLineEdit->setLabel(MusicUserLineEdit::Passwd, m_ui->pwdLineEditR, m_ui->labelRigh_5);
    connect(m_ui->verificationCode, SIGNAL(clicked()), SLOT(changeVerificationCode()));
}

void MusicUserDialog::clearOriginData()
{
    m_ui->registerUserLineR->hide();
    m_ui->registerMailLineR->hide();
    m_ui->registerPwdLineR->hide();
    m_ui->registerPwdCLineR->hide();
    m_ui->labelRight->hide();
    m_ui->labelRigh_2->hide();
    m_ui->labelRigh_3->hide();
    m_ui->labelRigh_4->hide();
    m_ui->pwdLineEditR->hide();
    m_ui->labelRigh_5->hide();

    m_ui->passwLineEdit->clear();
    m_ui->registerUserLine->clear();
    m_ui->registerMailLine->clear();
    m_ui->registerPwdLine->clear();
    m_ui->registerPwdCLine->clear();
    m_ui->userLineEdit->clear();
    m_ui->pwdLineEdit->clear();
    m_ui->mailLineEdit->clear();
    m_ui->verificationCodeEdit->clear();

    m_ui->automaticLogon->setChecked(false);
    m_ui->rememberPwd->setChecked(false);
#ifdef MUSIC_GREATER_NEW
    m_ui->userComboBox->setCurrentText(QString());
#else
    m_ui->userComboBox->setCurrentIndex(-1);
#endif
}

void MusicUserDialog::changeVerificationCode()
{
    m_ui->verificationCode->setCodeCount(6);
    m_ui->verificationCode->setNoisyPointCount(100);
    m_ui->verificationCode->renderPicture();
}

void MusicUserDialog::userLogin()
{
    windowRectChanged(0, QRect(20, 45, 331, 181));
}

void MusicUserDialog::registerUser()
{
    windowRectChanged(1, QRect(20, 45, 331, 300));
}

void MusicUserDialog::userForgotPasswd()
{
    windowRectChanged(2, QRect(20, 45, 331, 251));
}

void MusicUserDialog::windowRectChanged(int index, const QRect &rect)
{
    clearOriginData();
    m_ui->stackedWidget->setCurrentIndex(index);
    m_ui->stackedWidget->setGeometry(rect);
    QRect other = geometry();
    other.setHeight(rect.height() + 61);
    setGeometry(other);
}

void MusicUserDialog::checkUserLogin()
{
    QString user = m_ui->userComboBox->currentText();
    QString pwd = m_ui->passwLineEdit->text();

    if(!m_ui->rememberPwd->isChecked() ||
       pwd != m_userModel->getUserPWDMD5(m_userName) )
    {
        if( !m_userModel->passwordCheck(user, pwd) )
        {
            MusicMessageBox message;
            message.setText(tr("You passwd is incorrect or user is not exist"));
            message.exec();
            return;
        }
    }
    if( user.trimmed().isEmpty() || pwd.trimmed().isEmpty() )
    {
        MusicMessageBox message;
        message.setText(tr("You entered is incorrect"));
        message.exec();
        return;
    }
    writeToUserConfig();

    emit userLoginSuccess(user, m_userModel->getUserIcon(user));
    close();
}

void MusicUserDialog::checkRegisterUser()
{
    if( m_ui->registerUserLine->getStrStatus() &&
        m_ui->registerMailLine->getMailStatus() &&
        m_ui->registerPwdLine->getStrStatus() &&
        m_ui->registerPwdCLine->getStrStatus() )
    {
        if( m_ui->registerPwdLine->text() != m_ui->registerPwdCLine->text() )
        {
            MusicMessageBox message;
            message.setText(tr("The two passwords do not match"));
            message.exec();
            return;
        }
        if( !m_ui->agreementCheckBox->isChecked() )
        {
            MusicMessageBox message;
            message.setText(tr("The agreement does not tick"));
            message.exec();
            return;
        }
        if( !m_userModel->addUser(m_ui->registerUserLine->text(),
                                  m_ui->registerPwdLine->text(),
                                  m_ui->registerMailLine->text()) )
        {
            MusicMessageBox message;
            message.setText(tr("The username is existed"));
            message.exec();
            return;
        }
        MusicMessageBox message;
        message.setText(tr("The register successfully"));
        message.exec();

        userLogin();
    }
    else
    {
        MusicMessageBox message;
        message.setText(tr("You entered is incorrect"));
        message.exec();
    }
}

void MusicUserDialog::checkUserForgotPasswd()
{
     QString user = m_ui->userLineEdit->text();
     QString mail = m_ui->mailLineEdit->text();
     if( user.trimmed().isEmpty() || mail.trimmed().isEmpty() )
     {
         MusicMessageBox message;
         message.setText(tr("You entered is incorrect"));
         message.exec();
         return;
     }
     if( !m_userModel->mailCheck(user,mail) )
     {
         MusicMessageBox message;
         message.setText(tr("You mail is incorrect or user is not exist"));
         message.exec();
         return;
     }
     if( !m_ui->pwdLineEdit->getStrStatus() )
     {
         MusicMessageBox message;
         message.setText(tr("You passwd is incorrect"));
         message.exec();
         return;
     }
     if( m_ui->verificationCode->getCheckCode() != m_ui->verificationCodeEdit->text().trimmed() )
     {
         MusicMessageBox message;
         message.setText(tr("You verificationCode is incorrect"));
         message.exec();
         return;
     }
     if( m_userModel->updateUser(user, m_ui->pwdLineEdit->text(), mail, QString(), QString()))
     {
         MusicMessageBox message;
         message.setText(tr("Change password successfully"));
         message.exec();
     }
     userLogin();
}

int MusicUserDialog::findUserNameIndex(const QString &name)
{
    int index = -1;
    for(int i=0; i<m_records.count(); ++i)
    {
        if(m_records[i].m_name == name)
        {
            return i;
        }
    }
    return index;
}

void MusicUserDialog::readFromUserSettings()
{
    int index = findUserNameIndex(m_userName);
    if(index != -1)
    {
        m_ui->automaticLogon->setChecked( !(m_records[index].m_al == "0") );
        m_ui->rememberPwd->setChecked( !(m_records[index].m_rp == "0") );
        m_ui->passwLineEdit->setText( m_records[index].m_pwd );
    }
}

void MusicUserDialog::writeToUserSettings()
{
    int index = findUserNameIndex(m_userName);
    if(index != -1)
    {
        m_records[index].m_al = m_ui->automaticLogon->isChecked() ? "1" : "0";
        m_records[index].m_rp = m_ui->rememberPwd->isChecked() ? "1" : "0";
        m_records[index].m_pwd = m_ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userName)
                                                              : QString();
    }
    else
    {
        MusicUserRecord record;
        record.m_name = m_userName;
        record.m_al = (m_ui->automaticLogon->isChecked() ? "1" : "0");
        record.m_rp = (m_ui->rememberPwd->isChecked() ? "1" : "0");
        record.m_pwd = (m_ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userName) : QString() );
        m_records << record;
    }
}

void MusicUserDialog::userComboBoxChanged(const QString &name)
{
    m_userName = name;
    readFromUserSettings();
}

void MusicUserDialog::userEditTextChanged(const QString &name)
{
    if(m_userModel->getAllUsers().contains(name))
    {
        m_userName = name;
        readFromUserSettings();
    }
    else
    {
        m_ui->automaticLogon->setChecked(false);
        m_ui->rememberPwd->setChecked(false);
        m_ui->passwLineEdit->clear();
    }
}

void MusicUserDialog::checkToAutoLogin(QString &name, QString &icon)
{
    if(m_ui->automaticLogon->isChecked() && m_ui->rememberPwd->isChecked() &&
       m_ui->passwLineEdit->text() == m_userModel->getUserPWDMD5(m_userName))
    {
        name = m_userName;
        icon = m_userModel->getUserIcon(m_userName);
    }
}

void MusicUserDialog::buttonClicked(int)
{
    MusicMessageBox message;
    message.setText(tr("This way of loading is now not supported"));
    message.exec();
}
