//
// Created by alex on 23.05.19.
//

#include "Match.h"
#include "Config.h"
#include "Status.h"

void Match::generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                            KConfigGroup &configGroup, Status &vpnStatus, QString &term) {

    matches.append(
            createMatch(runner, configGroup,
                        vpnStatus.formatString(configGroup.readEntry("status", "%STATUS")), "status", 0.5));
#ifdef RUNNER_SETTINGS
    if (term.contains("vpn set") || term.contains("settings")) {
        Config::generateOptions(runner, matches, configGroup, term);
    } else {
        generateConnectionOptions(runner, matches, configGroup, vpnStatus, term);
    }
#else
    generateConnectionOptions(runner, matches, configGroup, vpnStatus, term);

#endif
}

void Match::runMatch(Plasma::RunnerContext &context, Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    Q_UNUSED(match)
}

void Match::generateConnectionOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                                      KConfigGroup &configGroup, Status &vpnStatus, QString &term) {
    QString target;
    // Disconnect/Connect options
    if (vpnStatus.connectionExists()) {
        int relevanceDisconnect = 0;
        if (term.contains(QRegExp("vpn d(isconnect)?[ ]*$"))) {
            relevanceDisconnect = 1;
        }
        matches.append(createMatch(runner, configGroup, "Disconnect", "disconnect", relevanceDisconnect));
    } else {
        target = Status::evalConnectQuery(term, configGroup.readEntry("default", "US")).toUpper();
        matches.append(createMatch(
                runner, configGroup, QString("Connect To " + target), QString("nordvpn connect " + target), 1)
        );
    }
    // Reconnect to current/other options
    if (vpnStatus.connectionExists() && (term.startsWith("vpn reconnect") || term.startsWith("nordvpn reconnect"))) {
        target = Status::evalConnectQuery(term, "");
        bool textOnly = target.contains(QRegExp("[a-zA-z ]{2,50}$"));
        bool sameStart = QString((vpnStatus.country + vpnStatus.server)).replace(" ", "").toUpper()
                .startsWith(target.replace(" ", "").toUpper());

        if ((sameStart && (textOnly || target.endsWith(vpnStatus.server))) || target.replace(" ", "").isEmpty()) {
            // [The address from the status startswith the one of the query &&
            // (the server addresses match exactly || no server address specified)] || no targeted address
            if (target.isEmpty()) {
                target = vpnStatus.country + vpnStatus.server;
            } else if (textOnly) {
                target += vpnStatus.server;
            }
            matches.append(createMatch(runner, configGroup, "Reconnect To Current ",
                                       QString("nordvpn d > /dev/null 2>&1 ;nordvpn c  " + target), 1));
        } else {
            matches.append(createMatch(runner, configGroup, QString("Reconnect To " + target),
                                       QString("nordvpn d > /dev/null 2>&1 ;nordvpn c  " + target), 1));
        }
    }
}

Plasma::QueryMatch Match::createMatch(Plasma::AbstractRunner *runner, KConfigGroup &configGroup,
                                      const QString &text, const QString &data, double relevance) {
    Plasma::QueryMatch match(runner);
    match.setIconName(configGroup.readEntry("icon", "/usr/share/icons/nordvpn.png"));
    match.setText(text);
    match.setData(data);
    match.setRelevance(relevance);
    return match;
}
