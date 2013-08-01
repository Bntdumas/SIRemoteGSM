#include "punchVisualiser.h"
#include "ui_punchVisualiser.h"

#include <QTime>

punchVisualiser::punchVisualiser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::punchVisualiser)
{
    ui->setupUi(this);
    m_sportIdentModule = new SportIdentReader(this);
    connect(m_sportIdentModule, SIGNAL(logText(QString,SportIdentReader::MessageType)), this, SLOT(processMessage(QString,SportIdentReader::MessageType)));
}

punchVisualiser::~punchVisualiser()
{
    delete ui;
}

void punchVisualiser::on_lePort_returnPressed()
{
    on_btConnect_clicked();
}

void punchVisualiser::on_btConnect_clicked()
{
    m_sportIdentModule->setserialPort(ui->lePort->text());
    m_sportIdentModule->setserialSpeed(ui->cbxBaudRate->currentText().toInt());
    m_sportIdentModule->initSerialConnection();
}

void punchVisualiser::processMessage(const QString &msg, SportIdentReader::MessageType msgType)
{
    switch(msgType) {
    case SportIdentReader::DebugInformation:
        ui->textEdit->setTextColor(Qt::darkGray);
        break;
    case SportIdentReader::RawSerial:
        ui->textEdit->setTextColor(Qt::darkGreen);
        break;
    case SportIdentReader::Warning:
        ui->textEdit->setTextColor(Qt::darkYellow);
        break;
    case SportIdentReader::Error:
        ui->textEdit->setTextColor(Qt::darkRed);
        break;
    default:
        ui->textEdit->setTextColor(Qt::black);
        break;
    }
    ui->textEdit->append(QString("%1 - %2").arg(QTime::currentTime().toString()).arg(msg));
}
