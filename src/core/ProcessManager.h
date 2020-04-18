#ifndef NORDVPN_PROCESSMANAGER_H
#define NORDVPN_PROCESSMANAGER_H

#include "NotificationManager.h"

/**
 * This class handles the execution of nordvpn processes and interacts
 * with the NotificationManager to handle the notifications
 */
class ProcessManager : public QObject {
public:

    typedef void (ProcessCallback)(const QString&);

    static void connectVPN(bool notify, const QString &target);
    static void disconnectVPN(bool notify);

    static void vpnStatus();

protected:

    /**
     * Gets called by public functions, makes logic reusable
     */
    static void runNordVPNCommand(const QStringList &args, bool notify, ProcessCallback *notificationFunction);
};


#endif //NORDVPN_PROCESSMANAGER_H
