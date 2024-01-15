#pragma once

#include "core/Status.h"

class Match
{
public:
    QString text;
    QStringList data;
    float relevance = 0;

    Match(const QString &text, const QStringList &data, float relevance)
        : text(text)
        , data(data)
        , relevance(relevance)
    {
    }

    static QList<Match> generateOptions(const Status &vpnStatus, const QString &term);
};
