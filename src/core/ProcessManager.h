#pragma once

#include <QObject>

/**
 * This class handles the execution of nordvpn processes and interacts
 * with the NotificationManager to handle the notifications
 */
class ProcessManager : public QObject {

public:
    typedef void (ProcessCallback)(const QString &);
    static void connectVPN(bool notify, const QStringList &args);
    static void disconnectVPN(bool notify, const QStringList &args);
    static void reconnectVPN(bool notify, const QStringList &args);
    static void vpnStatus(const QStringList &args);

protected:

    /**
     * Gets called by public functions, makes logic reusable
     */
    static void runNordVPNCommand(const QStringList &args, bool notify, ProcessCallback *notificationFunction);
};

