#include "musicuserrecordwidget.h"
#include "ui_musicuserrecordwidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"
#include "musicusermodel.h"

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

void MusicUserRecordWidget::setUserModel(MusicUserModel *model, const QString &id)
{
    m_userModel = model;
    initWidget(id);
}

void MusicUserRecordWidget::initWidget(const QString &id)
{
    ///first tab
    ui->cityComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->countryComboBox_F->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->signatureEdit_F->setStyleSheet(MusicUIObject::MTextEditStyle01);
    connect(ui->cityComboBox_F, SIGNAL(currentIndexChanged(QString)),
                                SLOT(cityComboBoxIndexChanged(QString)));
    ui->cityComboBox_F->addItems(m_userModel->getAllCities());

    ui->userTimeLabel_F->setText(m_userModel->getUserLogTime(id));
    ui->nicknameEdit->setText(m_userModel->getUserName(id));
    ui->userIDLabel_F->setText(id);
    ui->maleRadioButton_F->setChecked(true);
    ui->birthDateEdit_F->setDate(QDate::currentDate());
    connect(ui->confirmButton_F, SIGNAL(clicked()), SLOT(confirmButtonClickedF()));

    ///second tab

    ///third tab
}

void MusicUserRecordWidget::cityComboBoxIndexChanged(const QString &city)
{
    QStringList country = m_userModel->getAllCounties(city);
    ui->countryComboBox_F->clear();
    ui->countryComboBox_F->addItems(country);
}

void MusicUserRecordWidget::confirmButtonClickedF()
{
    close();
}

int MusicUserRecordWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();;
}
