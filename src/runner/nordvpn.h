#ifndef NORDVPN_H
#define NORDVPN_H

#include <KSharedConfig>
#include <KRunner/AbstractRunner>
#include <QFileSystemWatcher>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QMutex>
#include "core/Status.h"
#include <krunner_version.h>

class NordVPN : public Plasma::AbstractRunner {
Q_OBJECT

public:
#if KRUNNER_VERSION <= QT_VERSION_CHECK(5, 77, 0)
    NordVPN(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args){};
#else
    NordVPN(QObject *parent, const KPluginMetaData &pluginMetaData, const QVariantList &args)
        : Plasma::AbstractRunner(parent, pluginMetaData, args){};
#endif
    ~NordVPN() override;

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

protected Q_SLOTS:

    void init() override;

public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;

    void reloadPluginConfiguration();
};

#endif
