#include "view.h"
#include "ui_view.h"

#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>

view::view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::view),
    m_gsm(0)
{
    ui->setupUi(this);
    m_gsm = new gsmModule(this);
    connect(m_gsm, SIGNAL(message(QString, gsmModule::MessageType)), this, SLOT(processMessage(QString, gsmModule::MessageType)));
}

view::~view()
{
    delete ui;
}

void view::on_btConnect_clicked()
{
    // Creates the serial port here because it changes a lots depending on the system it's run from.
    QSerialPort *serialPort = new QSerialPort(m_gsm);

    serialPort->setPort(QSerialPortInfo("ttyUSB0"));
   serialPort->setBaudRate(QSerialPort::Baud19200);
   serialPort->setDataBits(QSerialPort::Data8);
   serialPort->setParity(QSerialPort::OddParity);
   serialPort->setStopBits(QSerialPort::OneStop);

    m_gsm->initializeSerialConnection(serialPort);
}

void view::processMessage(const QString &msg, gsmModule::MessageType msgType)
{
    switch(msgType) {
    case gsmModule::DebugInformation:
        ui->txtOutput->setTextColor(Qt::darkGray);
        break;
    case gsmModule::ATFormatted:
        ui->txtOutput->setTextColor(Qt::darkYellow);
        break;
    case gsmModule::RawSerial:
        ui->txtOutput->setTextColor(Qt::darkGreen);
        break;
    case gsmModule::Error:
        ui->txtOutput->setTextColor(Qt::darkRed);
        break;
    default:
        ui->txtOutput->setTextColor(Qt::black);
        break;
    }
    ui->txtOutput->append(QString("%1 - %2").arg(QTime::currentTime().toString()).arg(msg));
}

void view::on_btSend_clicked()
{
    if (!m_gsm) {
        processMessage(tr("Could not send: GSM module not initialized"), gsmModule::Error);
        return;
    } else if (ui->ctlSend->text().isEmpty()) {
        processMessage(tr("Could not send: nothing to send"), gsmModule::Error);
        return;
    }
    m_gsm->send(ui->ctlSend->text());
    ui->ctlSend->clear();
}

void view::on_ctlSend_returnPressed()
{
    on_btSend_clicked();
}

void view::on_btSendSMS_clicked()
{
    m_gsm->sendSMS(ui->txtSMS->toPlainText(), ui->ctlRecipient->text() );
}
