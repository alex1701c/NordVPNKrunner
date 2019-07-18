#include <QtCore/QRegExp>
#include <KConfigCore/KSharedConfig>
#include <KConfigCore/KConfigGroup>
#include <QtCore/QProcess>
#include "Status.h"

void Status::extractConnectionInformation() {
    if (status == "Status: Disconnected" || status == "Status: No Internet") {
        country = "";
        server = "";
        return;
    }
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

QString Status::evalConnectQuery(QString &term, QString defaultTarget) {
    QString target;
    // Returns extracted target from normal or reconnect queries, rejects disconnect query
    if (!term.contains("reconnect")) {
        if (term.contains(QRegExp("vpn d[ ]*$")) || term.contains("vpn di")) {
            // Rejects for example vpn d, vpn disconnect, vpn dis
            return target;
        }
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
        if (!reconnectRes.at(1).isEmpty()) {
            target = reconnectRes.at(1).toUpper();
        }
    }

    if (target.isEmpty() || target.size() == 1) return defaultTarget;

    return target;
}

QString Status::getRawConnectionStatus(const QString &statusSource) {
    QProcess process;
    process.start(statusSource);
    process.waitForFinished(-1);
    QString out = process.readAllStandardOutput();
    if (QString(out).replace('\n', "") == "Please check your internet connection and try again.") {
        return "Status: No Internet";
    }
    return out;
}

Status Status::objectFromRawData(const QString &statusData) {
    Status status;
    for (const auto &line:statusData.split('\n')) {
        if (line.startsWith("Status:")) {
            status.status = line;
        } else if (line.startsWith("Current server: ")) {
            status.current_server = line;
        }
        if (!line.isEmpty() && line.contains(':')) {
            status.rawData.insert("%" + line.split(':').first().replace(" ", "").toUpper(), line);
            status.rawData.insert("%" + line.split(':').first().replace(" ", "").toLower(),
                                  line.split(':').last().remove(0, 1));
        }
    }
    status.extractConnectionInformation();
    return status;
}

QString Status::formatString(QString raw) {
    for (const auto &key:rawData.keys()) {
        if (raw.contains(key)) {
            raw.replace(key, rawData.value(key));
        }
    }
    raw.replace("%server", country.toUpper() + server);
    raw.replace("%st", rawData.value("%STATUS"));
    raw.replace(QRegExp("%[a-zA-Z]+"), "");
    return raw;
}
