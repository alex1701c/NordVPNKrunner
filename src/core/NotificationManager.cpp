#include "NotificationManager.h"
#include "Utilities.h"

#include <KNotifications/KNotification>
#include <KConfigGroup>
#include <QDebug>
#include <KSharedConfig>

void NotificationManager::displaySimpleNotification(const QString &event,
                                                    const QString &status,
                                                    const QString &title) {
    // small NordVPN icon is used, param changes nothing
    KNotification::event(event, title, status, QString(), nullptr,
                         KNotification::CloseOnTimeout, "krunner_nordvpn");
}

void NotificationManager::displayConnectNotification(const QString &processOutput) {
    auto resList = processOutput.split('\n');
    for (auto &filteredRes: resList) {
        if (filteredRes.startsWith(QLatin1String("Connecting to"))) {
            continue;
        }
        filteredRes = Utilities::filterBeginning(filteredRes);
        if (!filteredRes.isEmpty()) {
            static const QString eventID = QStringLiteral("connect");
            NotificationManager::displaySimpleNotification(eventID, filteredRes);
            break;
        }
    }
}

void NotificationManager::displayDisconnectNotification(const QString &processOutput) {
    auto resList = processOutput.split('\n');
    if (!resList.isEmpty()) {
        static const QString eventID = QStringLiteral("disconnect");
        NotificationManager::displaySimpleNotification(eventID, resList.first());
    }
}

void NotificationManager::displayStatusNotification(const QString &processOutput) {
    KConfigGroup config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("NordVPN");
    config.config()->reparseConfiguration();
    QStringList keys = config.readEntry("status_filter_keys", QStringList());
    if (keys.isEmpty()) {
        keys = QStringList({"Status", "Current server", "Transfer", "Your new IP"});
    }
    displayStatusNotification(processOutput, keys);
}

void NotificationManager::displayStatusNotification(const QString &processOutput, const QStringList &keys) {
    QString notifyText;
    const auto resList = processOutput.split('\n');
    for (const auto &line: resList) {
        if (line.contains(':')) {
            const auto key = line.split(':').first();
            if (keys.contains(key)) {
                notifyText.append(line + '\n');
            }
        }
    }
    static const QString eventID = QStringLiteral("status");
    displaySimpleNotification(eventID, notifyText);
}

