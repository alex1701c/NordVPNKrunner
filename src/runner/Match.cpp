#include "Match.h"
#include "core/Status.h"
#include <KSharedConfig>
#include <KConfigGroup>
#include <QDebug>
#include <QRegularExpression>

QList<Match> Match::generateOptions(const Status &vpnStatus, const QString &term) {
    QString target;
    QList<Match> matches;
    const auto config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("NordVPN");
     // TODO Handle reconnect different!

    // The status gets always displayed
    matches.append(Match(vpnStatus.formatString(config.readEntry("status", "%STATUS")),
                         {"status", "status"},
                         0.5));

    if (vpnStatus.connectionExists()) {
        int relevanceDisconnect = 0;
        if (term.contains(QRegularExpression("vpn d(isconnect)? *$"))) {
            relevanceDisconnect = 1;
        }
        matches.append(Match("Disconnect", {"disconnect", "d"}, (float) relevanceDisconnect));
    } else {
        // Connect to new
        target = Status::evalConnectQuery(term, config.readEntry("default", "US")).toUpper();
        matches.append(Match("Connect To " + target, {"connect", "connect", target}, 1));
    }
    // Reconnect to current/other options
    if (vpnStatus.connectionExists() && (term.startsWith("vpn reconnect") || term.startsWith("nordvpn reconnect"))) {
        target = Status::evalConnectQuery(term, "");
        const bool countryOnly = target.contains(QRegularExpression("[a-zA-z ]{2,50}$"));
        const bool sameStart = QString((vpnStatus.country + vpnStatus.server)).remove(' ')
                .startsWith(target.remove(' '), Qt::CaseInsensitive);
        const bool emptyTarget = target.remove(' ').isEmpty();

        if (emptyTarget || (sameStart && countryOnly) ||
            target.toLower() == QString(vpnStatus.country + vpnStatus.status).toLower()) {
            // Target is empty or the connection and target country are the same or the connection and target are exactly the same
            if (target.isEmpty()) target = vpnStatus.country + vpnStatus.server;
            else if (countryOnly) target += vpnStatus.server;
            matches.append(Match("Reconnect To Current", {"connect", "connect", target}, 1));
        } else {
            matches.append(Match("Reconnect To " + target, {"connect", "connect", target}, 1));
        }
    }
    return matches;
}