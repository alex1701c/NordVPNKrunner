//
// Created by alex on 22.05.19.
//
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <iostream>
#include "Status.h"

void Status::extractConectionInformation() {
    if (!current_server.isEmpty()) {
        QRegExp regex("Current server: ([a-z]{2})(\\d{1,5})");
        regex.indexIn(current_server);

        QStringList res = regex.capturedTexts();
        if (res.size() == 3) {
            country = res.at(1);
            server = res.at(2);
        }
    }
}

bool Status::connectionExists() {
    // Valid statuses: Connected, Connecting, Reconnecting
    return status.startsWith("Status: Connect") || status == "Status: Reconnecting";
}

QString Status::evalConnectQuery(QString &term, QString target) {
    if (!term.contains("reconnect")) {// No reconnect in term
        QRegExp regex("vpn ([a-zA-Z _]+[\\da-zA-Z_]*)$");// vpn us42; vpn us 42; vpn united_states
        regex.indexIn(term);
        QStringList res = regex.capturedTexts();
        if (res.size() == 2 && !res.at(1).isEmpty()) {
            target = res.at(1).toUpper();
        }
    } else {// Reconnect in term
        QRegExp regexReconnect("vpn reconnect ([a-zA-Z _]+[\\da-zA-Z_]*)$");
        regexReconnect.indexIn(term);
        QStringList reconnectRes = regexReconnect.capturedTexts();
        if (reconnectRes.size() == 2 && !reconnectRes.at(1).isEmpty()) {
            target = reconnectRes.at(1).toUpper();
        }
    }

    return target;
}
