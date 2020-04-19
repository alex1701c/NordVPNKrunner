#ifndef NORDVPN_H
#define NORDVPN_H

#include <KSharedConfig>
#include <KRunner/AbstractRunner>
#include <QFileSystemWatcher>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include "core/Status.h"

class NordVPN : public Plasma::AbstractRunner {
Q_OBJECT

public:
    NordVPN(QObject *parent, const QVariantList &args);

    ~NordVPN() override;

    QFileSystemWatcher watcher;
    Status vpnStatus;

    const QLatin1String triggerWord = QLatin1String("nordvpn");
    const QLatin1String shortTriggerWord = QLatin1String("vpn");

    // Config stuff
    QString configFilePath;
    QString source;
    QIcon icon;
    bool notify;
    QString changeScript;

protected Q_SLOTS:

    void init() override;
    void prepareForMatchSession();


public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;

    void reloadPluginConfiguration();
};

#endif
