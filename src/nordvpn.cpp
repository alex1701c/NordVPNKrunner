#include "nordvpn.h"
#include "Status.h"
#include "Match.h"
// KF
#include <KLocalizedString>
#include <QtGui/QtGui>
#include <KSharedConfig>

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
    connect(this, &NordVPN::teardown, this, &NordVPN::matchSessionFinished);
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

void NordVPN::matchSessionFinished() {
    if (!wasActive) return;
    if (config.readEntry("clean_history", true)) {
        wasActive = false;
        QString history = config.parent().parent().group("General").readEntry("history");
        const int historySize = history.size();
        const QString filteredHistory = history.remove(QRegularExpression(R"((?:nord)?vpn d(?:isconnect)?[^ek],?)"));
        if (filteredHistory.size() == historySize) return;

        QFile f(QDir::homePath() + "/.config/krunnerrc");
        if (f.open(QIODevice::ReadWrite)) {
            QString s;
            QTextStream t(&f);
            while (!t.atEnd()) {
                const QString line = t.readLine();
                if (!line.startsWith(QLatin1String("history"))) {
                    s.append(line + "\n");
                } else {
                    s.append("history=" + filteredHistory + "\n");
                }
            }
            f.resize(0);
            f.write(s.toLocal8Bit());
            f.close();
        }
    }
}


void NordVPN::match(Plasma::RunnerContext &context) {
    if (!context.isValid()) return;
    QString term = context.query();

    if (!term.startsWith(shortTriggerWord) && !term.startsWith(triggerWord)) {
        return;
    }

    QList<Plasma::QueryMatch> matches;
    const QList<Match> matchList = Match::generateOptions(vpnStatus, term);
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

    const QString payload = match.data().toString();
    const QString iconPath = config.readEntry("icon", "nordvpn");
    const QString changeScript = config.readEntry("script");
    QString cmd;

    const bool notify = config.readEntry("notify", true);
    QString notifyPipes(R"( | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon <ICON> )");
    if (!notify) notifyPipes = " 2>&1 > /dev/null";
    if (payload == QLatin1String("disconnect")) {
        wasActive = true;
        cmd = R"($( nordvpn d |head -1 PIPES; <SCRIPT>) )";
        cmd.replace("PIPES", notifyPipes);
    } else if (payload == QLatin1String("status")) {
        cmd = QString("$(vpnStatus=$(nordvpn status 2>&1 | grep -i -E '%1');notify-send  \"$vpnStatus\"  --icon <ICON> ) ")
                .arg(config.readEntry("status_keys", "Status|Current server|Transfer|IP"));
    } else {
        cmd = "$( " + payload + notifyPipes + "; <SCRIPT>  )";// Disconnect or connect
    }
    cmd = cmd.replace("<ICON>", iconPath).replace("<SCRIPT>", changeScript);
    system(qPrintable(cmd + " 2>&1 &"));
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
