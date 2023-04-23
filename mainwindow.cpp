#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QDebug>
#include "item.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpacerItem>
#include <QPushButton>
#include <QStyle>
#include "choosecpudialog.h"
#include "choosembdialog.h"
#include "chooseramdialog.h"
#include "choosegpudialog.h"
#include "choosenetworkcarddialog.h"
#include "choosesoundcarddialog.h"

#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      view(nullptr),
      msgCount(0),
      installedMemoryAmnt(0),
      maxRAMValue(0),
      previousRAM1choice(0),
      flagRAM1(false),
      flagRAM2(false),
      maxPCISlots(0),
      PCIslots(0),
      flagGPUSlot(false),
      flagPCI1Slot(false),
      flagPCI2Slot(false)
{
    init_gui();
}

MainWindow::~MainWindow()
{
}

void MainWindow::init_gui()
{
    // Layout
    //setStyleSheet("background: gray");
    auto scene = new QGraphicsScene;
    view = new QGraphicsView;
    auto layout = new QHBoxLayout;
    auto form = new QWidget;
    form->setLayout(layout);
    setCentralWidget(form);
    view->setScene(scene);
    layout->addWidget(view);

    //--------------------------------------------------------------------------------------------------------------
    lblMB = new QLabel("Motherboard: <span style='background: red'>Not selected</span>");
    lblCPU = new QLabel("CPU: <span style='background: red'>Not selected</span>");
    lblRAM1 = new QLabel("RAM1: <span style='background: red'>Not Selected</span>");
    lblRAM2 = new QLabel("RAM2: <span style='background: red'>Not Selected</span>");
    lblGPU = new QLabel("GPU: <span style='background: red'>Not Selected</span>");
    lblNetworkCard = new QLabel("Network Card (optional): <span style='background: #FFD700'>Not Selected</span>");
    lblSoundCard = new QLabel("Sound Card (optional): <span style='background: #FFD700'>Not Selected</span>");

    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(lblMB);
    vLayout->addWidget(lblCPU);
    vLayout->addWidget(lblRAM1);
    vLayout->addWidget(lblRAM2);
    vLayout->addWidget(lblGPU);
    vLayout->addWidget(lblNetworkCard);
    vLayout->addWidget(lblSoundCard);

    auto grpBoxPC = new QGroupBox("PC Components");
    grpBoxPC->setAlignment(Qt::AlignHCenter);
    grpBoxPC->setLayout(vLayout);

    auto btnOK = new QPushButton("OK");
    btnOK->setIcon(style()->standardIcon(QStyle::SP_DialogOkButton));
    auto btnCancel = new QPushButton("Cancel");
    btnCancel->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    connect(btnCancel, &QPushButton::clicked, this, &MainWindow::close);

    auto btnsHLayout = new QHBoxLayout;
    btnsHLayout->addWidget(btnOK);
    btnsHLayout->addWidget(btnCancel);

    vLayout->addLayout(btnsHLayout);

    auto vSpacer = new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vLayout->addSpacerItem(vSpacer);

    layout->addWidget(grpBoxPC);
    //--------------------------------------------------------------------------------------------------------------

    // Disable scroll bars
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // How the view should position the scene when the view is resized
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    // For dererming selection coords
    view->setDragMode(QGraphicsView::RubberBandDrag);
    connect(view, &QGraphicsView::rubberBandChanged, this,
            [](QRectF rect, QPointF start, QPointF end) {
        qDebug() << "Start: " << start <<
                    "End: " << end <<
                    "Size?: " << rect.size();
    });

    QPixmap pix(":/images/mb.png");
    pix.scaled(view->size(), Qt::KeepAspectRatio);
    view->setSceneRect(pix.rect());
    scene->addPixmap(pix);

    addItem("cpu socket", QRectF(196.84, 126.924, 66, 66));
    addItem("ram1", QRectF(334.510, 87.1261, 10, 183));
    addItem("ram2", QRectF(375.395, 87.1261, 10, 183));
    addItem("gpu", QRectF(124.773, 321.613, 186, 8));
    addItem("pci1", QRectF(154.891, 378.622, 186, 8));
    addItem("pci1", QRectF(154.891, 419.955, 186, 8));

    ChooseMBDialog *mbDialog = new ChooseMBDialog(this);
    connect(mbDialog, &ChooseMBDialog::sendMBInfo, this,
            [this](const QString &MBInfo){
        QString MBInfo_copy = MBInfo;
        MBInfo_copy.remove(0,2);
        lblMB->setText("Motherboard: <span style='background: green'>" +
                       MBInfo_copy +
                       "</span>");
        mbName = MBInfo;
    });
    if (QDialog::Rejected == mbDialog->exec())
        exit(EXIT_FAILURE);

    maxRAMValue = mbDialog->getMaxRAMValue(mbName);
    maxPCISlots = mbDialog->getMaxPCISlots(mbName);

    qDebug() << mbName;
}

