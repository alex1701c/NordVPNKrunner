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
#include <iostream>
#include <string>
#include <sstream>
// KF
#include <KLocalizedString>
#include <QtGui/QtGui>

std::string vpnStatus;
QString ICON_PATH = "/home/alex/Downloads/ico/nordvpn_favicon57x57.png";


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


void NordVPN::prepareForMatchSession() {
    QProcess process;
    process.start("nordvpn status");
    process.waitForFinished(-1);
    QByteArray out = process.readAllStandardOutput();
    vpnStatus = out.toStdString();
    std::cout << "Inited" << std::endl;
}


void NordVPN::init() {
    reloadConfiguration();
    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
}

void NordVPN::match(Plasma::RunnerContext &context) {
    const QString term = context.query();
    if (term.length() < 3 || (!term.startsWith("vpn") && !term.startsWith("nordvpn"))) {
        return;
    }
    QList<Plasma::QueryMatch> matches;
    if (vpnStatus == "Error") {
        return;
    }
    std::string statusLine;
    std::stringstream statusStream(vpnStatus);
    while (std::getline(statusStream, statusLine, '\n')) {
        QString l = QString::fromStdString(statusLine);
        if (l.startsWith("Status: Connect") || l == "Status: Reconnecting") {//Connected / connecting/reconnecting
            Plasma::QueryMatch disconnectMatch(this);
            disconnectMatch.setIconName(ICON_PATH);
            disconnectMatch.setText(i18n("Disconnect"));
            disconnectMatch.setData("disconnect");
            disconnectMatch.setRelevance(0);
            matches.append(disconnectMatch);
        } else if (l == "Status: Disconnected") {
            std::string target = "us";
            const std::string msg = "Connect to " + target;
            Plasma::QueryMatch connectMatch(this);
            connectMatch.setIconName(ICON_PATH);
            connectMatch.setText(QString::fromStdString("Connect to ") + QString::fromStdString(target).toUpper());
            connectMatch.setData(QString::fromStdString("connect;" + target));
            connectMatch.setRelevance(1);
            matches.append(connectMatch);
        }
        if (l.startsWith("Status:")) {
            Plasma::QueryMatch statusMatch(this);
            statusMatch.setIconName(ICON_PATH);
            statusMatch.setText(l);
            statusMatch.setData("status");
            statusMatch.setRelevance(0.5);
            matches.append(statusMatch);
        }

    }
    context.addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    if (match.data().toString() == "disconnect") {
        system(R"(nordvpn d | tr -d '/\-|\\' | xargs -d '\n' notify-send --icon /home/alex/Downloads/ico/nordvpn_favicon57x57.png 2>&1 &)");
    } else if (match.data().toString() == "status") {
        system("status=$(nordvpn status 2>&1 | grep -E 'Status|Current server|Transfer|Your new IP');"
               "notify-send  \"$status\"  --icon /home/alex/Downloads/ico/nordvpn_favicon57x57.png  2>&1");
    } else if (match.data().toString().startsWith("connect;")) {
        QString cmd(
                R"(nordvpn c <target> | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon /home/alex/Downloads/ico/nordvpn_favicon57x57.png  2>&1 &)");
        cmd.replace("<target>", match.data().toString().split(';')[1]);
        QString exec = "/bin/sh -c \" " + cmd + " \"";
        std::cout << exec.toStdString() << std::endl;
        QProcess::startDetached(exec);
    }
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
