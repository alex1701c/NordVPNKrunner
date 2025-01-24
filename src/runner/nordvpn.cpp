#include "nordvpn.h"
#include "Match.h"
#include "core/ProcessManager.h"
#include "core/Status.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QFileInfo>

void NordVPN::init()
{
    setObjectName(QStringLiteral("NordVPN"));
    addSyntax("vpn us", "Connect option to United States, server is chosen by NordVPN");
    addSyntax("vpn us 3335", "Connect options to United States with server number 3335");
    addSyntax("vpn d", "Shows Disconnect option on top (just relevance changed)");
    addSyntax("vpn reconnect",
              "Reconnect to the current country and server. "
              "Sometimes you have to do this if you change from a wireless to a wired connection");

    // Fetch only the status data if the query matches
    connect(&vpnStatus, &Status::finished, this, [this]() {
        suspendMatching(false);
    });
    connect(this, &NordVPN::prepare, [=]() {
        suspendMatching(true);
        vpnStatus.updateConnectionStatus();
    });
    reloadConfiguration();
}

void NordVPN::reloadConfiguration()
{
    const KConfigGroup cg = config();
    QIcon fallback{QFileInfo::exists("/var/lib/nordvpn/icon.svg") ? QIcon("/var/lib/nordvpn/icon.svg") : QIcon()};
    icon = QIcon::fromTheme(cg.readEntry("icon", "/usr/share/pixmaps/nordvpn.png"), fallback);
    notify = cg.readEntry("notify", true);
}

void NordVPN::match(KRunner::RunnerContext &context)
{
    QString term = context.query();

    if (!term.startsWith(shortTriggerWord) && !term.startsWith(triggerWord)) {
        return;
    }
    if (vpnStatus.status == QLatin1String("Error")) {
        return;
    }

    QList<KRunner::QueryMatch> matches;
    const QList<Match> matchList = Match::generateOptions(config(), vpnStatus, term);
    for (const auto &m : matchList) {
        KRunner::QueryMatch match(this);
        match.setText(m.text);
        match.setData(m.data);
        match.setRelevance(m.relevance);
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);
        match.setIcon(icon);
        matches.append(match);
    }
    context.addMatches(matches);
}

void NordVPN::run(const KRunner::RunnerContext & /*context*/, const KRunner::QueryMatch &match)
{
    QStringList args = match.data().toStringList();
    const QString command = args.takeFirst();

    if (command == QLatin1String("disconnect")) {
        ProcessManager::disconnectVPN(notify, args);
    } else if (command == QLatin1String("status")) {
        ProcessManager::vpnStatus(config(), args);
    } else if (command == QLatin1String("reconnect")) {
        ProcessManager::reconnectVPN(notify, args);
    } else {
        ProcessManager::connectVPN(notify, args);
    }
}

K_PLUGIN_CLASS_WITH_JSON(NordVPN, "plasma-runner-nordvpn.json")

#include "moc_nordvpn.cpp"
#include "nordvpn.moc"
