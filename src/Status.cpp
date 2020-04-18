#include <QtCore/QRegExp>
#include <KConfigCore/KConfigGroup>
#include <QRegularExpression>
#include <QtCore/QProcess>
#include "Status.h"

bool Status::connectionExists() const {
    // Valid statuses: Connected, Connecting, Reconnecting
    return status.startsWith(QLatin1String("Status: Connect")) || status == QLatin1String("Status: Reconnecting");
}

QString Status::evalConnectQuery(const QString &term, const QString &defaultTarget) {
    QString target;
    // Returns extracted target from normal or reconnect queries, rejects disconnect query
    if (!term.contains(QLatin1String("reconnect"))) {
        if (term.contains(QRegularExpression("vpn d[ ]*$")) || term.contains(QLatin1String("vpn di"))) {
            // Rejects for example vpn d, vpn disconnect, vpn dis
            return target;
        }
        static const QRegularExpression regex("vpn ([a-zA-Z _]+[\\da-zA-Z_]*)$");// vpn us42; vpn us 42; vpn united_states
        const auto res = regex.match(term);
        if (res.lastCapturedIndex() == 1 && !res.captured(1).isEmpty()) {
            target = res.captured(1).toUpper();
        }
    } else {// Reconnect in term
        static const QRegularExpression regexReconnect("vpn reconnect ([a-zA-Z _]+[\\da-zA-Z_]*)$");
        const auto res = regexReconnect.match(term);
        if (!res.captured(1).isEmpty()) {
            target = res.captured(1).toUpper();
        }
    }

    if (target.isEmpty() || target.size() == 1) {
        return defaultTarget;
    }

    return target;
}

QString Status::getRawConnectionStatus(const QString &statusSource) {
    QProcess process;
    process.start(statusSource);
    process.waitForFinished(-1);
    static const QRegularExpression cleanupRegex(R"(^[\r\- ]*)");
    QString out = QString::fromLocal8Bit(process.readAllStandardOutput()).remove(cleanupRegex);
    if (QString(out).remove('\n') == QLatin1String("Please check your internet connection and try again.")) {
        return QStringLiteral("Status: No Internet");
    }
    return out;
}

Status Status::objectFromRawData(const QString &statusData) {
    Status status;
    for (const auto &line: statusData.split('\n')) {
        if (line.startsWith(QLatin1String("Status:"))) {
            status.status = line;
        } else if (line.startsWith(QLatin1String("Current server: "))) {
            status.current_server = line;
        }
        if (!line.isEmpty() && line.contains(':')) {
            status.rawData.insert('%' + line.split(':').first().remove(' ').toUpper(), line);
            status.rawData.insert('%' + line.split(':').first().remove(' ').toLower(),
                                  line.split(':').last().remove(0, 1));
        }
    }
    if (!status.current_server.isEmpty()) {
        static QRegularExpression regex(QStringLiteral("Current server: ([a-z]{2})(\\d{1,5})"));
        const auto res = regex.match(status.current_server);
        status.country = res.captured(1);
        status.server = res.captured(2);
    }

    return status;
}

QString Status::formatString(QString raw) const {
    // TODO Use KDE Macro exapnder
    for (const auto &key: rawData.keys()) {
        if (raw.contains(key)) {
            raw.replace(key, rawData.value(key));
        }
    }
    raw.replace("%server", country.toUpper() + server);
    raw.replace("%st", rawData.value("%STATUS"));
    raw.remove(QRegularExpression("%[a-zA-Z]+"));
    return raw;
}
