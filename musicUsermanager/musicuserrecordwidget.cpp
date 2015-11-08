#include "musicuserrecordwidget.h"
#include "ui_musicuserrecordwidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"
#include "musicusermodel.h"
#include <QDebug>
MusicUserRecordWidget::MusicUserRecordWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicUserRecordWidget)
{
    ui->setupUi(this);
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
    ui->cityComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->countryComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->signatureEdit_F->setStyleSheet(MusicUIObject::MTextEditStyle01);
    connect(ui->cityComboBox_F, SIGNAL(currentIndexChanged(QString)),
                                SLOT(cityComboBoxIndexChanged(QString)));
    ui->cityComboBox_F->addItems(m_userModel->getAllCities());

    ui->userTimeLabel_F->setText(m_userModel->getUserLogTime(uid));
    ui->nicknameEdit->setText(m_userModel->getUserName(uid));
    ui->userIDLabel_F->setText(uid);

    QString string(m_userModel->getUserSex(uid));
    ui->maleRadioButton_F->setChecked(string.isEmpty() | string == "0");
    ui->femaleRadioButton_F->setChecked(string == "1");

    string = m_userModel->getUserBirthday(uid);
    ui->birthDateEdit_F->setDisplayFormat(QString("yyyy-MM-dd"));
    ui->birthDateEdit_F->setDate(string.isEmpty() ? QDate::currentDate()
                               : QDate::fromString(string, QString("yyyy-MM-dd")));
    string = m_userModel->getUserCity(uid);
    if(!string.isEmpty())
    {
        ui->cityComboBox_F->setCurrentText(string);
    }

    string = m_userModel->getUserCountry(uid);
    if(!string.isEmpty())
    {
        ui->countryComboBox_F->setCurrentText(string);
    }
    ui->signatureEdit_F->setText(m_userModel->getUserSignature(uid));

    connect(ui->confirmButton_F, SIGNAL(clicked()), SLOT(confirmButtonClickedF()));
}

void MusicUserRecordWidget::initTabS()
{

}

void MusicUserRecordWidget::initTabT()
{

}

void MusicUserRecordWidget::cityComboBoxIndexChanged(const QString &city)
{
    QStringList country = m_userModel->getAllCounties(city);
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

int MusicUserRecordWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();;
}
