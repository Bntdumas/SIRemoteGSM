#include "basicfilemapper.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QDataStream>

#include "util.h"

BasicFileMapper::BasicFileMapper(QObject *parent) :
    RunnerMapper(parent)
{
    if (!hasCache()) {
        loadPermanentData();
    } else {
        loadCache();
    }
    reloadRunners();
}

QString BasicFileMapper::generateCache() const
{
    QStringList rows;
    foreach (const int si, m_runners.keys()) {
        MappedRunner runner = m_runners[si];
        QString row = "%1,,,%2,,,%3,,,%4,,,%5";
        row = row.arg(si)
                .arg(runner.name)
                .arg(runner.bibnumber)
                .arg(runner.team)
                .arg(runner.lap);
        rows.append(row);
    }

    return rows.join("\n") + '\n';
}

void BasicFileMapper::readCache(const QString &data)
{
    QStringList rows = data.split('\n');
    foreach (const QString& entry, rows) {
        if (entry.isEmpty()) {
            continue;
        }
        MappedRunner runner;
        QStringList row = entry.split(",,,");
        if (row.size() < 5) {
            qWarning("Not enough cells");
            continue;
        }
        runner.name = row[1];
        runner.bibnumber = row[2];
        runner.team = row[3];
        runner.lap = row[4].toInt();
        m_runners.insert(row[0].toInt(), runner);
    }
}

bool BasicFileMapper::hasCache() const
{
    QDir dir;
    if (!dir.exists("app")) {
        return false;
    }
    dir.cd("app");
    if (!dir.exists("native")) {
        return false;
    }
    dir.cd("native");
    if (!dir.exists("data")) {
        return false;
    }
    dir.cd("data");
    return dir.exists("BasicFileMapperCache.json");
}

void BasicFileMapper::saveToCache()
{
    QDir dir;
    dir.mkpath("app/native/data");
    dir.cd("app");
    dir.cd("native");
    dir.cd("data");
    QFile file(dir.filePath("BasicFileMapperCache.json"));
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "Error opening cache for writing:" << file.errorString();
    } else {
        file.write(generateCache().toUtf8());
        file.close();
    }
}

void BasicFileMapper::loadCache()
{
    if (!hasCache()) {
        return;
    }
    QDir dir;
    dir.cd("app");
    dir.cd("native");
    dir.cd("data");
    QFile file(dir.filePath("BasicFileMapperCache.json"));
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Error opening cache for reading:" << file.errorString();
    } else {
        readCache(QString::fromUtf8(file.readAll()));
        file.close();
    }
}

void BasicFileMapper::loadPermanentData()
{
    loadRunnersFromXML();
    //loadRunners();

    saveToCache();
}

void BasicFileMapper::loadRunnersFromXML()
{
    QDir dir;
    dir.cd("app");
    dir.cd("native");
    dir.cd("data");
    QFile file(dir.filePath("startlista.xml"));
    if (file.open(QFile::ReadOnly)) {
        qDebug() << "NOTE: Opened startlista.xml";
        qDebug() << "NOTE: The size of startlista.xml is" << file.size();

        QDomDocument doc;
        if(!doc.setContent(&file)) {
            qDebug() << "Could not parse XML file";
            return;
        }
        
        // Toplevel element is <StartList>
        
        QDomElement startListElement = doc.documentElement();
        if(startListElement.tagName() != "StartList" || startListElement.isNull()) {
            qDebug() << "Not found <StartList> element";
            return;
        }
        
        // Now we are looking for a <ClassStart> element (and only one in the case of Klarälvskavlen
        // TODO Make more general for other races
        QDomElement classStartElement = startListElement.firstChildElement("ClassStart");
        if(classStartElement.isNull()) {
            qDebug() << "Not found <ClassStart> element";
            return;
        }

        QDomElement classElement = classStartElement.firstChildElement("Class");
        if (classElement.isNull()) {
            qDebug() << "Not found <Class> element";
            return;
        } else {
            QDomElement raceClassElement = classElement.firstChildElement("RaceClass");
            if (raceClassElement.isNull()) {
                qDebug() << "Not found <RaceClass> element";
                return;
            }

            QDomElement firstStartElement = raceClassElement.firstChildElement("FirstStart");
            if (firstStartElement.isNull()) {
                qDebug() << "Not found <FirstClass> element";
                return;
            }

            Util::instance->setCompetitionTime(QDateTime::fromString(firstStartElement.text(), Qt::ISODate));
        }
        
        // Inside <ClassStart> is a <Class> element that we don't need here, and then one <TeamStart> element per team,
        // loop over all the <TeamStart> elements
        QDomElement teamStartElement = classStartElement.firstChildElement("TeamStart");
        while(!teamStartElement.isNull()) {
            MappedRunner runner;
            int si = 0;
            // From <TeamStart>, we need <Name> and <BibNumber>, plus the SI number, leg and runner's name
            // which are deeper down inside <TeamMemberStart>
            // We need to get <Name> and <BibNumber> first and then iterate over all <TeamMemberStart>
            QDomElement nameElement = teamStartElement.firstChildElement("Name");
            if(!nameElement.isNull()) {
                runner.team = nameElement.text();
                qDebug() << "Found team " << runner.team;
            }
            QDomElement bibnumberElement = teamStartElement.firstChildElement("BibNumber");
            if(!bibnumberElement.isNull()) {
                runner.bibnumber = bibnumberElement.text();
                qDebug() << "Found bib number " << runner.bibnumber;
            }
            QDomElement teamMemberStartElement = teamStartElement.firstChildElement("TeamMemberStart");
            while(!teamMemberStartElement.isNull()) {
                // Inside <TeamMemberStart>, we are looking for <Person> for the name, and for <Start>, and inside that for <Leg> and <ControlCard>
                QDomElement teamMemberStartChildElement = teamMemberStartElement.firstChildElement();
                while(!teamMemberStartChildElement.isNull()) {
                    if(teamMemberStartChildElement.tagName() == "Person") {
                        // Inside <Person>, find <Name>
                        QDomElement nameElement = teamMemberStartChildElement.firstChildElement("Name");
                        if(!nameElement.isNull()) {
                            QString family, given;
                            // Inside <Name>, find <Given> and <Family>
                            QDomElement familyElement = nameElement.firstChildElement("Family");
                            if(!familyElement.isNull()) family = familyElement.text();
                            QDomElement givenElement = nameElement.firstChildElement("Given");
                            if(!givenElement.isNull()) given = givenElement.text();
                            runner.name = QObject::tr("%1 %2").arg(given).arg(family);
                        }
                    } else if(teamMemberStartChildElement.tagName() == "Start") {
                        QDomElement startElement = teamMemberStartChildElement;
                            
                        QDomElement startChildElement = startElement.firstChild().toElement();
                        while(!startChildElement.isNull()) {
                            if(startChildElement.tagName() == "Leg")
                                runner.lap = startChildElement.text().toInt();
                            else if(startChildElement.tagName() == "ControlCard")
                                si = startChildElement.text().toInt();
                            else {
                                    ; // not interested in any other children of <Start>
                                }

                            startChildElement = startChildElement.nextSiblingElement();
                        }
                    } ; // not interested in any children of <TeamMemberStart> other than <Person> and <Start>
                        
                    teamMemberStartChildElement = teamMemberStartChildElement.nextSiblingElement();
                }

//                qDebug() << "Inserting SI " << si << " for Team " << runner.bibnumber << ' ' << runner.team << "(" << runner.name << ", leg " << runner.lap << ")";
                m_runners.insert(si, runner);
                teamMemberStartElement = teamMemberStartElement.nextSiblingElement("TeamMemberStart");
            }
            
            teamStartElement = teamStartElement.nextSiblingElement("TeamStart");
        }
        
    } else {
        qDebug() << "NOTE Error opening startlista.xml:" << qPrintable(file.errorString());
    }
    file.close();
}

