//
// Created by alex on 23.05.19.
//

#ifndef NORDVPN_CONFIG_H
#define NORDVPN_CONFIG_H


#include <QtCore/QString>
#include <KConfigCore/KConfigGroup>
#include <krunner/querymatch.h>

class Config {
public:
    static KConfigGroup getConfigGroup();

    static void generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches, QString &term);

    static void configureOptions(QString &data);
};


#endif //NORDVPN_CONFIG_H
