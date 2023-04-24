#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr);
    ~ResultDialog();

    void setlblMBText(const QString &MB);
    void setlblCPUText(const QString &CPU);
    void setlblRAM1Text(const QString &RAM1);
    void setlblRAM2Text(const QString &RAM2);
    void setlblGPUText(const QString &GPU);
    void setlblPCI1Text(const QString &PCI1);
    void setlblPCI2Text(const QString &PCI2);

private:
    Ui::ResultDialog *ui;

    void init_gui();
};

#endif // RESULTDIALOG_H
