#ifndef NORDVPN_STATUS_H
#define NORDVPN_STATUS_H


#include <QtCore/QString>
#include <QMap>

class Status {
public:
    QString status = "Error";
    QString current_server;
    QString country;
    QString server;

    QMap<QString, QString> rawData;

    void extractConnectionInformation();

    bool connectionExists() const;

    static QString evalConnectQuery(const QString &term, const QString &defaultTarget = "");

    static QString getRawConnectionStatus(const QString &statusSource);

    static Status objectFromRawData(const QString &statusData);

    QString formatString(QString raw) const;

};


#endif //NORDVPN_STATUS_H
