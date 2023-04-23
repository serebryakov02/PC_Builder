#include "chooseramdialog.h"
#include "ui_chooseramdialog.h"
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

ChooseRAMDialog::ChooseRAMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseRAMDialog), memory_size(-1)
{
    ui->setupUi(this);

    init_db();
    init_gui();
}

ChooseRAMDialog::~ChooseRAMDialog()
{
    qDebug() << "ChooseRAMDialog::~ChooseRAMDialog()";
    delete ui;
}

void ChooseRAMDialog::setFltr(const QString &fltrName)
{
    QSqlQuery query;
    query.exec("SELECT memory_size FROM motherboard WHERE motherboard_id = " +
               fltrName.left(1) + ";");
    QString memory_size;
    if (query.next()) {
        memory_size = query.value(0).toString();
        //        qDebug() << "MEMORY_SIZE: " << memory_size;
    }

    // Set the filter
    ramModel->setFilter("memory_type = "
                        "(SELECT memory_type FROM motherboard WHERE motherboard_id = " +
                        fltrName.left(1) + ") AND memory_size <= '" + memory_size + "';");
    // re-select data from the table, taking into account the filter
    ramModel->select();

    ui->comboBox->clear();
    query.exec("SELECT * FROM ram WHERE memory_type = (SELECT memory_type FROM"
               " motherboard WHERE motherboard_id = " +
               fltrName.left(1) + ") AND memory_size <= '" + memory_size + "';");

    while (query.next()) {
        QString id = query.value(0).toString();
        QString manufacturer = query.value(1).toString();
        QString model = query.value(2).toString();
        QString memory_type = query.value(3).toString();
        QString mry_siz = query.value(5).toString();

        ui->comboBox->addItem(id + " " + manufacturer + " " + model + " " + memory_type + " " + mry_siz + "GB");
    }
}

void ChooseRAMDialog::on_buttonBox_accepted()
{
    emit sendRAMInfo(ui->comboBox->itemText(ui->comboBox->currentIndex()));

    QSqlQuery query;
    QString id = QString(ui->comboBox->itemText(ui->comboBox->currentIndex())).left(1);
    query.exec("SELECT memory_size FROM ram WHERE ram_id = " + id + ";");

    if (query.next()) {
        memory_size = query.value(0).toInt();
    }
    qDebug() << "ChooseRAMDialog::memory_size " << memory_size;

    accept();
}

void ChooseRAMDialog::on_buttonBox_rejected()
{
    reject();
}

void ChooseRAMDialog::init_db()
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

void ChooseRAMDialog::init_gui()
{
    ui->lblAvailableRAM->setText("Available RAM");
    ui->lblAvailableRAM->setStyleSheet("color: lime");
    ui->lblAvailableRAM->setAlignment(Qt::AlignHCenter);

    ui->label->setText("Please select a RAM");
    ui->label->setAlignment(Qt::AlignHCenter);

    // Create the model and set its table name.
    ramModel = new QSqlTableModel;
    ramModel->setTable("ram");

    // must call select() to populate the model with data
    ramModel->select();
    ui->tableView->setModel(ramModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setMinimumSize(640, 480);
    // Set read-only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // No editing possible.

    //setAttribute(Qt::WA_DeleteOnClose);
}

int ChooseRAMDialog::getMemory_size() const
{
    return memory_size;
}

