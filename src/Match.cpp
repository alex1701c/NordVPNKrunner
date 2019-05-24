//
// Created by alex on 23.05.19.
//

#include "Match.h"
#include "Config.h"
#include "Status.h"
#include <iostream>

void Match::generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                            KConfigGroup &configGroup, Status &vpnStatus, QString &term) {
    matches.append(createMatch(runner, configGroup, vpnStatus.status, "status", 0.5));     // Status

    std::cout << "Generate Match Options" << std::endl;
    if (term.contains("vpn set") || term.contains("settings")) {
        Config::generateOptions(runner, matches, configGroup, term);
    } else {
        double relevance = 0;
        generateConnectionOptions(runner, matches, term, "", relevance);
    }

}

void Match::runMatch(Plasma::RunnerContext &context, Plasma::QueryMatch &match) {
    Q_UNUSED(context);
    Q_UNUSED(match);

}

void Match::generateConnectionOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                                      const QString &term, const QString &data, double relevance) {
    Q_UNUSED(runner);
    Q_UNUSED(matches);
    Q_UNUSED(term);
    Q_UNUSED(data);
    Q_UNUSED(relevance);
}

Plasma::QueryMatch
Match::createMatch(Plasma::AbstractRunner *runner,
                   KConfigGroup &configGroup, const QString &text, const QString &data, double relevance) {
    Plasma::QueryMatch match(runner);
    match.setIconName(configGroup.readEntry("icon", "/home/alex/Downloads/ico/nordvpn_favicon57x57.png"));
    match.setText(text);
    match.setData(data);
    match.setRelevance(relevance);
    return match;
}
