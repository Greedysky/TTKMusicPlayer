#include "musicuserrecordwidget.h"
#include "ui_musicuserrecordwidget.h"
#include "musicbackgroundmanager.h"
#include "musicuiobject.h"
#include "musicusermodel.h"
#include "musicmessagebox.h"
#include "musicutils.h"

#include <QFileDialog>
#include <QCryptographicHash>
#include <QStyledItemDelegate>

MusicUserRecordWidget::MusicUserRecordWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicUserRecordWidget)
{
    ui->setupUi(this);

#ifdef Q_OS_UNIX
    MusicUtils::setLabelFont(ui->label_5T, 9);
    MusicUtils::setLabelFont(ui->label_6T, 9);
#endif
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
}

MusicUserRecordWidget::~MusicUserRecordWidget()
{
    delete ui;
}

void MusicUserRecordWidget::setUserModel(MusicUserModel *model, const QString &uid)
{
    m_userModel = model;
    ui->userIDLabel_F->setText(uid);
    ///first tab
    initTabF();
    ///second tab
    initTabS();
    ///third tab
    initTabT();
}

void MusicUserRecordWidget::initTabF()
{
    QString uid = ui->userIDLabel_F->text();
    ui->cityComboBox_F->setItemDelegate(new QStyledItemDelegate(ui->cityComboBox_F));
    ui->cityComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->countryComboBox_F->setItemDelegate(new QStyledItemDelegate(ui->countryComboBox_F));
    ui->countryComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->cityComboBox_F->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->countryComboBox_F->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->signatureEdit_F->setStyleSheet(MusicUIObject::MTextEditStyle01);
    connect(ui->cityComboBox_F, SIGNAL(currentIndexChanged(QString)),
                                SLOT(cityComboBoxIndexChanged(QString)));
    ui->cityComboBox_F->addItems(m_userModel->getAllCities());

    ui->userTimeLabel_F->setText(m_userModel->getUserLogTime(uid));
    ui->nicknameEdit->setText(m_userModel->getUserName(uid));
    ui->userIDLabel_F->setText(uid);

    QString string(m_userModel->getUserSex(uid));
    ui->maleRadioButton_F->setChecked(string.isEmpty() || string == "0");
    ui->femaleRadioButton_F->setChecked(string == "1");

    string = m_userModel->getUserBirthday(uid);
    ui->birthDateEdit_F->setDisplayFormat(QString("yyyy-MM-dd"));
    ui->birthDateEdit_F->setDate(string.isEmpty() ? QDate::currentDate()
                               : QDate::fromString(string, QString("yyyy-MM-dd")));
    string = m_userModel->getUserCity(uid);
    if(!string.isEmpty())
    {
#ifdef MUSIC_QT_5
        ui->cityComboBox_F->setCurrentText(string);
#else
        MusicUtils::setComboboxText(ui->cityComboBox_F, string);
#endif
    }

    string = m_userModel->getUserCountry(uid);
    if(!string.isEmpty())
    {
#ifdef MUSIC_QT_5
        ui->countryComboBox_F->setCurrentText(string);
#else
        MusicUtils::setComboboxText(ui->countryComboBox_F, string);
#endif
    }
    ui->signatureEdit_F->setText(m_userModel->getUserSignature(uid));

    connect(ui->confirmButton_F, SIGNAL(clicked()), SLOT(confirmButtonClickedF()));
}

void MusicUserRecordWidget::initTabS()
{
    QString path = m_userModel->getUserIcon(ui->userIDLabel_F->text());
    ui->bigPixmapLabel_S->setPixmap(QPixmap(path).scaled(ui->bigPixmapLabel_S->size()));
    ui->smlPixmapLabel_S->setPixmap(QPixmap(path).scaled(ui->smlPixmapLabel_S->size()));
    connect(ui->openFileButton_S, SIGNAL(clicked()), SLOT(openFileButtonClickedS()));
}

void MusicUserRecordWidget::initTabT()
{
    ui->labelRighT1->hide();
    ui->labelRighT2->hide();
    ui->labelRighT3->hide();
    ui->originPwdEdit_T->setLabel(MusicUserLineEdit::PwdConfirm, nullptr, ui->labelRighT1);
    ui->newPwdEdit_T->setLabel(MusicUserLineEdit::PasswdNew, nullptr, ui->labelRighT2);
    ui->newCPwdEdit_T->setLabel(MusicUserLineEdit::PwdConfirm, nullptr, ui->labelRighT3);
    connect(ui->newPwdEdit_T, SIGNAL(checkPwdStrength(int)), SLOT(checkPwdStrength(int)));

    changeVerificationCodeT();
    connect(ui->verificationCode, SIGNAL(clicked()), SLOT(changeVerificationCodeT()));
    connect(ui->confirmButton_T, SIGNAL(clicked()), SLOT(confirmButtonClickedT()));
}

