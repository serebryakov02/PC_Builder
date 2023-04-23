#ifndef CHOOSECPUDIALOG_H
#define CHOOSECPUDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class ChooseCPUDialog;
}

class ChooseCPUDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCPUDialog(QWidget *parent = nullptr);
    ~ChooseCPUDialog();

    void setFltr(const QString &fltrName);

signals:
    void sendCPUName(const QString& cpuName);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ChooseCPUDialog *ui;

    QSqlDatabase db;
    QSqlTableModel *cpuModel;
    void init_db();

    void init_gui();
};

#endif // CHOOSECPUDIALOG_H
