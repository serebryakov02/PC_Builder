#include "choosembdialog.h"
#include "ui_choosembdialog.h"
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

ChooseMBDialog::ChooseMBDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseMBDialog)
{
    ui->setupUi(this);

    init_db();
    init_gui();
}

ChooseMBDialog::~ChooseMBDialog()
{
    delete ui;
}

int ChooseMBDialog::getMaxRAMValue(const QString &mbName)
{
    QSqlQuery query;
    query.exec("SELECT * FROM motherboard WHERE motherboard_id = " + mbName.left(1) +";");
    if (query.next()) {
        maxRAMValue = query.value(7).toInt();
    }

    return maxRAMValue;
}

int ChooseMBDialog::getMaxPCISlots(const QString &mbName)
{
    //int graphics_card_slot_quantity = 0;
    QSqlQuery query;
    query.exec("SELECT graphics_card_slot_quantity FROM motherboard WHERE motherboard_id = " + mbName.left(1) + ";");
    if (query.next()) {
        graphics_card_slot_quantity = query.value(0).toInt();
    }

    return graphics_card_slot_quantity;
}

void ChooseMBDialog::on_buttonBox_accepted()
{
    emit sendMBInfo(ui->comboBox->itemText(ui->comboBox->currentIndex()));
    accept();
}

void ChooseMBDialog::on_buttonBox_rejected()
{
    reject();
}

void ChooseMBDialog::init_db()
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

void ChooseMBDialog::init_gui()
{
    ui->lblAvailableMBs->setText("Available motherboards");
    ui->lblAvailableMBs->setStyleSheet("color: lime");
    ui->lblAvailableMBs->setAlignment(Qt::AlignHCenter);

    ui->label->setText("Please select a motherboard");
    ui->label->setAlignment(Qt::AlignHCenter);

    // Populate the comboBox with the data from db
    QSqlQuery query;
    query.exec("SELECT * FROM motherboard");
    while (query.next()) {
        QString id = query.value(0).toString();
        QString manufacturer = query.value(1).toString();
        QString model = query.value(2).toString();
        QString socket = query.value(3).toString();
        QString chipset = query.value(4).toString();

        ui->comboBox->addItem(id + " " + manufacturer + " " + model + " " + socket + " (" + chipset + ")");
    }

    // Create the model and set its table name.
    auto motherboardModel = new QSqlTableModel;
    motherboardModel->setTable("motherboard");
    // must call select() to populate the model with data
    motherboardModel->select();
    ui->tableView->setModel(motherboardModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setMinimumSize(1024, 600);
    // Set read-only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // No editing possible.

    setAttribute(Qt::WA_DeleteOnClose);
}
