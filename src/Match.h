#include <utility>

#ifndef NORDVPN_MATCH_H
#define NORDVPN_MATCH_H


#include <krunner/runnercontext.h>
#include "Status.h"

class Match {
public:
    QString text;
    QString data;
    float relevance = 0;

    Match() = default;

    Match(QString text, QString data, float relevance) : text(std::move(text)), data(std::move(data)), relevance(relevance) {}

    static QList<Match> generateOptions(Status &vpnStatus, QString &term);

    static QList<Match> generateConnectionOptions(Status &vpnStatus, const KConfigGroup &config, QString &term);
};


#endif //NORDVPN_MATCH_H
