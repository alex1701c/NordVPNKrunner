#ifndef NORDVPN_MATCH_H
#define NORDVPN_MATCH_H


#include <krunner/runnercontext.h>
#include "Status.h"

class Match {
public:

    static void
    generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches, KConfigGroup &configGroup,
                    Status &vpnStatus, QString &term);

    static void
    generateConnectionOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches,
                              KConfigGroup &configGroup, Status &vpnStatus, QString &term);

    static Plasma::QueryMatch
    createMatch(Plasma::AbstractRunner *runner, KConfigGroup &configGroup, const QString &text, const QString &data,
                double relevance);
};


#endif //NORDVPN_MATCH_H
