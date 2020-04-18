#include <utility>

#ifndef NORDVPN_MATCH_H
#define NORDVPN_MATCH_H


#include <krunner/runnercontext.h>
#include "core/Status.h"

class Match {
public:
    QString text;
    QStringList data;
    float relevance = 0;

    Match() = default;

    Match(QString text, QStringList data, float relevance) : text(std::move(text)), data(std::move(data)), relevance(relevance) {}

    static QList<Match> generateOptions(const Status &vpnStatus, const QString &term);
};


#endif //NORDVPN_MATCH_H
