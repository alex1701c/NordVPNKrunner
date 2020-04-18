#include "Match.h"
#include "Status.h"
#include <KSharedConfig>
#include <KConfigGroup>
#include <QDebug>
#include <QRegularExpression>

QList<Match> Match::generateOptions(const Status &vpnStatus, const QString &term) {
    QList<Match> matches;
    const auto config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("NordVPN");
    matches.append(Match(vpnStatus.formatString(config.readEntry("status", "%STATUS")), "status", 0.5));
    matches.append(generateConnectionOptions(vpnStatus, config, term));
    return matches;
}

QList<Match> Match::generateConnectionOptions(const Status &vpnStatus, const KConfigGroup &config, const QString &term) {
    QString target;
    QList<Match> matches;

    if (vpnStatus.connectionExists()) {
        // Disconnect option always shown, relevance differs
        int relevanceDisconnect = 0;
        if (term.contains(QRegularExpression("vpn d(isconnect)? *$"))) {
            relevanceDisconnect = 1;
        }
        matches.append(Match("Disconnect", "disconnect", (float) relevanceDisconnect));
    } else {
        // Connect to new
        target = Status::evalConnectQuery(term, config.readEntry("default", "US")).toUpper();
        matches.append(Match("Connect To " + target, "nordvpn connect " + target, 1));
    }
    // Reconnect to current/other options
    if (vpnStatus.connectionExists() && (term.startsWith("vpn reconnect") || term.startsWith("nordvpn reconnect"))) {
        target = Status::evalConnectQuery(term, "");
        const bool countryOnly = target.contains(QRegExp("[a-zA-z ]{2,50}$"));
        const bool sameStart = QString((vpnStatus.country + vpnStatus.server)).remove(" ")
                .startsWith(target.remove(' '), Qt::CaseInsensitive);
        const bool emptyTarget = target.remove(' ').isEmpty();

        if (emptyTarget || (sameStart && countryOnly) ||
            target.toLower() == QString(vpnStatus.country + vpnStatus.status).toLower()) {
            // Target is empty or the connection and target country are the same or the connection and target are exactly the same
            if (target.isEmpty()) target = vpnStatus.country + vpnStatus.server;
            else if (countryOnly) target += vpnStatus.server;
            matches.append(Match("Reconnect To Current", "nordvpn d > /dev/null 2>&1 ;nordvpn c  " + target, 1));
        } else {
            matches.append(Match("Reconnect To " + target, "nordvpn d > /dev/null 2>&1 ;nordvpn c  " + target, 1));
        }
    }
    return matches;
}