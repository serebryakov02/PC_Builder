#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QTextEdit>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QGraphicsView *view;
    QTextEdit *textEdit;
    int msgCount;

    QLabel *lblMB;
    QLabel *lblCPU;
    QLabel *lblRAM1;
    QLabel *lblRAM2;
    QLabel *lblGPU;
    QLabel *lblNetworkCard;
    QLabel *lblSoundCard;

    void init_gui();
    void addItem(QString name, const QRectF& rect);

    QString mbName;

    int installedMemoryAmnt;
    int maxRAMValue;
    int previousRAM1choice;
    int previousRAM2choice;

    bool flagRAM1;
    bool flagRAM2;

    int maxPCISlots;
    int PCIslots;

    bool flagGPUSlot;

    bool flagPCI1Slot;
    bool flagPCI2Slot;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
