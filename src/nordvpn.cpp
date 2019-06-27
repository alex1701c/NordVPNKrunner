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

void NordVPN::reloadConfiguration() {
    vpnConfigGroup = Config::getConfigGroup();

    statusSource = vpnConfigGroup.readEntry("source", "nordvpn status");
    ICON_PATH = vpnConfigGroup.readEntry("icon", "/usr/share/icons/nordvpn.png");
    changeScript = vpnConfigGroup.readEntry("script", "");
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
    vpnStatus = Status::objectFromRawData(statusData);
}

void NordVPN::matchSessionFinished() {
    if (!wasActive) return;
    if (vpnConfigGroup.readEntry("clean_history", "true") == "true") {
        wasActive = false;
        QString history = vpnConfigGroup.parent().parent().group("General").readEntry("history");
        QString filteredHistory = history
                .replace(QRegExp(R"((?:nord)?vpn set[^,]*,?)"), "")
                .replace(QRegExp(R"((?:nord)?vpn d(?:isconnect)?[^ek],?)"), "");
        if (filteredHistory.size() == vpnConfigGroup.parent().parent().group("General").readEntry("history").size()) {
            return;
        }
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
            f.write(s.toLocal8Bit());
            f.close();
        }
    }
}

void NordVPN::init() {
    reloadConfiguration();
    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
    connect(this, SIGNAL(teardown()), this, SLOT(matchSessionFinished()));
}

void NordVPN::match(Plasma::RunnerContext &context) {
    QString term = context.query();
    if (!context.isValid()) return;
    QList<Plasma::QueryMatch> matches;
    if (term.length() < 3 || (!term.startsWith("vpn") && !term.startsWith("nordvpn"))) return;
    if (vpnStatus.status == "Error") return;


    Match::generateOptions(this, matches, vpnConfigGroup, vpnStatus, term);
    context.addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    wasActive = true;
    QString payload = match.data().toString();
    QString cmd = "";
#ifdef RUNNER_SETTINGS
    if (payload.startsWith("settings|")) {
        payload = payload.section('|', 1);
        Config::configureOptions(vpnConfigGroup, payload);
        reloadConfiguration();
        return;
    }
#endif

    bool notify = vpnConfigGroup.readEntry("notify", "true") == "true";
    QString startFilter(R"( | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon <ICON> )");
    if (!notify) startFilter = " 2>&1 > /dev/null";
    if (payload == "disconnect") {
        if (notify) {
            cmd = R"($( nordvpn d | tr -d '/\-|\\' | xargs -d '\n' notify-send --icon <ICON>; <SCRIPT>) )";
        } else {
            cmd = R"($( nordvpn d; <SCRIPT>) )";
        }
    } else if (payload == "status") {
        cmd = QString("$(vpnStatus=$(nordvpn status 2>&1 | grep -i -E '%1');"
                      "notify-send  \"$vpnStatus\"  --icon <ICON> ) ")
                .arg(vpnConfigGroup.readEntry("status_keys", "Status|Current server|Transfer|IP"));
    } else if (payload.startsWith("nordvpn connect")) {
        cmd = "$( " + payload + startFilter + " ; <SCRIPT>  )";
    } else if (payload.startsWith("nordvpn d")) {
        cmd = "$( " + payload + startFilter + "; <SCRIPT>  )";
    }
    cmd = cmd.replace("<ICON>", ICON_PATH).replace("<SCRIPT>", changeScript);
    system(qPrintable(cmd + " 2>&1 &"));
}

void NordVPN::createRunOptions(QWidget *widget) {
    Q_UNUSED(widget);
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
