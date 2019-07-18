#include "nordvpn.h"
#include "Status.h"
#include "Match.h"
// KF
#include <KLocalizedString>
#include <QtGui/QtGui>
#include <KSharedConfig>

NordVPN::NordVPN(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName("NordVPN");
    setSpeed(NormalSpeed);
    setPriority(HighestPriority);
    setHasRunOptions(true);
    setIgnoredTypes(Plasma::RunnerContext::Directory |
                    Plasma::RunnerContext::File |
                    Plasma::RunnerContext::NetworkLocation);
}

NordVPN::~NordVPN() = default;

void NordVPN::init() {
    reloadConfiguration();
    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
    connect(this, SIGNAL(teardown()), this, SLOT(matchSessionFinished()));
}

void NordVPN::reloadConfiguration() {
    config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("NordVPN");
    statusSource = config.readEntry("source", "nordvpn status");

    //region syntax
    QList<Plasma::RunnerSyntax> syntaxes;
    syntaxes.append(Plasma::RunnerSyntax("vpn us", "Connect option to United States, server is chosen by NordVPN"));
    syntaxes.append(Plasma::RunnerSyntax("vpn us 3335", "Connect options to United States with server number 3335"));
    syntaxes.append(
            Plasma::RunnerSyntax("vpn germany berlin", "Can connect to country by name and city, case insensitive"));
    syntaxes.append(Plasma::RunnerSyntax("vpn d", "Shows Disconnect option on top (just relevance changed)"));
    syntaxes.append(Plasma::RunnerSyntax("vpn reconnect", "Reconnect to the current country and server. "
                                                          "Sometimes you have to do this if you change from a wireless to a wired connection"));
    syntaxes.append(
            Plasma::RunnerSyntax(
                    "vpn reconnect de",
                    "Reconnect to specific country/server. "
                    "This only supports formats like us3335 or us 3335 and not the names"
                    "Additionally the plugin checks if the target matches the current connection and displays the \"Reconnect To Current option\","
                    " otherwise it displays e.g.\"Reconnect To US 3335 option\""
            ));

    setSyntaxes(syntaxes);
    //endregion
}

void NordVPN::prepareForMatchSession() {

    QString statusData = Status::getRawConnectionStatus(statusSource);
    if (!statusData.isEmpty()) {
        vpnStatus = Status::objectFromRawData(statusData);
    } else if (vpnStatus.rawData.keys().isEmpty()) {
        vpnStatus = Status::objectFromRawData("Status: Disconnected");
    }
}

void NordVPN::matchSessionFinished() {
    if (!wasActive) return;
    if (config.readEntry("clean_history", "true") == "true") {
        wasActive = false;
        QString history = config.parent().parent().group("General").readEntry("history");
        const int historySize = history.size();
        QString filteredHistory = history.replace(QRegExp(R"((?:nord)?vpn d(?:isconnect)?[^ek],?)"), "");
        if (filteredHistory.size() == historySize) return;

        QFile f(QString(getenv("HOME")) + "/.config/krunnerrc");
        if (f.open(QIODevice::ReadWrite)) {
            QString s;
            QTextStream t(&f);
            while (!t.atEnd()) {
                QString line = t.readLine();
                if (!line.startsWith("history")) {
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

    QList<Plasma::QueryMatch> matches;
    if (!term.startsWith("vpn") && !term.startsWith("nordvpn")) return;
    if (vpnStatus.status == "Error") return;

    QList<Match> matchList = Match::generateOptions(vpnStatus, term);
    for (const auto &m:matchList) {
        Plasma::QueryMatch match(this);
        match.setText(m.text);
        match.setData(m.data);
        match.setRelevance(m.relevance);
        match.setIconName(config.readEntry("icon", "/usr/share/icons/nordvpn.png"));
        matches.append(match);
    }
    context.addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)

    QString payload = match.data().toString();
    const QString iconPath = config.readEntry("icon", "/usr/share/icons/nordvpn.png");
    const QString changeScript = config.readEntry("script", "");
    QString cmd;

    bool notify = config.readEntry("notify", "true") == "true";
    QString notifyPipes(R"( | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon <ICON> )");
    if (!notify) notifyPipes = " 2>&1 > /dev/null";
    if (payload == "disconnect") {
        wasActive = true;
        cmd = R"($( nordvpn d PIPES; <SCRIPT>) )";
        cmd.replace("PIPES", notifyPipes);
    } else if (payload == "status") {
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
