#include "nordvpn.h"
#include "core/Status.h"
#include "Match.h"
// KF
#include <KLocalizedString>
#include <QtGui/QtGui>
#include <KSharedConfig>
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

    // TODO Reqrite using QProcess
    const QString payload = match.data().toString();
    const QString iconPath = config.readEntry("icon", "nordvpn");
    const QString changeScript = config.readEntry("script");
    QString cmd;

    const bool notify = config.readEntry("notify", true);
    QString notifyPipes(R"( | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon <ICON> )");
    if (!notify) notifyPipes = " 2>&1 > /dev/null";
    if (payload == QLatin1String("disconnect")) {
        ProcessManager::disconnectVPN(notify);
        return;
    } else if (payload == QLatin1String("status")) {
        cmd = QString("$(vpnStatus=$(nordvpn status 2>&1 | grep -i -E '%1');notify-send  \"$vpnStatus\"  --icon <ICON> ) ")
                .arg(config.readEntry("status_keys", "Status|Current server|Transfer|IP"));
    } else if (payload.startsWith(QLatin1String("nordvpn connect"))){
        ProcessManager::connectVPN(notify, payload.split(' ').last());
        return;
    } else {
        // TODO Handle reconnect
        cmd = "$( " + payload + notifyPipes + "; <SCRIPT>  )";// Disconnect or connect
    }
    cmd = cmd.replace("<ICON>", iconPath).replace("<SCRIPT>", changeScript);
#pragma GCC diagnostic ignored "-Wunused-result"
    system(qPrintable(cmd + " 2>&1 &"));
#pragma GCC diagnostic pop
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
