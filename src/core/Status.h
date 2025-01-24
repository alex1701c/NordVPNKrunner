#pragma once

#include <QHash>
#include <QObject>
#include <QString>

class Status : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    QString status = QStringLiteral("Error");
    QString current_server;
    QString country;
    QString server;

    QHash<QString, QString> rawData;

    bool connectionExists() const;

    static QString evalConnectQuery(const QString &term, const QString &defaultTarget = QString());

    void updateConnectionStatus();

    void parseStatusData(const QString &statusData);

    QString formatString(const QString &raw) const;

    Q_SIGNAL void finished();
};
