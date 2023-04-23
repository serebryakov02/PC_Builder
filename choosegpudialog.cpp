#include "choosegpudialog.h"
#include "ui_choosegpudialog.h"
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

ChooseGPUDialog::ChooseGPUDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseGPUDialog)
{
    ui->setupUi(this);

    init_db();
    init_gui();
}

ChooseGPUDialog::~ChooseGPUDialog()
{
    delete ui;
}

void ChooseGPUDialog::setFltr(const QString &fltrName)
{
    QSqlQuery query;
    query.exec("SELECT graphics_card_slot FROM motherboard WHERE motherboard_id = "
               + fltrName.left(1) + ";");
    QString graphics_card_slot;
    if (query.next()) {
        graphics_card_slot = query.value(0).toString();
        qDebug() << "graphics_card_slot: " << graphics_card_slot;
    } else {
        qDebug() << "graphics_card_slot: -";
    }

    gpuModel->setFilter("graphics_card_slot = '" + graphics_card_slot + "';");
    gpuModel->select();

    ui->comboBox->clear();
    query.exec("SELECT * "
               "FROM gpu "
               "WHERE graphics_card_slot = '" +
               graphics_card_slot + "';");

    while (query.next()) {
        QString manufacturer = query.value(1).toString();
        QString model = query.value(2).toString();
        QString memory_type = query.value(3).toString();
        QString memory_size = query.value(4).toString();

        ui->comboBox->addItem(manufacturer + " " + model + " " + memory_type + " " +
                              memory_size + "GB");
    }
}

void ChooseGPUDialog::on_buttonBox_accepted()
{
    emit sendGPUName(ui->comboBox->itemText(ui->comboBox->currentIndex()));
    accept();
}

void ChooseGPUDialog::on_buttonBox_rejected()
{
    reject();
}

void ChooseGPUDialog::init_db()
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

void ChooseGPUDialog::init_gui()
{
    ui->lblAvailableGPUs->setText("Available GPUs");
    ui->lblAvailableGPUs->setStyleSheet("color: lime");
    ui->lblAvailableGPUs->setAlignment(Qt::AlignHCenter);

    ui->label->setText("Please select a GPU");
    ui->label->setAlignment(Qt::AlignHCenter);

    // Create the model and set its table name.
    gpuModel = new QSqlTableModel;
    gpuModel->setTable("gpu");

    // must call select() to populate the model with data
    gpuModel->select();
    ui->tableView->setModel(gpuModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setMinimumSize(800, 600);
    // Set read-only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // No editing possible.

    setAttribute(Qt::WA_DeleteOnClose);
}

