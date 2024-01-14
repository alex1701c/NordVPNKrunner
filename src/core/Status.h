#pragma once

#include <QString>
#include <QHash>
#include <QObject>

class Status : public QObject {
    Q_OBJECT
public:
    using QObject::QObject;
    QString status = "Error";
    QString current_server;
    QString country;
    QString server;

    QHash<QString, QString> rawData;

    bool connectionExists() const;

    static QString evalConnectQuery(const QString &term, const QString &defaultTarget = "");

    void updateConnectionStatus();

    void parseStatusData(const QString &statusData);

    QString formatString(const QString &raw) const;

    Q_SIGNAL void finished();

};

