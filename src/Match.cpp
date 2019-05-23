//
// Created by alex on 23.05.19.
//

#include "Match.h"

void Match::generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches, const QString &text,
                            const QString &data, double relevance) {
    Q_UNUSED(runner);
    Q_UNUSED(matches);
    Q_UNUSED(text);
    Q_UNUSED(data);
    Q_UNUSED(relevance);

}

void Match::runMatch(Plasma::RunnerContext &context, Plasma::QueryMatch &match) {
    Q_UNUSED(context);
    Q_UNUSED(match);

}
