#ifndef PUNCHVISUALISER_H
#define PUNCHVISUALISER_H

#include <QWidget>
#include <SportIdentReader.h>

namespace Ui {
class punchVisualiser;
}

/**
 * @brief Example program using the sport ident reader library
 */

class punchVisualiser : public QWidget
{
    Q_OBJECT
    
public:
    explicit punchVisualiser(QWidget *parent = 0);
    ~punchVisualiser();
    
private Q_SLOTS:
    void on_lePort_returnPressed();
    void on_btConnect_clicked();

    /**
     * @brief Displays a message in the TextEdit, formatting the message according to msgType
     */
    void processMessage(const QString &msg, SportIdentReader::MessageType msgType);

    void dataReceived(const QByteArray& data);


private:
    Ui::punchVisualiser *ui;

    SportIdentReader *m_sportIdentModule;
};

#endif // PUNCHVISUALISER_H
