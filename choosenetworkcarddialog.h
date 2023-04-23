#ifndef CHOOSENETWORKCARDDIALOG_H
#define CHOOSENETWORKCARDDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class ChooseNetworkCardDialog;
}

class ChooseNetworkCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseNetworkCardDialog(QWidget *parent = nullptr);
    ~ChooseNetworkCardDialog();

    void setFltr(const QString &fltrName);

signals:
    void sendNCInfo(const QString &NCInfo);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ChooseNetworkCardDialog *ui;

    QSqlDatabase db;
    QSqlTableModel *netCardModel;
    void init_db();

    void init_gui();
};

#endif // CHOOSENETWORKCARDDIALOG_H
