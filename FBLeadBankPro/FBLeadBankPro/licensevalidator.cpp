#include "licensevalidator.h"
#include "ui_licensevalidator.h"
#include <QMessageBox>
#include "MockPage/metashot.h"

LicenseValidator::LicenseValidator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LicenseValidator)
{
    ui->setupUi(this);
       QSettings s(MetaShot::ins()->appDataRoot()+"/lbp.s",QSettings::IniFormat);


    this->m_lText = s.value("LBPL").toString();
    if(!this->m_lText.isEmpty()){
        this->ui->licenseBox->setText(this->m_lText);
        this->on_validateButton_clicked();
    }

    this->connect(&this->m_lEngine,SIGNAL(invalidLicense()),this,SLOT(invalid()));
    this->connect(&this->m_lEngine,SIGNAL(validLicense()),this,SLOT(valid()));
}

LicenseValidator::~LicenseValidator()
{
    delete ui;
}

void LicenseValidator::valid()
{
    this->hide();
    this->m_window.show();

       QSettings s(MetaShot::ins()->appDataRoot()+"/lbp.s",QSettings::IniFormat);
    s.setValue("LBPL",this->m_lText);
    s.sync();
}

void LicenseValidator::invalid()
{
    this->ui->message->setText("");

    QMessageBox::information(this,"Error!!!","Invalid license.");
    this->ui->licenseBox->setHidden(false);
    this->ui->validateButton->setHidden(false);
    this->ui->licenseBox->setEnabled(true);
    this->ui->validateButton->setEnabled(true);
}

void LicenseValidator::on_validateButton_clicked()
{
    if(this->ui->licenseBox->text().isEmpty()){
        QMessageBox::information(this,"Error!!!","Please enter your license key");
        return;
    }
    else{
        this->ui->licenseBox->setHidden(true);
        this->ui->validateButton->setHidden(true);
        this->ui->licenseBox->setEnabled(false);
        this->ui->validateButton->setEnabled(false);
        this->m_lText = this->ui->licenseBox->text();
        this->m_lEngine.validateLicense(this->ui->licenseBox->text());
        this->ui->message->setText("Checking license......");
    }
}
