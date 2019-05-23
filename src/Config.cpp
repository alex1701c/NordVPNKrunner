//
// Created by alex on 23.05.19.
//

#include "Config.h"
#include <iostream>
#include <QtGui/QtGui>
#include <KSharedConfig>


void Config::configureOptions(QString &data) {
    Q_UNUSED(data);
}

void Config::generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches, QString &term) {
    Q_UNUSED(matches);
    Q_UNUSED(runner);
    if (term.contains(QRegExp("^(nord)?vpn set defaults ?$"))) {
        std::cout << "SET DEFAULTS" << std::endl;
    }
}

KConfigGroup Config::getConfigGroup() {
    KConfigGroup vpnConfigGroup = KSharedConfig::openConfig("krunnerrc")->group("Runners");
    return KConfigGroup(&vpnConfigGroup, "NordVPN");
}
