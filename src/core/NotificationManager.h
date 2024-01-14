#pragma once

#include <QString>

/**
 * This class handles the display of the notifications
 */
class NotificationManager {
public:
    static void displaySimpleNotification(const QString &event, const QString &status, const QString &title = QString());

    static void displayConnectNotification(const QString &processOutput);
    static void displayDisconnectNotification(const QString &processOutput);
    static void displayStatusNotification(const QString &processOutput);
    static void displayStatusNotification(const QString &processOutput, const QStringList &keys, bool ip);
};


