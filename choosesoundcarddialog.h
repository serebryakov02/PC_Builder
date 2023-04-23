#ifndef CHOOSESOUNDCARDDIALOG_H
#define CHOOSESOUNDCARDDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class ChooseSoundCardDialog;
}

class ChooseSoundCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseSoundCardDialog(QWidget *parent = nullptr);
    ~ChooseSoundCardDialog();

    void setFltr(const QString &fltrName);

signals:
    void sendSoundCardInfo(const QString &SCInfo);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ChooseSoundCardDialog *ui;

    QSqlDatabase db;
    QSqlTableModel *scModel;
    void init_db();

    void init_gui();
};

#endif // CHOOSESOUNDCARDDIALOG_H
