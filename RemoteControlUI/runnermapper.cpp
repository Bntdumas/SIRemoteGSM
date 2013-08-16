#include "runnermapper.h"

#include <QDebug>
#include <QAbstractListModel>

#include "util.h"

class RunnerListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    RunnerListModel(QObject* parent = 0) : QAbstractListModel(parent)
    {
#if QT_VERSION < 0x050000
        setRoleNames(roleNames());
#endif
    }

    enum {
        NameRole = Qt::UserRole,
        SIRole
    };

    int rowCount(const QModelIndex & = QModelIndex()) const
    {
        return m_runners.size();
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        int row = index.row();

        switch (role) {
        case NameRole:
            return m_runners.values()[row];
        case SIRole:
            return m_runners.keys()[row];
        }
        return QVariant();
    }
    QHash<int, QByteArray> roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[SIRole] = "si";
        return roles;
    }

    void setRunners(QMap<int, QString> runners)
    {
        beginResetModel();
        m_runners = runners;
        endResetModel();
    }

private:
    QMap<int, QString> m_runners;
};

SIProvider::SIProvider(QObject *parent) :
    QObject(parent)
{

}

RunnerMapper::RunnerMapper(QObject *parent) :
    Provider(parent), m_provider(0)
{
    m_model = new RunnerListModel(this);
}

void RunnerMapper::setProvider(SIProvider *provider)
{
    if (m_provider) {
        disconnect(m_provider, SIGNAL(runnerPunched(int,QTime)), this, SLOT(punched(int,QTime)));
    }
    m_provider = provider;
    if (m_provider) {
        connect(m_provider, SIGNAL(runnerPunched(int,QTime)), this, SLOT(punched(int,QTime)));
    }
}

int RunnerMapper::siForIndex(const int index)
{
    RunnerListModel* model = qobject_cast<RunnerListModel*>(m_model);
    return model->index(index).data(RunnerListModel::SIRole).toInt();
}

QString RunnerMapper::nameForIndex(const int index)
{
    RunnerListModel* model = qobject_cast<RunnerListModel*>(m_model);
    return model->index(index).data(RunnerListModel::NameRole).toString();
}

void RunnerMapper::reloadRunners()
{
    RunnerListModel* model = qobject_cast<RunnerListModel*>(m_model);
    model->setRunners(runners());
    emit modelChanged();
}

void RunnerMapper::error(const QString &message)
{
    m_errorMessage = message;
    emit errorMessage(m_errorMessage);
}

void RunnerMapper::punched(const int si, const QTime &realTime)
{
    QString name;
    QString team;
    QTime time;
    int lap;
    if (map(si, realTime, &name, &team, &time, &lap)) {
        emit runnerPunched(name, team , time, realTime, lap);
    } else {
        emit runnerPunched(QString("SI %1").arg(si), "", Util::instance->competitionTime().time(), realTime, -1);
    }
}

#include "runnermapper.moc"
