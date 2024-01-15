#pragma once

#include "core/Status.h"
#include <KRunner/AbstractRunner>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QIcon>
#include <QMutex>
#include <krunner_version.h>

class NordVPN : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
#if QT_VERSION_MAJOR == 6
    NordVPN(QObject *parent, const KPluginMetaData &pluginMetaData)
        : KRunner::AbstractRunner(parent, pluginMetaData){};
#else
    NordVPN(QObject *parent, const KPluginMetaData &pluginMetaData, const QVariantList &args)
        : KRunner::AbstractRunner(parent, pluginMetaData, args){};
#endif

private:
    QFileSystemWatcher watcher;
    Status vpnStatus;

    const QLatin1String triggerWord{"nordvpn"};
    const QLatin1String shortTriggerWord{"vpn"};

    // For thread sync, only fetch status if the query matches
    // the trigger words and reuse the status for the match session
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
    Q_DISABLE_COPY_MOVE(NordVPN)
};
