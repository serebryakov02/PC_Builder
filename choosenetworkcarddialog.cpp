#include "choosenetworkcarddialog.h"
#include "ui_choosenetworkcarddialog.h"
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

ChooseNetworkCardDialog::ChooseNetworkCardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseNetworkCardDialog)
{
    ui->setupUi(this);

    init_db();
    init_gui();
}

ChooseNetworkCardDialog::~ChooseNetworkCardDialog()
{
    delete ui;
}

void ChooseNetworkCardDialog::setFltr(const QString &fltrName)
{
    QSqlQuery query;
    query.exec("SELECT graphics_card_slot FROM motherboard WHERE motherboard_id = "
               + fltrName.left(1) + ";");
    QString graphics_card_slot;
    if (query.next()) {
        graphics_card_slot = query.value(0).toString();
        qDebug() << "ChooseNetworkCardDialog::   graphics_card_slot: " << graphics_card_slot;
    } else {
        qDebug() << "ChooseNetworkCardDialog::   graphics_card_slot: -";
    }

    netCardModel->setFilter("pci_slot = '" + graphics_card_slot + "';");
    netCardModel->select();

    ui->comboBox->clear();
    query.exec("SELECT * "
               "FROM network_card "
               "WHERE pci_slot = '" +
               graphics_card_slot + "';");

    while (query.next()) {
        QString manufacturer = query.value(1).toString();
        QString model = query.value(2).toString();
        QString interface_type = query.value(3).toString();
        //QString pci_slot = query.value(4).toString();

        ui->comboBox->addItem(manufacturer + " " + model + " " + interface_type);
    }
}

void ChooseNetworkCardDialog::on_buttonBox_accepted()
{
    emit sendNCInfo(ui->comboBox->itemText(ui->comboBox->currentIndex()));
    accept();
}

void ChooseNetworkCardDialog::on_buttonBox_rejected()
{
    reject();
}

void ChooseNetworkCardDialog::init_db()
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

void ChooseNetworkCardDialog::init_gui()
{
    ui->lblAvailableNetworkCards->setText("Available Netword Cards");
    ui->lblAvailableNetworkCards->setStyleSheet("color: lime");
    ui->lblAvailableNetworkCards->setAlignment(Qt::AlignHCenter);

    ui->label->setText("Please select a network card");
    ui->label->setAlignment(Qt::AlignHCenter);

    // Create the model and set its table name.
    netCardModel = new QSqlTableModel;
    netCardModel->setTable("network_card");

    // must call select() to populate the model with data
    netCardModel->select();
    ui->tableView->setModel(netCardModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setMinimumSize(800, 600);
    // Set read-only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // No editing possible.

    //setAttribute(Qt::WA_DeleteOnClose);
}
