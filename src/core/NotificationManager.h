#ifndef NORDVPN_NOTIFICATIONMANAGER_H
#define NORDVPN_NOTIFICATIONMANAGER_H


#include "Status.h"

/**
 * This class handles the display of the notifications
 */
class NotificationManager {

public:
    static void displaySimpleNotification(const QString &status, const QString &icon = "nordvpn",
                                          const QString &title = "NordVPN Krunner");

    static void displayConnectNotification(const QString &processOutput);
    static void displayDisconnectNotification(const QString &processOutput);
    static void displayStatusNotification(const QString &processOutput);
    static void displayStatusNotification(const QString &processOutput, const QStringList &keys);
};


#endif //NORDVPN_NOTIFICATIONMANAGER_H
