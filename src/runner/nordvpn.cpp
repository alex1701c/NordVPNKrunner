#include "nordvpn.h"
#include "core/Status.h"
#include "Match.h"
// KF
#include <QtGui/QtGui>
#include <core/ProcessManager.h>
#include <core/Utilities.h>

void NordVPN::init() {
    setObjectName(QStringLiteral("NordVPN"));
    setPriority(HighestPriority);
    addSyntax(Plasma::RunnerSyntax("vpn us", "Connect option to United States, server is chosen by NordVPN"));
    addSyntax(Plasma::RunnerSyntax("vpn us 3335", "Connect options to United States with server number 3335"));
    addSyntax(Plasma::RunnerSyntax("vpn d", "Shows Disconnect option on top (just relevance changed)"));
    addSyntax(Plasma::RunnerSyntax("vpn reconnect", "Reconnect to the current country and server. "
                                                          "Sometimes you have to do this if you change from a wireless to a wired connection"));

    // Fetch only the status data if the query matches
    connect(&vpnStatus, &Status::finished, this, [this](){
        suspendMatching(false);
    });
    connect(this, &NordVPN::prepare, [=]() {
        suspendMatching(true);
        vpnStatus.updateConnectionStatus();
    });

    // Add file watcher for config
    configFilePath = Utilities::initializeConfigFile();
    watcher.addPath(configFilePath);
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &NordVPN::reloadPluginConfiguration);
    reloadPluginConfiguration();
}

void NordVPN::reloadPluginConfiguration() {
    KConfigGroup config = KSharedConfig::openConfig(configFilePath, KConfig::NoGlobals)->group("Config");
    config.config()->reparseConfiguration();

    // If the file gets edited with a text editor, it often gets replaced by the edited version
    // https://stackoverflow.com/a/30076119/9342842
    watcher.addPath(configFilePath);
    icon = QIcon::fromTheme(config.readEntry("icon", "nordvpn"),
                            QIcon("/var/lib/nordvpn/icon.svg"));
    notify = config.readEntry("notify", true);
}

void NordVPN::match(Plasma::RunnerContext &context) {
    QString term = context.query();

    if (!term.startsWith(shortTriggerWord) && !term.startsWith(triggerWord)) {
        return;
    }
    if (vpnStatus.status == QLatin1String("Error")) {
        return;
    }

    QList<Plasma::QueryMatch> matches;
    const QList<Match> matchList = Match::generateOptions(vpnStatus, term);
    for (const auto &m: matchList) {
        Plasma::QueryMatch match(this);
        match.setText(m.text);
        match.setType(Plasma::QueryMatch::ExactMatch);
        match.setData(m.data);
        match.setRelevance(m.relevance);
        match.setIcon(icon);
        matches.append(match);
    }
    context.addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &/*context*/, const Plasma::QueryMatch &match) {
    QStringList args = match.data().toStringList();
    const QString command = args.takeFirst();

    if (command == QLatin1String("disconnect")) {
        ProcessManager::disconnectVPN(notify, args);
    } else if (command == QLatin1String("status")) {
        ProcessManager::vpnStatus(args);
    } else if (command == QLatin1String("reconnect")) {
        ProcessManager::reconnectVPN(notify, args);
    } else {
        ProcessManager::connectVPN(notify, args);
    }
}

K_EXPORT_PLASMA_RUNNER_WITH_JSON(NordVPN, "plasma-runner-nordvpn.json")

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
