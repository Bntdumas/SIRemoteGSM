#include "basicfilemapper.h"

#include <QFile>
#include <QDir>
#include <QDebug>

BasicFileMapper::BasicFileMapper(QObject *parent) :
    RunnerMapper(parent)
{
    loadRunners();
}

void BasicFileMapper::loadRunners()
{
    QDir dir;
    dir.cd("app");
    dir.cd("native");
    dir.cd("data");
    QFile file(dir.filePath("runners.txt"));
    if (file.open(QFile::ReadOnly)) {
        qDebug() << "NOTE: Opened runners.txt";
        qDebug() << "NOTE: The size of runners.txt is" << file.size();
        while (!file.atEnd()) {
            const QString line = QString::fromLatin1(file.readLine());
            if (line.startsWith('#')) {
                qDebug() << "NOTE: Commented line:" << line;
                continue;
            }
            MappedRunner runner;
            const int si = line.section(",,,", 0, 0).toInt();
            runner.name = line.section(",,,", 1, 1);
            runner.team = line.section(",,,", 2, 2);
            runner.lap = line.section(",,,", 3, 3).toInt();
            qDebug() << "NOTE: Added runner to mapping:" << si << "->" << runner.name;
            m_runners.insert(si, runner);
        }
    } else {
        qDebug() << "NOTE: Couldn't open file:" << file.errorString();
    }
}

bool BasicFileMapper::map(const int si, const QTime &realTime, QString *name, QString *team, QTime *time, int *lap)
{
    if (!m_runners.contains(si)) {
        qDebug() << "NOTE: No such runner found";
        return false;
    }

    const MappedRunner runner = m_runners[si];
    qDebug() << "NOTE: Found runner with name" << runner.name;
    *name = runner.name;
    *team = runner.team;
    *lap = runner.lap;
    *time = QTime();
    return true;
}
