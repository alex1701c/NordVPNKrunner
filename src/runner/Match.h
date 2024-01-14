#pragma once

#include "core/Status.h"
#include <utility>

class Match
{
public:
    QString text;
    QStringList data;
    float relevance = 0;

    Match() = default;

    Match(QString text, QStringList data, float relevance)
        : text(std::move(text))
        , data(std::move(data))
        , relevance(relevance)
    {
    }

    static QList<Match> generateOptions(const Status &vpnStatus, const QString &term);
};
