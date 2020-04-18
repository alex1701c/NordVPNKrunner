#include "NotificationManager.h"
#include "Utilities.h"

#include <KNotifications/KNotification>
#include <KConfigGroup>
#include <QDebug>
#include <KSharedConfig>

// TODO Only used for example, FIXME
QString NotificationManager::createNotificationText(const QString &processOutput, const QStringList &keyList) {
    QString res;
    for (const QString &line: processOutput.split('\n')) {
        const QStringList keyValueSplit = line.split(':');
        if (!keyValueSplit.isEmpty()) {
            if (keyList.contains(keyValueSplit.first())) {
                res.append(line + '\n');
            }
        }
    }
    return res;
}

void NotificationManager::displaySimpleNotification(const QString &status, const QString &icon, const QString &title) {
    KNotification::event(KNotification::Notification, title, status, icon);
}

void NotificationManager::displayConnectNotification(QString processOutput) {
    auto resList = Utilities::filterBeginning(processOutput).split('\n');
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

void NotificationManager::displayDisconnectNotification(QString processOutput) {
    auto resList = Utilities::filterBeginning(processOutput).split('\n');
    if (!resList.isEmpty()) {
        NotificationManager::displaySimpleNotification(resList.first());
    }
}

void NotificationManager::displayStatusNotification(QString processOutput) {
    // TODO Reuse config group
    KConfigGroup config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("NordVPN");
    config.config()->reparseConfiguration();
    QStringList keys = config.readEntry("status_filter_keys", QStringList());
    if (keys.isEmpty()) {
        keys = QStringList({"Status", "Current server", "Transfer", "Your new IP"});
    }

    QString notifyText;
    const auto resList = Utilities::filterBeginning(processOutput).split('\n');
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

