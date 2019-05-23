/*
   Copyright %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nordvpn.h"
#include "Status.h"
#include <iostream>
#include <string>
#include <sstream>
// KF
#include <KLocalizedString>
#include <QtGui/QtGui>
#include <KSharedConfig>

NordVPN::NordVPN(QObject *parent, const QVariantList &args)
        : Plasma::AbstractRunner(parent, args) {
    setObjectName("NordVPN");
    setSpeed(SlowSpeed);
    setPriority(HighestPriority);
    setHasRunOptions(true);
    setIgnoredTypes(Plasma::RunnerContext::Directory |
                    Plasma::RunnerContext::File |
                    Plasma::RunnerContext::NetworkLocation);
}

NordVPN::~NordVPN() {
}

void NordVPN::reloadConfiguration() {

    KSharedConfig::Ptr config = KSharedConfig::openConfig("krunnerrc");
    KConfigGroup vpnConfigGroup = config->group("Runners");
    vpnConfigGroup = KConfigGroup(&vpnConfigGroup, "NordVPN");
    statusSource = vpnConfigGroup.readEntry("source", "nordvpn status");
    ICON_PATH = vpnConfigGroup.readEntry("icon", "/home/alex/Downloads/ico/nordvpn_favicon57x57.png");
    /*vpnConfigGroup.writeEntry("msg", "Test 🙂🙃");
    vpnConfigGroup.sync();*/

    QList<Plasma::RunnerSyntax> syntaxes;
    syntaxes.append(Plasma::RunnerSyntax("vpn us", "Connect options to United States, server is chosen by NordVPN"));
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
}

void NordVPN::prepareForMatchSession() {
    QProcess process;
    process.start(statusSource);
    std::cout << statusSource.toStdString() << std::endl;
    //process.start("cat /tmp/ramdisktemp/status");
    // I have the issue that it es sometimes very slow, file gets updated by command output widget
    process.waitForFinished(-1);
    QByteArray out = process.readAllStandardOutput();

    for (const auto &line:out.split('\n')) {
        if (line.startsWith("Status:")) {
            vpnStatus.status = line;
        } else if (line.startsWith("Current server: ")) {
            vpnStatus.current_server = line;
            break;
        }
    }
    vpnStatus.extractConectionInformation();
    /*std::cout << "INFORMATION" << std::endl;
    std::cout << vpnStatus.status.toStdString() << std::endl;
    std::cout << vpnStatus.current_server.toStdString() << std::endl;
    std::cout << vpnStatus.country.toStdString() << std::endl;
    std::cout << vpnStatus.server.toStdString() << std::endl;
    std::cout << "END" << std::endl;*/
}

void NordVPN::matchSessionFinished() {

}

void NordVPN::init() {
    reloadConfiguration();
    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
    connect(this, SIGNAL(teardown()), this, SLOT(matchSessionFinished()));
}

void NordVPN::match(Plasma::RunnerContext &context) {
    QString term = context.query();
    if (term.length() < 3 || (!term.startsWith("vpn") && !term.startsWith("nordvpn"))) {
        return;
    }
    if (vpnStatus.status == "Error") {
        return;
    }
    QList<Plasma::QueryMatch> matches;

    createMatch(matches, vpnStatus.status, QString("status"), 0.5);     // Status


    if (vpnStatus.connectionExists()) {             //Disconnect
        int relevanceDisconnect = 0;
        if (term.contains(QRegExp("vpn d(isconnect)?[ ]*$"))) {
            relevanceDisconnect = 1;
        }
        createMatch(matches, QString("Disconnect"), QString("disconnect"), relevanceDisconnect);
    } else {                                        // Connect
        QString target = Status::evalConnectQuery(term, "US");
        createMatch(matches, QString("Connect To " + target),
                    QString("nordvpn connect " + target), 1);
    }

    if (vpnStatus.connectionExists() && (term.startsWith("vpn reconnect") || term.startsWith("nordvpn reconnect"))) {
        QString target = Status::evalConnectQuery(term);
        bool textOnly = target.contains(QRegExp("[a-zA-z ]{2,50}$"));
        if ((QString((vpnStatus.country + vpnStatus.server)).replace(" ", "").toUpper()
                     .startsWith(target.replace(" ", "").toUpper()) &&
             (textOnly || target.endsWith(vpnStatus.server))) ||
            target.replace(" ", "").isEmpty()) {
            // [The address from the status startswith the one of the query &&
            // (the server addresses match exactly || no server address specified)] || no targeted address
            if (target.isEmpty()) {
                target = vpnStatus.country + vpnStatus.server;
            } else if (textOnly) {
                target += vpnStatus.server;
            }
            createMatch(matches, QString("Reconnect To Current "),
                        QString("nordvpn d > /dev/null 2>&1 ;nordvpn c  " + target), 1);
        } else {
            createMatch(matches, QString("Reconnect To " + target),
                        QString("nordvpn d > /dev/null 2>&1 ;nordvpn c  " + target), 1);
        }
    }

    context.addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    QString cmd = "";
    QString startFilter(R"( | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon <ICON> )");
    if (match.data().toString() == "disconnect") {
        cmd = R"(nordvpn d | tr -d '/\-|\\' | xargs -d '\n' notify-send --icon <ICON> )";
    } else if (match.data().toString() == "status") {
        cmd = "$(vpnStatus=$(nordvpn status 2>&1 | grep -E 'Status|Current server|Transfer|Your new IP');"
              "notify-send  \"$vpnStatus\"  --icon <ICON> )  ";
    } else if (match.data().toString().startsWith("nordvpn connect")) {
        cmd = match.data().toString() + startFilter;
    } else if (match.data().toString().startsWith("nordvpn d")) {
        cmd = "$( " + match.data().toString() + startFilter + " )";
    }
    system(qPrintable(cmd.replace("<ICON>", ICON_PATH) + " 2>&1 &"));
}

void NordVPN::createMatch(QList<Plasma::QueryMatch> &matches,
                          const QString &text, const QString &data, double relevance) {
    Plasma::QueryMatch match(this);
    match.setIconName(ICON_PATH);
    match.setText(text);
    match.setData(data);
    match.setRelevance(relevance);
    matches.append(match);
}

void NordVPN::createRunOptions(QWidget *widget) {
    AbstractRunner::createRunOptions(widget);
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