void BasicFileMapper::loadRunners()
{
    QDir dir;
    dir.cd("app");
    dir.cd("native");
    dir.cd("data");
    QFile file(dir.filePath("runners.txt"));
    if (file.open(QFile::ReadOnly)) {
        while (!file.atEnd()) {
            const QString line = QString::fromUtf8(file.readLine());
            if (line.startsWith('#') || line.trimmed().isEmpty()) {
                continue;
            }
            MappedRunner runner;
            const int si = line.section(",,,", 0, 0).toInt();
            runner.name = line.section(",,,", 1, 1);
            runner.team = line.section(",,,", 2, 2);
            runner.lap = line.section(",,,", 3, 3).toInt();
            m_runners.insert(si, runner);
        }
    } else {
        qDebug() << "NOTE: Couldn't open file:" << file.errorString();
    }
}

bool BasicFileMapper::map(const int si, const QTime &, QString *name, QString *team, QTime *time, int *lap)
{
    if (!m_runners.contains(si)) {
        return false;
    }

    const MappedRunner runner = m_runners[si];
    *name = runner.name;
    *team = runner.team;
    *lap = runner.lap;
    *time = Util::instance->competitionTime().time();
    return true;
}

QMap<int, QString> BasicFileMapper::runners()
{
    QMap<int, QString> out;
    foreach (const int si, m_runners.keys()) {
        out.insert(si, m_runners[si].name);
    }
    return out;
}

void BasicFileMapper::changeSINumber(const int si, const int newSI)
{
    if (!m_runners.contains(newSI)) {
        m_runners[newSI] = m_runners[si];
        m_runners.remove(si);
        reloadRunners();
        saveToCache();
    } else {
        error("There is already a runner with this SI number. No change will be made.");
    }
}

void BasicFileMapper::changeName(const int si, const QString &newName)
{
    if (m_runners.contains(si)) {
        m_runners[si].name = newName;
        reloadRunners();
        saveToCache();
    } else {
        error("There is no runner with this SI number. No change will be made.");
    }
}

void BasicFileMapper::changeLap(const int si, const int newLap)
{
    if (m_runners.contains(si)) {
        m_runners[si].lap = newLap;
        reloadRunners();
        saveToCache();
    } else {
        error("There is no runner with this SI number. No change will be made.");
    }
}

void BasicFileMapper::removeRunner(const int si)
{
    if (m_runners.contains(si)) {
        m_runners.remove(si);
        reloadRunners();
        saveToCache();
    } else {
        error("There is no runner with this SI number. No change will be made.");
    }
}

void BasicFileMapper::addRunner(const int si, const QString &name, const QString &team, const int lap)
{
    if (m_runners.contains(si)) {
        error("There is already a runner with this SI number. No change will be made.");
        return;
    }

    MappedRunner runner;
    runner.name = name;
    runner.team = team;
    runner.lap = lap;
    m_runners.insert(si, runner);
    reloadRunners();
    saveToCache();
}

int BasicFileMapper::lapForIndex(const int index)
{
    if (index < 0 || m_runners.values().count() < index) {
        return -1;
    }
    return m_runners.values()[index].lap;
}

void BasicFileMapper::invalidateCache()
{
    if (hasCache()) {
        QDir dir;
        dir.cd("app");
        dir.cd("native");
        dir.cd("data");
        dir.remove("BasicFileMapperCache.json");
        m_runners.clear();
        loadPermanentData();
        reloadRunners();
    }
}
