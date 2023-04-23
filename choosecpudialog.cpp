#include "choosecpudialog.h"
#include "ui_choosecpudialog.h"
#include <QFile>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
//#include "cpu.h"
#include <QSqlQuery>
#include <QSqlTableModel>

ChooseCPUDialog::ChooseCPUDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseCPUDialog)
{
    ui->setupUi(this);

    init_db();

    init_gui();
}

ChooseCPUDialog::~ChooseCPUDialog()
{
    qDebug() << "ChooseCPUDialog::~ChooseCPUDialog()";
    delete ui;
}

void ChooseCPUDialog::setFltr(const QString &fltrName)
{
    QSqlQuery query;
    query.exec("SELECT chipset FROM motherboard WHERE motherboard_id = "
               + fltrName.left(1) + ";");
    QString chipset;
    if (query.next()) {
        chipset = query.value(0).toString();
        qDebug() << "CHIPSET: " << chipset;
    } else {
        qDebug() << "CHIPSET: -";
    }

    // Set the filter
    cpuModel->setFilter("socket IN (SELECT socket FROM motherboard WHERE motherboard_id = "
                        + fltrName.left(1) + ") AND chipset = '" + chipset + "';");
    // re-select data from the table, taking into account the filter
    cpuModel->select();

    ui->comboBox->clear();
    //QSqlQuery query;
    query.exec("SELECT * "
               "FROM cpu "
               "WHERE socket IN ( "
               "SELECT socket "
               "FROM motherboard "
               "WHERE motherboard_id = " + fltrName.left(1) +
               ") AND chipset = '" +
               chipset + "';");
    while (query.next()) {
        QString model = query.value(2).toString();

        ui->comboBox->addItem(model);
    }
}

void ChooseCPUDialog::on_buttonBox_accepted()
{
    emit sendCPUName(ui->comboBox->itemText(ui->comboBox->currentIndex()));
    accept();
}

void ChooseCPUDialog::on_buttonBox_rejected()
{
    reject();
}

void ChooseCPUDialog::init_db()
{
    QString filePath("/Users/mykytaserebriakov/QtProjects2023/0-0DiplomaDemo/testDB.db");
    if (QFile::exists(filePath)) {
        qDebug() << "Success! Database file exists!";
        //QMessageBox::information(this, "Connection Info", "Success! Database file exists!");

        // Create the connection object.
        db = QSqlDatabase::addDatabase("QSQLITE");
        // Only need to set the databaseName argument to the path of the SQLite database
        // file on a local machine, and do not need to set the hostName, userName,
        // or password arguments.
        db.setDatabaseName(filePath);
        if (db.open())
            qDebug() << "Database connection established!";
        else
            qDebug() << "Failed to connect to database: " << db.lastError().text();
    } else {
        qDebug() << "Error! Database file does not exist!";
        //QMessageBox::critical(this, "Connection Info", "Error! Database file does not exist!");
        exit(EXIT_FAILURE);
    }
}

void ChooseCPUDialog::init_gui()
{
    ui->lblAvailableCPUs->setText("Available CPUs");
    ui->lblAvailableCPUs->setStyleSheet("color: lime");
    ui->lblAvailableCPUs->setAlignment(Qt::AlignHCenter);

    ui->label->setText("Please select a CPU");
    ui->label->setAlignment(Qt::AlignHCenter);

//    QSqlQuery query;
//    query.exec("SELECT * "
//               "FROM cpu "
//               "WHERE socket IN ( "
//               "SELECT socket "
//               "FROM motherboard "
//               "WHERE motherboard_id = 1 "
//               ");");
//    query.exec("SELECT * FROM cpu");
//    while (query.next()) {
//        QString model = query.value(2).toString();

//        ui->comboBox->addItem(model);
//    }

    // Create the model and set its table name.
    cpuModel = new QSqlTableModel;
    cpuModel->setTable("cpu");

    // must call select() to populate the model with data
    cpuModel->select();
    ui->tableView->setModel(cpuModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setMinimumSize(640, 480);
    // Set read-only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // No editing possible.

    setAttribute(Qt::WA_DeleteOnClose);
}

