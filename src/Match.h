//
// Created by alex on 23.05.19.
//

#ifndef NORDVPN_MATCH_H
#define NORDVPN_MATCH_H


#include <krunner/runnercontext.h>

class Match {
public:
    static void runMatch(Plasma::RunnerContext &context, Plasma::QueryMatch &match);

    static void generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                                const QString &text, const QString &data, double relevance);

};


#endif //NORDVPN_MATCH_H
