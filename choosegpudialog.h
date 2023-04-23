#ifndef CHOOSEGPUDIALOG_H
#define CHOOSEGPUDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class ChooseGPUDialog;
}

class ChooseGPUDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseGPUDialog(QWidget *parent = nullptr);
    ~ChooseGPUDialog();

    void setFltr(const QString &fltrName);

signals:
    void sendGPUName(const QString& GPUName);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ChooseGPUDialog *ui;

    QSqlDatabase db;
    QSqlTableModel *gpuModel;
    void init_db();

    void init_gui();
};

#endif // CHOOSEGPUDIALOG_H
