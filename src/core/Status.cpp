#include <QtCore/QRegExp>
#include <KConfigCore/KConfigGroup>
#include <QRegularExpression>
#include <QtCore/QProcess>
#include <KCoreAddons/KMacroExpander>

#include "Status.h"
#include "Utilities.h"

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

    // Target countries have at least two letters
    if (target.isEmpty() || target.size() == 1) {
        return defaultTarget;
    }

    return target;
}

void Status::updateConnectionStatus() {
    QProcess *process = new QProcess();
    process->start("nordvpn", {"status"});
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, process](){
        QString out = QString::fromLocal8Bit(process->readAll());
        out = Utilities::filterBeginning(out);
        if (QString(out).remove('\n') == QLatin1String("Please check your internet connection and try again.")) {
            out = QStringLiteral("Status: No Internet");
        }
        parseStatusData(out);
        process->deleteLater();
    });
}

void Status::parseStatusData(const QString &statusData) {
    rawData.clear();
    status.clear();
    current_server.clear();
    for (const auto &line: statusData.split('\n')) {
        if (line.startsWith(QLatin1String("Status:"))) {
            status = line;
        } else if (line.startsWith(QLatin1String("Current server: "))) {
            current_server = line;
        }
        if (!line.isEmpty() && line.contains(':')) {
            rawData.insert(line.split(':').first().remove(' ').toUpper(), line);
            rawData.insert(line.split(':').first().remove(' ').toLower(),
                                  line.split(':').last().remove(0, 1));
        }
    }
    if (!current_server.isEmpty()) {
        static QRegularExpression regex(QStringLiteral("Current server: ([a-z]{2})(\\d{1,5})"));
        const auto res = regex.match(current_server);
        country = res.captured(1);
        server = res.captured(2);
        rawData.insert("server", country + server);
    }
    Q_EMIT finished();
}

QString Status::formatString(const QString &raw) const {
    return KMacroExpander::expandMacros(raw, rawData).remove(QRegularExpression("%[a-zA-Z]+"));
}
