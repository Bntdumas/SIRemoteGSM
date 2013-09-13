#include "incommingrunnersmodel.h"

#include <QTime>
#include <QDebug>
#include <QTextStream>
//#include <QStandardPaths>
#include <QDir>
#include <QHash>
//#include <QRegularExpression>

#include "util.h"

IncommingRunnersModel::IncommingRunnersModel(QObject *parent) :
    QAbstractListModel(parent)
{
//    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
//    if(!dir.exists()) {
//        QString dirname = dir.dirName();
//        dir.cdUp();
//        dir.mkdir(dirname);
//        dir.cd(dirname);
//    }

    reload();

#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int IncommingRunnersModel::rowCount(const QModelIndex &) const
{
    return m_runners.size();
}

Qt::ItemFlags IncommingRunnersModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index);
}
QHash<int, QByteArray> IncommingRunnersModel::roleNames() const
{
    QHash<int, QByteArray> hash;
    hash.insert(NameRole, "name");
    hash.insert(TeamRole, "team");
    hash.insert(TimeRole, "time");
    hash.insert(RawTimeRole, "rawTime");
    hash.insert(RealTimeRole, "realTime");
    hash.insert(LapRole, "lap");
    hash.insert(BibRole, "bib");
    hash.insert(ArrivalTimeRole, "arrivalTime");
    return hash;
}

QVariant IncommingRunnersModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if(row < 0 || row > m_runners.size())
        return QVariant();

    IncommingRunner runner = m_runners[row];

    switch(role)
    {
    case NameRole:
        return runner.name;
    case TeamRole:
        return runner.team;
    case TimeRole:
    {
        if (!runner.time.isValid() || runner.time.isNull()) {
            return "";
        }
        // FIXME this is currently a hack. sort out what realTime and time are supposed to do instead!!!
        QTime time = Util::instance->timeDifference(runner.realTime, runner.time).time();
        return time.toString("hh:mm:ss");
    }
    case RawTimeRole:
        // FIXME this is currently a hack. sort out what realTime and time are supposed to do instead!!!
        return Util::instance->timeDifference(runner.realTime, runner.time).time();
    case RealTimeRole:
        return runner.realTime;
    case LapRole:
        if (runner.lap == -1) {
            return QVariant("");
        }
        return runner.lap;
    case BibRole:
        return runner.bib;
    case ArrivalTimeRole:
        return runner.arrivalTime;
    default:
        return QVariant();
    }
}
bool IncommingRunnersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();

    if(row < 0 || row > m_runners.size()) {
        return false;
    }

    switch(role)
    {
    case NameRole:
        m_runners[row].name = value.toString();
        return true;
    case TeamRole:
        m_runners[row].team = value.toString();
        return true;
    case TimeRole:
        m_runners[row].time = value.toTime();
        return true;
    case RealTimeRole:
        m_runners[row].realTime = value.toTime();
        return true;
    case LapRole:
        m_runners[row].lap = value.toInt();
        return true;
    case BibRole:
        m_runners[row].bib = value.toInt();
        return true;
    default:
        return false;
    }
}

QVariant IncommingRunnersModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{
    return QVariant();
}
bool IncommingRunnersModel::setHeaderData(int /*section*/, Qt::Orientation /*orientation*/, const QVariant &/*value*/, int /*role*/)
{
    return false;
}

bool IncommingRunnersModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count);
    for(int i = 0; i < count; i++) {
        m_runners.insert(row, IncommingRunner());
    }
    endInsertRows();
    return true;
}
bool IncommingRunnersModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count);
    for(int i = 0; i < count; i++) {
        m_runners.removeAt(row);
    }
    endRemoveRows();
    return true;
}

void IncommingRunnersModel::addRunner(const QString &name, const QString &team, const QTime &time, const QTime &realTime, const int lap, const int bib)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_runners.insert(0, IncommingRunner(name, team, time, realTime, lap, bib));

//    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
//    QFile file(dir.absoluteFilePath("runners_cache"));
//    file.open(QFile::WriteOnly | QFile::Text | QFile::Append);
//    QTextStream fileStream(&file);
//    fileStream << "\"\"" << name << "\"\", \"\"" << team << "\"\", "
//               << time.toString("hh:mm:ss") << ", " << realTime.toString("hh:mm:ss")
//               << ", " << lap << endl;
//    file.close();

    endInsertRows();
}

void IncommingRunnersModel::reload()
{
//    qDebug() << "Loading...";
//    beginResetModel();
//    m_runners.clear();
//    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
//    QFile file(dir.absoluteFilePath("runners_cache"));
//    file.open(QFile::ReadOnly);
//    QTextStream fileStream(&file);
//    QString line;
//    QRegularExpression exp("^\\\"\\\"([A-Za-z0-9\\\" ]*)\\\"\\\", \\\"\\\"([A-Za-z0-9\\\" ]*)\\\"\\\", (\\d{1,2}:\\d\\d:\\d\\d), (\\d{1,2}:\\d\\d:\\d\\d), (\\d*)$");
//    do {
//        line = fileStream.readLine();
//        QRegularExpressionMatch match = exp.match(line);
//        if (match.hasMatch()) {
//            IncommingRunner runner;
//            runner.name = match.captured(1);
//            runner.team = match.captured(2);
//            runner.time = QTime::fromString(match.captured(3), "hh:mm:ss");
//            runner.realTime = QTime::fromString(match.captured(4), "hh:mm:ss");
//            runner.lap = match.captured(5).toInt();
//            m_runners.append(runner);
//        }
//    } while (!line.isNull());
//    file.close();
//    endResetModel();
}

bool IncommingRunnersModel::IncommingRunner::operator <(const IncommingRunnersModel::IncommingRunner &other)
{
    return realTime < other.realTime;
}
