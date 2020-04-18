#include "NotificationManager.h"
#include "Utilities.h"

#include <KNotifications/KNotification>

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

void NotificationManager::displayStatusNotification(const QString &status, const QString &icon, const QString &title) {
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
            NotificationManager::displayStatusNotification(filteredRes);
            break;
        }
    }
}

void NotificationManager::displayDisconnectNotification(QString processOutput) {
    auto resList = Utilities::filterBeginning(processOutput).split('\n');
    if (!resList.isEmpty()) {
        NotificationManager::displayStatusNotification(resList.first());
    }
}

