#ifndef CHOOSEMBDIALOG_H
#define CHOOSEMBDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class ChooseMBDialog;
}

class ChooseMBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseMBDialog(QWidget *parent = nullptr);
    ~ChooseMBDialog();

    int getMaxRAMValue(const QString &mbName);
    int getMaxPCISlots(const QString &mbName);

signals:
    void sendMBInfo(const QString& MBInfo);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ChooseMBDialog *ui;

    QSqlDatabase db;
    void init_db();
    void init_gui();

    int maxRAMValue;
    int graphics_card_slot_quantity;
};

#endif // CHOOSEMBDIALOG_H
