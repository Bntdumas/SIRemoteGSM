#ifndef BBSMSPROVIDER_H
#define BBSMSPROVIDER_H

#include "runnermapper.h"

#include <bb/pim/message/MessageService>

class BBSMSProvider : public SIProvider
{
    Q_OBJECT
public:
    explicit BBSMSProvider(QObject *parent = 0);

private slots:
    void messageReceived(bb::pim::account::AccountKey accountId,
                         bb::pim::message::ConversationKey conversationId,
                         bb::pim::message::MessageKey messageId);

private:
    bb::pim::message::MessageService* m_service;
};

#endif // BBSMSPROVIDER_H
