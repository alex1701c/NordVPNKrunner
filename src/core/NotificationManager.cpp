#include "NotificationManager.h"

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

