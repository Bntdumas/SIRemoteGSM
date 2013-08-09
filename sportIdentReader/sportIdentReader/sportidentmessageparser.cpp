#include "sportidentmessageparser.h"
#include <QDataStream>
#include <QDebug>

bool SportIdentMessageParser::parse(const QByteArray& inMsg, SportIdentMessage& out)
{
    out.cardNumber = 0;
    out.stationID = 0;
    out.cardSeries = 0;
    out.punchTime = QTime(0,0,0);

    int inPos = 0;
    if(inPos>=inMsg.count()) return false; // nothing left to read
    quint8 chRead;
    chRead = inMsg.at(inPos++); // read first byte

    // A byte has been read; process it.
    if(chRead==STX && inPos < inMsg.count()) // expected byte came in, and there's more to read
    {
        chRead = inMsg.at(inPos++); // read next byte which tells us what kind of data is coming up
        switch(chRead)
        {
        case 0xD3:{//Control Punch, extended mode.
            QByteArray bf; bf.resize(32);
            bf[0]=chRead;
            bf.insert(1, inMsg.mid(inPos,17)); // copy 17 bytes from the incoming data to this new array that is CRC-checked
            if(checkCRC(bf))
            {
                QDataStream bfStream(bf); // defaults to BigEndian
                bfStream.skipRawData(2); // Skip bytes 0 (mode identifier) and 1 (unused?)

                quint16 station;
                bfStream >> station; // station ID is in bytes 2 and 3

                // We need to look ahead at byte 5 to find out card series which in turn says how to get the card no
                quint8 series = bf[5];

                quint32 card = 0;
                if(series<=4 && series>=1) {
                    // stream is still at 4, skip ahead two to get to 6/7 where the number is
                    bfStream.skipRawData(2);
                    quint16 shortCard;
                    bfStream >> shortCard;
                    card = shortCard+100000*series;
                } else {
                    // more modern cards that have number in four consecutive bytes starting at pos 4
                    bfStream >> card;
                }

                // now read punch time. The next byte says whether we are AM or PM
                quint8 ampm;
                bfStream >> ampm;
                // and byte 9 and 10 specify the seconds past noon or midnight
                quint16 seconds;
                bfStream >> seconds;
                out.punchTime = out.punchTime.addSecs(seconds);
                if(ampm&0x01) out.punchTime = out.punchTime.addSecs(3600*12); // PM; add twelve hours

                // set the card number, series and station ID
                out.cardNumber = card;
                out.stationID = station;
                out.cardSeries = series;

                return true;
            }
            break;
        }
        case 0x53:{ //Control Punch, old mode
            QByteArray bf; bf.resize(1);
            bf[0]=chRead;
            // Copy data (max 30 bytes) from inMsg to bf, erasing all the DLEs
            for(int i = 0; i < 30; i++) {
                if(inPos >= inMsg.count()) break;
                quint8 byte = inMsg[inPos++];
                if(byte==DLE) continue;
                bf.append(byte);
            }

            // We expect to have at least 9 bytes available now
            Q_ASSERT(bf.count() >= 9);

            quint8 station = bf[2];
            quint8 series = bf[3];

            QDataStream bfStream(bf); // defaults to BigEndian
            bfStream.skipRawData(4); // Skip bytes 0 (mode identifier), 1 (unused?), 2 (station), 3 (series)

            quint16 card;
            bfStream >> card; // bytes 4 and 5

            quint32 dcard = 0;

            if(series==1)
                dcard=card;
            else if(series<=4)
                dcard=card+100000*series;
            else {
                dcard = series << 16;
                dcard += card;
            }

            // Skip byte 6
            bfStream.skipRawData(1);

            quint16 seconds;
            bfStream >> seconds;

            quint8 ampm = bf[1];
            out.punchTime = out.punchTime.addSecs(seconds);
            //           qDebug() << "adding " << seconds << " seconds";
            //           qDebug() << "out.punchTime = " << out.punchTime;
            if(ampm&0x01) out.punchTime = out.punchTime.addSecs(3600*12); // PM; add twelve hours

            //            qDebug() << "out.punchTime = " << out.punchTime;

            // set the card number, series and station ID
            out.cardNumber = dcard;
            out.stationID = station;
            out.cardSeries = series;

            return true;
        }
        case 0xE7:{//SI5/6 removed
#ifdef KDAB_TEMPORARILY_REMOVED
            BYTE bf[32];
            ReadBytes(bf+1, 10,  hComm);
#endif
            break;
        }

        case 0xE6:{
#ifdef KDAB_TEMPORARILY_REMOVED
            BYTE bf[32];
            bf[0]=0xE6;
            ReadBytes(bf+1, 10,  hComm);

            //ReadByte(chRead); //ETX!
            if(CheckCRC(LPBYTE(bf)))
                GetSI6DataExt(hComm);

#endif
            break;
        }
        case 0xE5:{
#ifdef KDAB_TEMPORARILY_REMOVED
            BYTE bf[32];
            bf[0]=0xE5;
            ReadBytes(bf+1, 10,  hComm);

            if(CheckCRC(LPBYTE(bf)))
                GetSI5DataExt(hComm);

#endif
            break;
        }
        case 0x46:
#ifdef KDAB_TEMPORARILY_REMOVED
            ReadByte(chRead,  hComm); //0x49?!
            ReadByte(chRead,  hComm); //ETX!
            GetSI5Data(hComm);
#endif
            break;
        case 0x66:{ //STX, 66h, CSI, TI, TP, CN3, CN2, CN1, CN0, ETX
#ifdef KDAB_TEMPORARILY_REMOVED
            BYTE bf[32];
            //SICard5Detect si5;
            //si5.code=0xE5;
            bf[0]=0xE6;
            ReadBytesDLE(bf+1, 8,  hComm);

            GetSI6Data(hComm);

#endif
            break;
        }
        case 0xB1:{
#ifdef KDAB_TEMPORARILY_REMOVED
            BYTE bf[200];
            bf[0]=chRead;
            ReadBytes(bf+1, 200,  hComm);
            //GetSI5DataExt(hComm);
            MessageBox(NULL, "Programmera stationen utan AUTOSEND!", NULL, MB_OK);
#endif
            break;
        }

        case 0xE1:{
#ifdef KDAB_TEMPORARILY_REMOVED
            BYTE bf[200];
            bf[0]=chRead;
            ReadBytes(bf+1, 200,  hComm);
            MessageBox(NULL, "Programmera stationen utan AUTOSEND!", NULL, MB_OK);
#endif
            break;
        }

        case 0xE8:{
#ifdef KDAB_TEMPORARILY_REMOVED
            BYTE bf[32];
            bf[0]=0xE8;
            ReadBytes(bf+1, 10,  hComm);

            //ReadByte(chRead); //ETX!
            if(CheckCRC(LPBYTE(bf)))
                GetSI9DataExt(hComm);

#endif
            break;
        }
        }

    }

    return false;
}

