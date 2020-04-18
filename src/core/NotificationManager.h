#ifndef NORDVPN_NOTIFICATIONMANAGER_H
#define NORDVPN_NOTIFICATIONMANAGER_H


#include "Status.h"

/**
 * This class handles the display of the notifications and contains some utilities for  them
 */
class NotificationManager {

public:
    static QString createNotificationText(const QString &processOutput, const QStringList &keyList);

    static void displayStatusNotification(const QString &status, const QString &icon = QStringLiteral("nordvpn"),
            const QString &title = QStringLiteral("NordVPN Krunner"));
};


#endif //NORDVPN_NOTIFICATIONMANAGER_H
