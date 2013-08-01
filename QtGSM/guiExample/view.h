#ifndef VIEW_H
#define VIEW_H
#include <QWidget>

#include "GSModule.h"

namespace Ui {
class view;
}

/**
 * @brief GUI class which uses the GSModule
 */

class view : public QWidget
{
    Q_OBJECT
    
public:
    explicit view(QWidget *parent = 0);
    ~view();
    
private Q_SLOTS:
    /**
     * @brief Creates a QSerialPort object and initialize the GSModule
     */
    void on_btConnect_clicked();

    /**
     * @brief Displays a message in the TextEdit, formatting the message according to msgType
     */
    void processMessage(const QString &msg, GSModule::MessageType msgType);

    /**
     * @brief Sends a line using the text in the lineEdit
     */
    void on_btSend_clicked();
    void on_ctlSend_returnPressed();

    void on_btSendSMS_clicked();

private:
    Ui::view *ui;
    GSModule *m_gsm;
};

#endif // VIEW_H
