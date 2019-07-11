#ifndef NORDVPN_CONFIG_H
#define NORDVPN_CONFIG_H


#include <QtCore/QString>
#include <KConfigCore/KConfigGroup>
#include <krunner/querymatch.h>

class Config {
public:
    static KConfigGroup getConfigGroup();

#ifdef RUNNER_SETTINGS

    static void
        generateOptions(Plasma::AbstractRunner *runner, QList<Plasma::QueryMatch> &matches, KConfigGroup &configGroup,
                        QString &term);

        static void configureOptions(KConfigGroup &configGroup, QString &data);
#endif
};


#endif //NORDVPN_CONFIG_H
