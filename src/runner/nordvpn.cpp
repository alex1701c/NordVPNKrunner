#include "nordvpn.h"
#include "core/Status.h"
#include "Match.h"
// KF
#include <QtGui/QtGui>
#include <core/ProcessManager.h>

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
    connect(this, &NordVPN::prepare, this, &NordVPN::prepareForMatchSession);
}

void NordVPN::prepareForMatchSession() {
    const QString statusData = Status::getRawConnectionStatus(config.readEntry("source", "nordvpn status"));
    if (!statusData.isEmpty()) {
        vpnStatus = Status::objectFromRawData(statusData);
    } else if (vpnStatus.rawData.isEmpty()) {
        vpnStatus = Status::objectFromRawData("Status: Disconnected");
    }
    suspendMatching(vpnStatus.status == QLatin1String("Error"));
}


void NordVPN::match(Plasma::RunnerContext &context) {
    QString term = context.query();

    if (!term.startsWith(shortTriggerWord) && !term.startsWith(triggerWord)) {
        return;
    }

    QList<Plasma::QueryMatch> matches;
    const QList<Match> matchList = Match::generateOptions(vpnStatus, term);
    if (!context.isValid()) {
        return;
    }
    for (const auto &m: matchList) {
        Plasma::QueryMatch match(this);
        match.setText(m.text);
        match.setData(m.data);
        match.setRelevance(m.relevance);
        match.setIconName(config.readEntry("icon", "nordvpn"));
        matches.append(match);
    }
    context.addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    QStringList args = match.data().toStringList();
    const QString command = args.takeFirst();

    const bool notify = config.readEntry("notify", true);
    if (command == QLatin1String("disconnect")) {
        ProcessManager::disconnectVPN(notify, args, config.readEntry("script"));
    } else if (command == QLatin1String("status")) {
        ProcessManager::vpnStatus(args);
    } else if (command == QLatin1String("reconnect")) {
        ProcessManager::reconnectVPN(notify, args, config.readEntry("script"));
    } else {
        // Since a few nordvpn versions the connect command can be used to reconnect to other server
        ProcessManager::connectVPN(notify, args, config.readEntry("script"));
    }
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
