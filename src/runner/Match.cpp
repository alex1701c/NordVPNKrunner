#include "Match.h"
#include "core/Status.h"
#include <KConfigGroup>
#include <KSharedConfig>
#include <QRegularExpression>
#include <core/Utilities.h>

QList<Match> Match::generateOptions(const Status &vpnStatus, const QString &term)
{
    QString target;
    QList<Match> matches;
    const auto config = KSharedConfig::openConfig(Utilities::initializeConfigFile())->group("Config");

    // The status gets always displayed
    matches.append(Match(vpnStatus.formatString(config.readEntry("status", "%STATUS")), {"status", "status"}, 0.5));

    const bool connectionExists = vpnStatus.connectionExists();
    if (connectionExists) {
        int relevanceDisconnect = 0;
        const static QRegularExpression disconnectRegex("vpn d(isconnect)? *$");
        const bool disconnectQuery = term.contains(disconnectRegex);
        if (disconnectQuery) {
            relevanceDisconnect = 1;
        }
        matches.append(Match("Disconnect", {"disconnect", "d"}, (float)relevanceDisconnect));
        if (disconnectQuery) {
            // Early return, not required but one case less to worry about ;-)
            return matches;
        }
    }

    // Connect option if there is no connection (might be default target), or other target
    target = Status::evalConnectQuery(term, QString());
    const bool targetSpecified = !target.isEmpty();
    const bool differentConnect = (targetSpecified && !Utilities::sameTarget(target, vpnStatus));
    if (!connectionExists || differentConnect) {
        if (target.isEmpty()) {
            target = config.readEntry("default", "us");
        }
        matches.append(Match("Connect To " + target, {"connect", "connect", target}, 1));
    }

    // The target is the same and the keywords match
    if (!differentConnect && (term.startsWith("vpn reconnect") || term.startsWith("nordvpn reconnect"))) {
        target = vpnStatus.country + vpnStatus.server;
        matches.append(Match("Reconnect To Current", {"reconnect", "connect", target}, 1));
    }
    return matches;
}
