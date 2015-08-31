#include "musicuserdialog.h"
#include "musicuiobject.h"
#include "ui_musicuserdialog.h"
#include "musicuserlineedit.h"
#include "musicuserModel.h"
#include "musicuserconfigmanager.h"
#include "musicmessagebox.h"

#include <QValidator>
#include <time.h>
#include <QTimer>

MusicUserDialog::MusicUserDialog(QWidget *parent) :
    MusicMoveDialogAbstract(parent),ui(new Ui::MusicUserDialog)
{
    ui->setupUi(this);

    m_userModel = new MusicUserModel(this);
    qsrand(time(NULL));
    changeVerificationCode();

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    firstStatckWidget();
    secondStatckWidget();
    thirdStatckWidget();
    drawRoundedRect();
    ui->userComboBox->addItems(m_userModel->getAllUsers());
    connect(ui->userComboBox, SIGNAL(currentIndexChanged(QString)),
                              SLOT(userComboBoxChanged(QString)));
    connect(ui->userComboBox, SIGNAL(editTextChanged(QString)),
                              SLOT(userEditTextChanged(QString)));
    m_userComboIndex = ui->userComboBox->currentText();
    readFromUserConfig();

    QTimer::singleShot(1, this, SLOT(checkToAutoLogin()));
}

MusicUserDialog::~MusicUserDialog()
{
    delete m_userModel;
    delete ui;
}

void MusicUserDialog::readFromUserConfig()
{
    MusicUserConfigManager xml;
    if(!xml.readUserXMLConfig())
    {
        return;
    }
    xml.readUserConfig(m_parameter);
    readFromUserSettings();
}

void MusicUserDialog::writeToUserConfig()
{
    MusicUserConfigManager xml;
    writeToUserSettings();
    xml.writeUserXMLConfig(m_parameter);
}

void MusicUserDialog::firstStatckWidget()
{
    ui->userComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->passwLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->loginButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->forgotPwdButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->registerButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    ui->registerButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->forgotPwdButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->registerButton,SIGNAL(clicked()),SLOT(registerUser()));
    connect(ui->forgotPwdButton,SIGNAL(clicked()),SLOT(userForgotPasswd()));
    connect(ui->loginButton,SIGNAL(clicked()),SLOT(checkUserLogin()));
}

void MusicUserDialog::secondStatckWidget()
{
    ui->registerUserLine->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->registerMailLine->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->registerPwdLine->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->registerPwdCLine->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->agreementButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->confirmButton,SIGNAL(clicked()),SLOT(checkRegisterUser()));
    connect(ui->cancelButton,SIGNAL(clicked()),SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx,this);
    ui->registerUserLine->setValidator(valid);
    ui->registerMailLine->setValidator(valid);
    ui->registerPwdLine->setValidator(valid);
    ui->registerPwdCLine->setValidator(valid);

    ui->registerUserLine->setLabel(User,ui->registerUserLineR,ui->labelRight);
    ui->registerMailLine->setLabel(Mail,ui->registerMailLineR,ui->labelRigh_2);
    ui->registerPwdLine->setLabel(Passwd,ui->registerPwdLineR,ui->labelRigh_3);
    ui->registerPwdCLine->setLabel(PasswdC,ui->registerPwdCLineR,ui->labelRigh_4);
}

void MusicUserDialog::thirdStatckWidget()
{
    ui->userLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->pwdLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->mailLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->verificationCodeEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->confirmButton_2->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton_2->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    ui->confirmButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton_2->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->confirmButton_2,SIGNAL(clicked()),SLOT(checkUserForgotPasswd()));
    connect(ui->cancelButton_2,SIGNAL(clicked()),SLOT(userLogin()));

    QRegExp userRx("[^\u4E00-\u9FA5]+$");
    QRegExpValidator *valid = new QRegExpValidator(userRx,this);
    ui->userLineEdit->setValidator(valid);
    ui->pwdLineEdit->setValidator(valid);
    ui->mailLineEdit->setValidator(valid);

    ui->pwdLineEdit->setLabel(Passwd,ui->pwdLineEditR,ui->labelRigh_5);
    ui->verificationCode->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    connect(ui->verificationCode,SIGNAL(clicked()),
                                 SLOT(changeVerificationCode()));
}

void MusicUserDialog::clearOriginData()
{
    ui->registerUserLineR->hide();
    ui->registerMailLineR->hide();
    ui->registerPwdLineR->hide();
    ui->registerPwdCLineR->hide();
    ui->labelRight->hide();
    ui->labelRigh_2->hide();
    ui->labelRigh_3->hide();
    ui->labelRigh_4->hide();
    ui->pwdLineEditR->hide();
    ui->labelRigh_5->hide();

    ui->passwLineEdit->clear();
    ui->registerUserLine->clear();
    ui->registerMailLine->clear();
    ui->registerPwdLine->clear();
    ui->registerPwdCLine->clear();
    ui->userLineEdit->clear();
    ui->pwdLineEdit->clear();
    ui->mailLineEdit->clear();
    ui->verificationCodeEdit->clear();
}

void MusicUserDialog::changeVerificationCode()
{
    ui->verificationCode->setText(QString::number(qrand()));
}

