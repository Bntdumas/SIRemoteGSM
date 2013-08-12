#include "basicfilemapper.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>

BasicFileMapper::BasicFileMapper(QObject *parent) :
    RunnerMapper(parent)
{
    //loadRunnersFromXML();
    loadRunners();
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
