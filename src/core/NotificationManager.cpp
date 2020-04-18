#include "NotificationManager.h"
#include "Utilities.h"

#include <KNotifications/KNotification>
#include <KConfigGroup>
#include <QDebug>
#include <KSharedConfig>

void NotificationManager::displaySimpleNotification(const QString &status, const QString &icon, const QString &title) {
    KNotification::event(KNotification::Notification, title, status, icon);
}

void NotificationManager::displayConnectNotification(const QString &processOutput) {
    auto resList = processOutput.split('\n');
    for (auto &filteredRes: resList) {
        if (filteredRes.startsWith(QLatin1String("Connecting to"))) {
            continue;
        }
        filteredRes = Utilities::filterBeginning(filteredRes);
        if (!filteredRes.isEmpty()) {
            NotificationManager::displaySimpleNotification(filteredRes);
            break;
        }
    }
}

void NotificationManager::displayDisconnectNotification(const QString &processOutput) {
    auto resList = processOutput.split('\n');
    if (!resList.isEmpty()) {
        NotificationManager::displaySimpleNotification(resList.first());
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
    displaySimpleNotification(notifyText);
}

