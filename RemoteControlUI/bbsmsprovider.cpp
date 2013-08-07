#include "bbsmsprovider.h"

#include <QStringList>
#include <QDebug>

using namespace bb::pim::message;
using namespace bb::pim::account;

BBSMSProvider::BBSMSProvider(QObject *parent) :
    SIProvider(parent)
{
    m_service = new MessageService(this);
    connect(m_service,
            SIGNAL(messageAdded(bb::pim::account::AccountKey,
                                bb::pim::message::ConversationKey,
                                bb::pim::message::MessageKey)),
            this,
            SLOT(messageReceived(bb::pim::account::AccountKey,
                                 bb::pim::message::ConversationKey,
                                 bb::pim::message::MessageKey)));
}

void BBSMSProvider::messageReceived(AccountKey accountId, ConversationKey /*conversationId*/, MessageKey messageId)
{
    const Message message = m_service->message(accountId, messageId);
    m_service->markRead(accountId, messageId);
    const QString sender = message.sender().address();
    const QString body = message.body(MessageBody::PlainText).plainText();
    const QStringList parts = body.split('@');
    if (parts.size() < 2) {
        return;
    }

    const int si = parts.at(0).toInt();
    const QTime time = QTime::fromString(parts.at(1), "hh:mm:ss");
    qDebug() << "NOTE: Emitting signal with parameters" << si << "and" << time;
    emit runnerPunched(si, time);
}
