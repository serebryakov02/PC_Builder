#ifndef CPU_H
#define CPU_H

#include <QString>

class Cpu
{
public:
    Cpu(int id, QString manufacturer, QString model, QString socket, QString chipset);

    int id() const;
    QString manufacturer() const;
    QString model() const;
    QString socket() const;
    QString chipset() const;

    void showInfo() const;

private:
    int m_id;
    QString m_manufacturer;
    QString m_model;
    QString m_socket;
    QString m_chipset;
};

#endif // CPU_H
