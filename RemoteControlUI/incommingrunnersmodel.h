#ifndef INCOMMINGRUNNERSMODEL_H
#define INCOMMINGRUNNERSMODEL_H

#include <QAbstractListModel>
#include <QTime>

/**
 * @brief The IncommingRunnersModel class
 *
 * \see IncommingRunnersModel::addRunner
 */
class IncommingRunnersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit IncommingRunnersModel(QObject *parent = 0);

    enum Roles
    {
        NameRole = Qt::UserRole,
        TeamRole,
        TimeRole,
        RawTimeRole,
        RealTimeRole,
        LapRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QHash<int, QByteArray> roleNames() const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

public slots:
    /**
     * @brief addRunner Adds a runner to the list of punched runners
     * @param name The name of the runner
     * @param team The runners team
     * @param time The time the runner has been on his lap for
     * @param realTime The time at which the runner punched the control
     * @param lap The lap the runner is on
     */
    void addRunner(const QString& name, const QString& team, const QTime& time, const QTime& realTime,
                   const int lap);

    /**
     * @brief reload Reloads all runners from memory
     */
    void reload();

private:
    struct IncommingRunner
    {
        IncommingRunner() {}
        IncommingRunner(const QString& name, const QString& team, const QTime& time, const QTime& realTime,
                        const int lap) : name(name), team(team), time(time), realTime(realTime), lap(lap) {}
        QString name;
        QString team;
        QTime time;
        QTime realTime;
        int lap;

        bool operator<(const IncommingRunner& other);
    };
    QList<IncommingRunner> m_runners;
};

#endif // INCOMMINGRUNNERSMODEL_H
