#ifndef NORDVPN_STATUS_H
#define NORDVPN_STATUS_H


#include <QtCore/QString>
#include <QHash>

class Status {
public:
    QString status = "Error";
    QString current_server;
    QString country;
    QString server;

    QHash<QString, QString> rawData;

    bool connectionExists() const;

    static QString evalConnectQuery(const QString &term, const QString &defaultTarget = "");

    static QString getRawConnectionStatus(const QString &statusSource);

    static Status objectFromRawData(const QString &statusData);

    QString formatString(const QString &raw) const;

};


#endif //NORDVPN_STATUS_H
