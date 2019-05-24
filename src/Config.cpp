//
// Created by alex on 23.05.19.
//

#include "Config.h"
#include "Match.h"
#include <iostream>
#include <QtGui/QtGui>
#include <KSharedConfig>


void Config::configureOptions(QString &data) {
    Q_UNUSED(data);
}

void Config::generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                             KConfigGroup &configGroup, QString &term) {
    Q_UNUSED(configGroup);

    if (term.contains(QRegExp("^(nord)?vpn set defaults ?$"))) {
        std::cout << "SET DEFAULTS" << std::endl;
        matches.append(Match::createMatch(runner, configGroup, "Set sll settings to defaults", "status", 1));
    }
}

KConfigGroup Config::getConfigGroup() {
    KConfigGroup vpnConfigGroup = KSharedConfig::openConfig("krunnerrc")->group("Runners");
    return KConfigGroup(&vpnConfigGroup, "NordVPN");
}
