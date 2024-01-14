#ifndef NORDVPN_H
#define NORDVPN_H

#include "core/Status.h"
#include <KRunner/AbstractRunner>
#include <KSharedConfig>
#include <QFileSystemWatcher>
#include <QMutex>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <krunner_version.h>

class NordVPN : public Plasma::AbstractRunner
{
    Q_OBJECT

public:
    NordVPN(QObject *parent, const KPluginMetaData &pluginMetaData, const QVariantList &args)
        : Plasma::AbstractRunner(parent, pluginMetaData, args){};

    QFileSystemWatcher watcher;
    Status vpnStatus;

    const QLatin1String triggerWord = QLatin1String("nordvpn");
    const QLatin1String shortTriggerWord = QLatin1String("vpn");

    // For thread sync, only fetch status if the query matches
    // the trigger words and reuse the status for the match session
    bool newMatchSession = false;
    QMutex mutex;

    // Config stuff
    QString configFilePath;
    QIcon icon;
    bool notify;

protected:
    void init() override;

public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;

    void reloadPluginConfiguration();
};

#endif
