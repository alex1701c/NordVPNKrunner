//
// Created by alex on 22.05.19.
//
#ifndef NORDVPN_STATUS_H
#define NORDVPN_STATUS_H


#include <QtCore/QString>

class Status {
public:
    QString status = "Error";
    QString current_server = "";
    QString country = "US";
    QString server = "";

    void extractConectionInformation();

    bool connectionExists();

    static QString evalConnectQuery(QString &term, QString target = "");

    static void evalSettings(QString &term);


};


#endif //NORDVPN_STATUS_H
