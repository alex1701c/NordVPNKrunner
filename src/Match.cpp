//
// Created by alex on 23.05.19.
//

#include "Match.h"

void Match::generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches, const QString &term,
                            const QString &data, double relevance) {

    if (term.contains("vpn set") || term.contains("settings")) {
        generateConfigOptions(runner, matches, term, data, relevance);
    } else {
        generateConnectionOptions(runner, matches, term, data, relevance);
    }

}

void Match::runMatch(Plasma::RunnerContext &context, Plasma::QueryMatch &match) {
    Q_UNUSED(context);
    Q_UNUSED(match);

}

void Match::generateConnectionOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                                      const QString &term, const QString &data, double relevance) {

}

void Match::generateConfigOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                                  const QString &term, const QString &data, double relevance) {

}
