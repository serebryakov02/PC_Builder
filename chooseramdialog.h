#ifndef CHOOSERAMDIALOG_H
#define CHOOSERAMDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class ChooseRAMDialog;
}

class ChooseRAMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseRAMDialog(QWidget *parent = nullptr);
    ~ChooseRAMDialog();

    void setFltr(const QString &fltrName);


    int getMemory_size() const;

signals:
    void sendRAMInfo(const QString& ramInfo);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ChooseRAMDialog *ui;

    QSqlDatabase db;
    QSqlTableModel *ramModel;
    void init_db();
    void init_gui();

    int memory_size;
};

#endif // CHOOSERAMDIALOG_H
