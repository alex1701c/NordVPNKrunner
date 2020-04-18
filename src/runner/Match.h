#include <utility>

#ifndef NORDVPN_MATCH_H
#define NORDVPN_MATCH_H


#include <krunner/runnercontext.h>
#include "core/Status.h"

class Match {
public:
    QString text;
    QString data;
    float relevance = 0;

    Match() = default;

    Match(QString text, QString data, float relevance) : text(std::move(text)), data(std::move(data)), relevance(relevance) {}

    static QList<Match> generateOptions(const Status &vpnStatus, const QString &term);

    static QList<Match> generateConnectionOptions(const Status &vpnStatus, const KConfigGroup &config, const QString &term);
};


#endif //NORDVPN_MATCH_H
