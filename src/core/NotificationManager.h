#ifndef NORDVPN_NOTIFICATIONMANAGER_H
#define NORDVPN_NOTIFICATIONMANAGER_H


#include "Status.h"

/**
 * This class handles the display of the notifications and contains some utilities for  them
 */
class NotificationManager {

public:
    typedef void (displayFunction)(QString);
    static QString createNotificationText(const QString &processOutput, const QStringList &keyList);

    static void displaySimpleNotification(const QString &status, const QString &icon = "nordvpn",
                                          const QString &title = "NordVPN Krunner");

    static void displayConnectNotification(QString processOutput);
    static void displayDisconnectNotification(QString processOutput);
};


#endif //NORDVPN_NOTIFICATIONMANAGER_H
