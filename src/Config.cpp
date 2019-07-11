#include "Config.h"
#include "Match.h"
#include <QtGui/QtGui>
#include <KSharedConfig>

#ifdef RUNNER_SETTINGS
void Config::configureOptions(KConfigGroup &configGroup, QString &data) {
    if (data == "defaults") {
        for (const auto &key : configGroup.keyList()) {
            configGroup.deleteEntry(key);
        }
    } else if (data.startsWith("default")) {
        for (const auto &key: data.replace("default", "").split(' ')) {
            if (!key.isEmpty()) {
                configGroup.deleteEntry(key);
            }
        }
    } else if (data.startsWith("set")) {
        QRegExp payload("set[ ]+([a-z_]+)[ ]+(.*)$");
        payload.indexIn(data);
        if (payload.captureCount() == 2) {
            QString key = payload.capturedTexts().at(1);
            QString value = payload.capturedTexts().at(2);
            if (!value.isEmpty()) {
                configGroup.writeEntry(key, value);
            }
        }
    }
    configGroup.sync();

}

void Config::generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                             KConfigGroup &configGroup, QString &term) {
    Q_UNUSED(configGroup);

    if (term.contains(QRegExp("^(nord)?vpn set defaults ?$"))) {
        matches.append(Match::createMatch(runner, configGroup, "Set all settings to defaults", "settings|defaults", 1));
    } else if (term.contains(QRegExp("^(nord)?vpn set .+ default[ ]*$"))) {
        QString selected = term.split("vpn set ").last().split("default").at(0);
        if (selected.contains('*')) {
            matches.append(
                    Match::createMatch(runner, configGroup, "Set all settings to defaults", "settings|defaults", 1));
        } else {
            matches.append(Match::createMatch(runner, configGroup, QString("Set " + selected + "to default"),
                                              QString("settings|default  " + selected), 1));
        }
    } else if (term.contains(QRegExp("^(nord)?vpn set .+$"))) {
        QString selected = term.split("vpn set ").last();
        QString first = selected.split(' ').first();
        matches.append(
                Match::createMatch(
                        runner, configGroup, QString(
                                "Set " + first + " to" + selected.replace(selected.indexOf(first), first.size(), "")),
                        QString("settings|set  " + selected), 1
                )
        );
    }
}
#endif

KConfigGroup Config::getConfigGroup() {
    KConfigGroup vpnConfigGroup = KSharedConfig::openConfig("krunnerrc")->group("Runners");
    return KConfigGroup(&vpnConfigGroup, "NordVPN");
}
