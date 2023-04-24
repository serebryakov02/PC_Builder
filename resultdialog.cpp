#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);

    init_gui();
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setlblMBText(const QString &MB)
{
    ui->lblMB->setText(MB);
}

void ResultDialog::setlblCPUText(const QString &CPU)
{
    ui->lblCPU->setText(CPU);
}

void ResultDialog::setlblRAM1Text(const QString &RAM1)
{
    ui->lblRAM1->setText(RAM1);
}

void ResultDialog::setlblRAM2Text(const QString &RAM2)
{
    ui->lblRAM2->setText(RAM2);
}

void ResultDialog::setlblGPUText(const QString &GPU)
{
    ui->lblGPU->setText(GPU);
}

void ResultDialog::setlblPCI1Text(const QString &PCI1)
{
    ui->lblPCI1->setText(PCI1);
}

void ResultDialog::setlblPCI2Text(const QString &PCI2)
{
    ui->lblPCI2->setText(PCI2);
}

void ResultDialog::init_gui()
{
    ui->groupBox->setAlignment(Qt::AlignCenter);
    ui->groupBox->setTitle("Your choice");
}
