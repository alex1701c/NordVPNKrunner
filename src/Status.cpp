//
// Created by alex on 22.05.19.
//
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
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
