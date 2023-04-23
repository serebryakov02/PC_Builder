#include "choosesoundcarddialog.h"
#include "ui_choosesoundcarddialog.h"
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

ChooseSoundCardDialog::ChooseSoundCardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseSoundCardDialog)
{
    ui->setupUi(this);

    init_db();
    init_gui();
}

ChooseSoundCardDialog::~ChooseSoundCardDialog()
{
    delete ui;
}

void ChooseSoundCardDialog::setFltr(const QString &fltrName)
{
    QSqlQuery query;
    query.exec("SELECT graphics_card_slot FROM motherboard WHERE motherboard_id = "
               + fltrName.left(1) + ";");
    QString graphics_card_slot;
    if (query.next()) {
        graphics_card_slot = query.value(0).toString();
    } else {
        qDebug() << "graphics_card_slot: -";
    }

    scModel->setFilter("pci_slot = '" + graphics_card_slot + "';");
    scModel->select();

    ui->comboBox->clear();
    query.exec("SELECT * "
               "FROM sound_card "
               "WHERE pci_slot = '" +
               graphics_card_slot + "';");

    while (query.next()) {
        QString name = query.value(1).toString();
        QString manufacturer = query.value(2).toString();
        QString interface = query.value(3).toString();
        //QString pci_slot = query.value(4).toString();

        ui->comboBox->addItem(name + " " + manufacturer + " " + interface);
    }
}

void ChooseSoundCardDialog::on_buttonBox_accepted()
{
    emit sendSoundCardInfo(ui->comboBox->itemText(ui->comboBox->currentIndex()));
    accept();
}

void ChooseSoundCardDialog::on_buttonBox_rejected()
{
    reject();
}

void ChooseSoundCardDialog::init_db()
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

void ChooseSoundCardDialog::init_gui()
{
    ui->lblAvailableSoundCards->setText("Available Sound Cards");
    ui->lblAvailableSoundCards->setStyleSheet("color: lime");
    ui->lblAvailableSoundCards->setAlignment(Qt::AlignHCenter);

    ui->label->setText("Please select a sound card");
    ui->label->setAlignment(Qt::AlignHCenter);

    // Create the model and set its table name.
    scModel = new QSqlTableModel;
    scModel->setTable("sound_card");

    // must call select() to populate the model with data
    scModel->select();
    ui->tableView->setModel(scModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setMinimumSize(800, 600);
    // Set read-only
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // No editing possible.

    setAttribute(Qt::WA_DeleteOnClose);
}