bool SportIdentMessageParser::checkCRC(const QByteArray& bf)
{
    quint32 len=bf[1];
    quint16 crc=calcCRC(bf, len+2);

    quint8 crcbyte1 = bf[len+2];
    quint8 crcbyte2 = bf[len+3];

    quint16 crcbytes = (crcbyte1<<8)+crcbyte2;

    qDebug("%x %x %x %x",crcbyte1, crcbyte2, crcbytes, crc);

    return crcbytes == crc;
}

quint16 SportIdentMessageParser::calcCRC(const QByteArray& data, quint32 count)
{
    // Return 0 for no or one data byte
    if (count<2)
        return 0;

    quint32 index=0;
    quint8 byte1 = data[index];
    quint8 byte2 = data[index+1];
    quint16 crc = (byte1<<8)+byte2;
//    quint16 crc = (quint16(data[index])<<8) + quint16(data[index+1]);
    index +=2;
    // Return crc for two data bytes
    if (count==2)
        return crc;

    quint16 value;
    for (size_t k = count>>1; k>0; k--) {
        if (k>1) {
            quint8 pos1 = data[index];
            quint8 pos2 = data[index+1];
            value = (pos1<<8)+pos2;
            index +=2;
        }
        else  // If the number of bytes is odd, complete with 0.
            value = (count&1) ? data[index]<<8 : 0;

        for (int j = 0; j<16; j++) {
            if (crc & 0x8000) {
                crc  <<= 1;
                if (value & 0x8000)
                    crc++;
                crc ^= 0x8005;
            }
            else {
                crc  <<= 1;
                if (value & 0x8000)
                    crc++;
            }
            value <<= 1;
        }
    }
    return crc;
}
