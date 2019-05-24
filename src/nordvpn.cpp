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
#include "Config.h"
#include "Match.h"
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
    setSpeed(NormalSpeed);
    setPriority(HighestPriority);
    setHasRunOptions(true);
    setIgnoredTypes(Plasma::RunnerContext::Directory |
                    Plasma::RunnerContext::File |
                    Plasma::RunnerContext::NetworkLocation);
}

NordVPN::~NordVPN() {
}

// TODO Read default target (currently US) from config ✔
// TODO Add option to set the config values from Krunner
// TODO Add reset to default option
// TODO Add filters for history (to remove disconnect entries from autocompletion)
// TODO Script that runs after the command gets executed  ✔
// TODO Implement dialog to change the config, how?
void NordVPN::reloadConfiguration() {
    std::cout << "Initializing" << std::endl;
    vpnConfigGroup = Config::getConfigGroup();

    statusSource = vpnConfigGroup.readEntry("source", "nordvpn status");
    ICON_PATH = vpnConfigGroup.readEntry("icon", "/home/alex/Downloads/ico/nordvpn_favicon57x57.png");
    changeScript = vpnConfigGroup.readEntry("script", "");
    defaultTarget = vpnConfigGroup.readEntry("default", "US");
    //std::cout << config->group("General").readEntry("history").toStdString() << std::endl;
    /*vpnConfigGroup.writeEntry("msg", "Test 🙂🙃");
    vpnConfigGroup.sync();*/
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
    QProcess process;
    process.start(statusSource);
    //std::cout << statusSource.toStdString() << std::endl;
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

    if (!context.isValid()) return;
    if (term.length() < 3 || (!term.startsWith("vpn") && !term.startsWith("nordvpn"))) return;
    if (vpnStatus.status == "Error") return;

    QList<Plasma::QueryMatch> matches;
    Match::generateOptions(this, matches, vpnConfigGroup, vpnStatus, term);
    context.addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    QString cmd = "";
    QString startFilter(R"( | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon <ICON> )");
    if (match.data().toString() == "disconnect") {
        cmd = R"($( nordvpn d | tr -d '/\-|\\' | xargs -d '\n' notify-send --icon <ICON>; <SCRIPT>) )";
    } else if (match.data().toString() == "status") {
        cmd = "$(vpnStatus=$(nordvpn status 2>&1 | grep -E 'Status|Current server|Transfer|Your new IP');"
              "notify-send  \"$vpnStatus\"  --icon <ICON> )  ";
    } else if (match.data().toString().startsWith("nordvpn connect")) {
        cmd = "$( " + match.data().toString() + startFilter + " ; <SCRIPT>  )";
    } else if (match.data().toString().startsWith("nordvpn d")) {
        cmd = "$( " + match.data().toString() + startFilter + "; <SCRIPT>  )";
    }
    cmd = cmd.replace("<ICON>", ICON_PATH).replace("<SCRIPT>", changeScript);
    //std::cout << cmd.toStdString() << std::endl;
    system(qPrintable(cmd + " 2>&1 &"));
}

void NordVPN::createRunOptions(QWidget *widget) {
    Q_UNUSED(widget);
    /*std::cout << "CONFIGURE" << std::endl;
    QVBoxLayout *layout = new QVBoxLayout(widget);
    QCheckBox *cb = new QCheckBox(widget);
    cb->setText(i18n("This is just for show"));
    layout->addWidget(cb);
    widget->showNormal();*/
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