void MainWindow::addItem(QString name, const QRectF& rect)
{
    if (!view)
        return;

    auto scene = view->scene();
    if (!scene)
        return;

    auto item = new Item(rect);
    item->setObjectName(name);
    scene->addItem(item);

    connect(item, &Item::cpuSocketClicked, this, [this](){
        auto chooseCPUDialog = new ChooseCPUDialog(this);
        connect(chooseCPUDialog, &ChooseCPUDialog::sendCPUName, this,
                [this](const QString& cpuName){
            lblCPU->setText("CPU: <span style='background: green'>" +
                            cpuName +
                            "</span>");
        });
        chooseCPUDialog->setFltr(mbName);
        chooseCPUDialog->exec();
    });

    connect(item, &Item::ram1Clicked, this, [this](){
        // If the user clicks on the slot, then he want to re-choose RAM
        if (flagRAM1) {
            installedMemoryAmnt -= previousRAM1choice;
            flagRAM1 = false;
        }

        qDebug() << "BEFORE; installedMemoryAmnt: " << installedMemoryAmnt;
        qDebug() << "BEFORE; " << maxRAMValue;

        if (installedMemoryAmnt < maxRAMValue) {
            auto chooseRAMDialog = new ChooseRAMDialog(this);

            connect(chooseRAMDialog, &ChooseRAMDialog::sendRAMInfo, this,
                    [this](const QString &RAMInfo) {
                auto RAMInfo_copy = RAMInfo;
                RAMInfo_copy.remove(0, 2);
                lblRAM1->setText("RAM1: <span style='background: green'>" +
                                 RAMInfo_copy +
                                 "</span>");
            });
            chooseRAMDialog->setFltr(mbName);
            chooseRAMDialog->exec();
            previousRAM1choice= chooseRAMDialog->getMemory_size();
            installedMemoryAmnt += chooseRAMDialog->getMemory_size();

            flagRAM1 = true;

            if (installedMemoryAmnt > maxRAMValue) {
                installedMemoryAmnt -= previousRAM1choice;
                QMessageBox::critical(this, "Error!", "You have installed max amount of RAM!"
                                                      " Please re-select a ram with less volume!");
                lblRAM1->setText("RAM1: <span style='background: red'>Not Selected</span>");
                flagRAM1 = false;
            }
        } else {
            QMessageBox::critical(this, "Error!", "You have already installed max amount of RAM!");
        }

        qDebug() << "AFTER; installedMemoryAmnt: " << installedMemoryAmnt;
    });

    connect(item, &Item::ram2Clicked, this, [this](){
        // If the user clicks on the slot, then he want to re-choose RAM
        if (flagRAM2) {
            installedMemoryAmnt -= previousRAM2choice;
            flagRAM2 = false;
        }

        qDebug() << "BEFORE; installedMemoryAmnt: " << installedMemoryAmnt;
        qDebug() << "BEFORE; " << maxRAMValue;

        if (installedMemoryAmnt < maxRAMValue) {
            auto chooseRAMDialog = new ChooseRAMDialog(this);

            connect(chooseRAMDialog, &ChooseRAMDialog::sendRAMInfo, this,
                    [this](const QString &RAMInfo) {
                auto RAMInfo_copy = RAMInfo;
                RAMInfo_copy.remove(0, 2);
                lblRAM2->setText("RAM2: <span style='background: green'>" +
                                 RAMInfo_copy +
                                 "</span>");
            });
            chooseRAMDialog->setFltr(mbName);
            chooseRAMDialog->exec();
            previousRAM2choice = chooseRAMDialog->getMemory_size();
            installedMemoryAmnt += chooseRAMDialog->getMemory_size();

            flagRAM2 = true;

            if (installedMemoryAmnt > maxRAMValue) {
                installedMemoryAmnt -= previousRAM2choice;
                QMessageBox::critical(this, "Error!", "You have installed max amount of RAM!"
                                                      " Please re-select a ram with less volume!");
                lblRAM2->setText("RAM2: <span style='background: red'>Not Selected</span>");
                flagRAM2 = false;
            }
        } else {
            QMessageBox::critical(this, "Error!", "You have already install max amount of RAM!");
        }

        qDebug() << "AFTER; installedMemoryAmnt: " << installedMemoryAmnt;
    });

    connect(item, &Item::gpuSlotClicked, this, [this](){
        qDebug() << "PCISlots BEFORE " << PCIslots;

        if (flagGPUSlot) {
            QMessageBox::warning(this, "Warning!", "You have to pick another GPU necessarily!");
            PCIslots--;
            flagGPUSlot = false;
        }

        if (PCIslots < maxPCISlots) {
            auto chooseGPUDialog = new ChooseGPUDialog(this);
            connect(chooseGPUDialog, &ChooseGPUDialog::sendGPUName, this,
                    [this](const QString& gpuName){
                lblGPU->setText("GPU: <span style='background: green'>" +
                                gpuName +
                                "</span>");
            });
            chooseGPUDialog->setFltr(mbName);
            if (chooseGPUDialog->exec() == QDialog::Accepted) {
                PCIslots++;
                flagGPUSlot = true;
            }
        } else {
            QMessageBox::critical(this, "Info", "You have already used all PCI slots!");
        }

        qDebug() << "PCISlots AFTER " << PCIslots;
    });

    connect(item, &Item::pci1Clicked, this, [this](){
        qDebug() << "PCISlots BEFORE " << PCIslots;

        if (flagPCI1Slot) {
            QMessageBox::warning(this, "Warning!", "You have to pick another network card necessarily!");
            PCIslots--;
            flagPCI1Slot = false;
        }

        if (PCIslots < maxPCISlots) {
            auto chooseNetworkCardDialog = new ChooseNetworkCardDialog(this);

            connect(chooseNetworkCardDialog, &ChooseNetworkCardDialog::sendNCInfo, this,
                    [this](const QString& NCInfo){
                lblNetworkCard->setText("Network Card: <span style='background: green'>" +
                                        NCInfo +
                                        "</span>");
            });

            chooseNetworkCardDialog->setFltr(mbName);
            if (chooseNetworkCardDialog->exec() == QDialog::Accepted) {
                PCIslots++;
                flagPCI1Slot = true;
            }
        } else {
            QMessageBox::critical(this, "Info", "You have already used all PCI slots!");
        }
        qDebug() << "PCISlots AFTER " << PCIslots;
    });

    connect(item, &Item::pci2Clicked, this,
            [this](){

        if (flagPCI2Slot) {
            QMessageBox::warning(this, "Warning!", "You have to pick another sound card necessarily!");
            PCIslots--;
            flagPCI2Slot = false;
        }

        if (PCIslots < maxPCISlots) {
            auto chooseSoundCardDialog = new ChooseSoundCardDialog(this);
            connect(chooseSoundCardDialog, &ChooseSoundCardDialog::sendSoundCardInfo, this,
                    [this](const QString &SCInfo) {
                lblSoundCard->setText("Sound Card: <span style='background: green'>" +
                                      SCInfo +
                                      "</span>");
            });
            chooseSoundCardDialog->setFltr(mbName);
            if (chooseSoundCardDialog->exec() == QDialog::Accepted) {
                PCIslots++;
                flagPCI2Slot = true;
            }
        } else {
            QMessageBox::critical(this, "Info", "You have already used all PCI slots!");
        }
    });
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    view->fitInView(view->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}