void MusicUserDialog::drawRoundedRect()
{
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
}

void MusicUserDialog::userLogin()
{
    clearOriginData();
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->setGeometry(1,35,329,180);
    QRect rect = geometry();
    rect.setHeight(216);
    this->setGeometry(rect);
    drawRoundedRect();
}

void MusicUserDialog::registerUser()
{
    clearOriginData();
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setGeometry(1,35,329,299);
    QRect rect = geometry();
    rect.setHeight(335);
    this->setGeometry(rect);
    drawRoundedRect();
}

void MusicUserDialog::userForgotPasswd()
{
    clearOriginData();
    ui->stackedWidget->setCurrentIndex(2);
    ui->stackedWidget->setGeometry(1,35,329,250);
    QRect rect = geometry();
    rect.setHeight(286);
    this->setGeometry(rect);
    drawRoundedRect();
}

void MusicUserDialog::checkUserLogin()
{
    QString user = ui->userComboBox->currentText();
    QString pwd = ui->passwLineEdit->text();

    if(!ui->rememberPwd->isChecked() ||
       pwd != m_userModel->getUserPWDMD5(m_userComboIndex) )
    {
        if( !m_userModel->checkUser(user,pwd) )
        {
            MusicMessageBox message;
            message.setText(tr("You passwd is incorrect or user is not exist"));
            message.exec();
            return;
        }
    }
    if( user.trimmed() == "" || pwd.trimmed() == "" )
    {
        MusicMessageBox message;
        message.setText(tr("You entered is incorrect"));
        message.exec();
        return;
    }
    writeToUserConfig();
    emit userLoginSuccess(user);
    close();
}

void MusicUserDialog::checkRegisterUser()
{
    if( ui->registerUserLine->getStrStatus() &&
        ui->registerMailLine->getMailStatus() &&
        ui->registerPwdLine->getStrStatus() &&
        ui->registerPwdCLine->getStrStatus() )
    {
        if( ui->registerPwdLine->text() != ui->registerPwdCLine->text() )
        {
            MusicMessageBox message;
            message.setText(tr("The two passwords do not match"));
            message.exec();
            return;
        }
        if( !ui->agreementCheckBox->isChecked() )
        {
            MusicMessageBox message;
            message.setText(tr("The agreement does not tick"));
            message.exec();
            return;
        }
        if( !m_userModel->addUser(ui->registerUserLine->text(),
                                 ui->registerPwdLine->text(),
                                 ui->registerMailLine->text()) )
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
     QString user = ui->userLineEdit->text();
     QString mail = ui->mailLineEdit->text();
     if( user.trimmed() == "" || mail.trimmed() == "" )
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
     if( !ui->pwdLineEdit->getStrStatus() )
     {
         MusicMessageBox message;
         message.setText(tr("You passwd is incorrect"));
         message.exec();
         return;
     }
     if( ui->verificationCode->text() !=
         ui->verificationCodeEdit->text().trimmed() )
     {
         MusicMessageBox message;
         message.setText(tr("You verificationCode is incorrect"));
         message.exec();
         return;
     }
     if( m_userModel->updateUser(user,ui->pwdLineEdit->text(),mail,"","") )
     {
         MusicMessageBox message;
         message.setText(tr("Change password successfully"));
         message.exec();
     }
     userLogin();
}

void MusicUserDialog::readFromUserSettings()
{
    MStringsListMapIt p(m_parameter);
    bool flag;
    while(p.hasNext())
    {
        p.next();
        if(p.key() == m_userComboIndex)
        {
            p.value()[0] == "0" ? flag = false : flag = true;
            ui->automaticLogon->setChecked(flag);
            p.value()[1] == "0" ? flag = false : flag = true;
            ui->rememberPwd->setChecked(flag);
            ui->passwLineEdit->setText(p.value()[2]);
            break;
        }
    }
}

void MusicUserDialog::writeToUserSettings()
{
    m_parameter.insert(m_userComboIndex,QStringList()
                        <<(ui->automaticLogon->isChecked() ? "1" : "0")
                        <<(ui->rememberPwd->isChecked() ? "1" : "0")
    <<(ui->rememberPwd->isChecked() ? m_userModel->getUserPWDMD5(m_userComboIndex)
                                    : QString() ));
}

void MusicUserDialog::userComboBoxChanged(const QString &index)
{
    m_userComboIndex = index;
    readFromUserSettings();
}

void MusicUserDialog::userEditTextChanged(const QString &index)
{
    if(m_userModel->getAllUsers().contains(index))
    {
        m_userComboIndex = index;
        readFromUserSettings();
    }
    else
    {
        ui->automaticLogon->setChecked(false);
        ui->rememberPwd->setChecked(false);
        ui->passwLineEdit->clear();;
    }
}

void MusicUserDialog::checkToAutoLogin()
{
    if(ui->automaticLogon->isChecked() &&
       ui->rememberPwd->isChecked() &&
       ui->passwLineEdit->text() == m_userModel->getUserPWDMD5(m_userComboIndex))
    {
        emit userLoginSuccess(m_userComboIndex);
        close();
    }
}
