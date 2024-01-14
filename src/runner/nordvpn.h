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

class NordVPN : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    NordVPN(QObject *parent, const KPluginMetaData &pluginMetaData, const QVariantList &args)
        : KRunner::AbstractRunner(parent, pluginMetaData, args){};

private:
    QFileSystemWatcher watcher;
    Status vpnStatus;

    const QLatin1String triggerWord{"nordvpn"};
    const QLatin1String shortTriggerWord{"vpn"};

    // For thread sync, only fetch status if the query matches
    // the trigger words and reuse the status for the match session
    bool newMatchSession = false;
    QMutex mutex;

    // Config stuff
    QString configFilePath;
    QIcon icon;
    bool notify = true;

public: // AbstractRunner API
    void init() override;
    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;
    void reloadPluginConfiguration();
};

#endif
