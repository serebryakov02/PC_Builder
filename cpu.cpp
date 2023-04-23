#include "cpu.h"
#include <QDebug>

Cpu::Cpu(int id, QString manufacturer, QString model, QString socket, QString chipset) :
    m_id(id),
    m_manufacturer(manufacturer),
    m_model(model),
    m_socket(socket),
    m_chipset(chipset)
{}

int Cpu::id() const
{
    return m_id;
}

QString Cpu::manufacturer() const
{
    return m_manufacturer;
}

QString Cpu::model() const
{
    return m_model;
}

QString Cpu::socket() const
{
    return m_socket;
}

QString Cpu::chipset() const
{
    return m_chipset;
}

void Cpu::showInfo() const
{
    qDebug() << "CPU ID:" << m_id;
    qDebug() << "Manufacturer:" << m_manufacturer;
    qDebug() << "Model:" << m_model;
    qDebug() << "Socket:" << m_socket;
    qDebug() << "Chipset:" << m_chipset;
}
