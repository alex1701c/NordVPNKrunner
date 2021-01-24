#include "nordvpn.h"
#include "core/Status.h"
#include "Match.h"
// KF
#include <QtGui/QtGui>
#include <core/ProcessManager.h>
#include <core/Utilities.h>

NordVPN::NordVPN(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName(QStringLiteral("NordVPN"));
    setSpeed(NormalSpeed);
    setPriority(HighestPriority);
    setIgnoredTypes(Plasma::RunnerContext::Directory |
                    Plasma::RunnerContext::File |
                    Plasma::RunnerContext::NetworkLocation);
}

NordVPN::~NordVPN() = default;

void NordVPN::init() {
    QList<Plasma::RunnerSyntax> syntaxes;
    syntaxes.append(Plasma::RunnerSyntax("vpn us", "Connect option to United States, server is chosen by NordVPN"));
    syntaxes.append(Plasma::RunnerSyntax("vpn us 3335", "Connect options to United States with server number 3335"));
    syntaxes.append(Plasma::RunnerSyntax("vpn d", "Shows Disconnect option on top (just relevance changed)"));
    syntaxes.append(Plasma::RunnerSyntax("vpn reconnect", "Reconnect to the current country and server. "
                                                          "Sometimes you have to do this if you change from a wireless to a wired connection"));
    setSyntaxes(syntaxes);

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
    source = config.readEntry("source", "nordvpn status");
    icon = QIcon::fromTheme(config.readEntry("icon", "nordvpn"),
                            QIcon("/var/lib/nordvpn/icon.svg"));
    notify = config.readEntry("notify", true);
    changeScript = config.readEntry("script");
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

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    QStringList args = match.data().toStringList();
    const QString command = args.takeFirst();

    if (command == QLatin1String("disconnect")) {
        ProcessManager::disconnectVPN(notify, args, changeScript);
    } else if (command == QLatin1String("status")) {
        ProcessManager::vpnStatus(args);
    } else if (command == QLatin1String("reconnect")) {
        ProcessManager::reconnectVPN(notify, args, changeScript);
    } else {
        ProcessManager::connectVPN(notify, args, changeScript);
    }
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