void MusicUserRecordWidget::cityComboBoxIndexChanged(const QString &city)
{
    QStringList country = m_userModel->getAllCountries(city);
    ui->countryComboBox_F->clear();
    ui->countryComboBox_F->addItems(country);
}

void MusicUserRecordWidget::confirmButtonClickedF()
{
    QString nickname(ui->nicknameEdit->text());
    if(nickname != m_userModel->getUserName(ui->userIDLabel_F->text()))
    {
        emit resetUserName(ui->nicknameEdit->text());
    }
    m_userModel->updateUser(ui->userIDLabel_F->text(), nickname,
                            ui->maleRadioButton_F->isChecked() ? "0" : "1",
                            ui->birthDateEdit_F->text(),
                            ui->cityComboBox_F->currentText(),
                            ui->countryComboBox_F->currentText(),
                            ui->signatureEdit_F->toPlainText());
    close();
}

void MusicUserRecordWidget::openFileButtonClickedS()
{
    QString path =  QFileDialog::getOpenFileName(
                              this, QString(), "./", "Images (*.png *.bmp *.jpg)");
    if(path.isEmpty())
    {
        return;
    }
    ui->bigPixmapLabel_S->setPixmap(QPixmap( path ).scaled(ui->bigPixmapLabel_S->size()));
    ui->smlPixmapLabel_S->setPixmap(QPixmap( path ).scaled(ui->smlPixmapLabel_S->size()));

    QFile file(path);
    QByteArray name;
    if(file.open(QIODevice::ReadOnly))
    {
        name = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
    }
    path = QString("%1%2").arg(DATA_CACHED_AL)
                          .arg(QString(name.toHex().toUpper()));
    file.copy( path );
    file.close();

    m_userModel->updateUserIcon(ui->userIDLabel_F->text(), path);
    emit userIconChanged(ui->userIDLabel_F->text(), path);
}

void MusicUserRecordWidget::changeVerificationCodeT()
{
    ui->verificationCode->setCodeCount(6);
    ui->verificationCode->setNoisyPointCount(100);
    ui->verificationCode->renderPicture();
}

void MusicUserRecordWidget::confirmButtonClickedT()
{
    QString uid = ui->userIDLabel_F->text();
    if( ui->originPwdEdit_T->getStrStatus() &&
        ui->newPwdEdit_T->getStrStatus() &&
        ui->newCPwdEdit_T->getStrStatus() )
    {
        if(ui->verificationCode->getCheckCode() != ui->verificationCodeEdit_T->text().trimmed())
        {
            MusicMessageBox message;
            message.setText(tr("You verificationCode is incorrect"));
            message.exec();
            return;
        }
        if(!m_userModel->passwordCheck(uid, ui->originPwdEdit_T->text()))
        {
            MusicMessageBox message;
            message.setText(tr("The origin password does not match"));
            message.exec();
            return;
        }
        if(ui->newPwdEdit_T->text() != ui->newCPwdEdit_T->text())
        {
            MusicMessageBox message;
            message.setText(tr("The two passwords do not match"));
            message.exec();
            return;
        }
        if(!m_userModel->updateUserPwd(uid, ui->newPwdEdit_T->text()))
        {
            MusicMessageBox message;
            message.setText(tr("The modity password failed"));
            message.exec();
            return;
        }

        emit userIconChanged(QString(), QString());
        close();

        MusicMessageBox message;
        message.setText(tr("The modity password successfully, please relogin"));
        message.exec();
    }
    else
    {
        MusicMessageBox message;
        message.setText(tr("You entered is incorrect"));
        message.exec();
    }
}

void MusicUserRecordWidget::checkPwdStrength(int code)
{
    QString blue = "background:#80B7F1;";
    QString grey = "background:#BFBFBF;";

    ui->pwdStrengthT1->setStyleSheet(code != -1 ? blue : grey);
    ui->pwdStrengthT2->setStyleSheet(grey);
    ui->pwdStrengthT3->setStyleSheet(grey);

    switch(code)
    {
        case  1:
            ui->pwdStrengthT2->setStyleSheet(blue);
            break;
        case  2:
            ui->pwdStrengthT2->setStyleSheet(blue);
            ui->pwdStrengthT3->setStyleSheet(blue);
            break;
        default: break;
    }
}

int MusicUserRecordWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();;
}
