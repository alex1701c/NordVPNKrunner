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

Status vpnStatus;
QString ICON_PATH = "/home/alex/Downloads/ico/nordvpn_favicon57x57.png"; //TODO put icon in resources and parameterize


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
    // TODO Some sort of caching if the nordvpn cli is very slow
    QProcess process;
    process.start("nordvpn status");
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
    std::cout << "INFORMATION" << std::endl;
    std::cout << vpnStatus.status.toStdString() << std::endl;
    std::cout << vpnStatus.current_server.toStdString() << std::endl;
    std::cout << vpnStatus.country.toStdString() << std::endl;
    std::cout << vpnStatus.server.toStdString() << std::endl;
    std::cout << "END" << std::endl;
}


void NordVPN::init() {
    connect(this, SIGNAL(prepare()), this, SLOT(prepareForMatchSession()));
}

void NordVPN::match(Plasma::RunnerContext &context) {
    QString term = context.query();
    if (term.length() < 3 || (!term.startsWith("vpn") && !term.startsWith("nordvpn"))) {
        return;
    }
    QList<Plasma::QueryMatch> matches;
    if (vpnStatus.status == "Error") {
        return;
    }
    Plasma::QueryMatch statusMatch(this);           // Status
    statusMatch.setIconName(ICON_PATH);
    statusMatch.setText(vpnStatus.status);
    statusMatch.setData("status");
    statusMatch.setRelevance(0.5);
    matches.append(statusMatch);

    if (vpnStatus.connectionExists()) {             //Disconnect
        Plasma::QueryMatch disconnectMatch(this);
        disconnectMatch.setIconName(ICON_PATH);
        disconnectMatch.setText(i18n("Disconnect"));
        disconnectMatch.setData("disconnect");
        disconnectMatch.setRelevance(0);// TODO increase relevance if term is e.g. vpn d or vpn disconnect
        matches.append(disconnectMatch);
    } else {                                        // Connect
        Plasma::QueryMatch connectMatch(this);
        QString target = vpnStatus.evalConnectQuery(term);
        connectMatch.setIconName(ICON_PATH);
        connectMatch.setText(QString::fromStdString("Connect To ") + target);
        connectMatch.setData(QString::fromStdString("nordvpn connect " + target.toStdString()));
        connectMatch.setRelevance(1);
        matches.append(connectMatch);
    }

    if (term.startsWith("vpn reconnect") || term.startsWith("nordvpn reconnect")) {
        // TODO Connect new if disconnected
        // TODO Reconnect if no specific value/value == current server
        // TODO Reconnect to other country/server
    }

    context.
            addMatches(matches);
}

void NordVPN::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
    Q_UNUSED(context)
    if (match.data().toString() == "disconnect") {
        system(R"(nordvpn d | tr -d '/\-|\\' | xargs -d '\n' notify-send --icon /home/alex/Downloads/ico/nordvpn_favicon57x57.png 2>&1 &)");
    } else if (match.data().toString() == "status") {
        system("$(vpnStatus=$(nordvpn status 2>&1 | grep -E 'Status|Current server|Transfer|Your new IP');"
               "notify-send  \"$vpnStatus\"  --icon /home/alex/Downloads/ico/nordvpn_favicon57x57.png;)  2>&1 &");
    } else if (match.data().toString().startsWith("nordvpn connect")) {
        QString cmd = match.data().toString();
        cmd += QString(
                R"( | tr -d '/\-|\\' | tail -2 | cut -d '(' -f 1  |xargs -d '\n' notify-send --icon /home/alex/Downloads/ico/nordvpn_favicon57x57.png 2>&1 &)");
        system(cmd.toLatin1().constData());
    }
}

K_EXPORT_PLASMA_RUNNER(nordvpn, NordVPN)

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER
#include "nordvpn.moc"
