#ifndef NORDVPN_PROCESSMANAGER_H
#define NORDVPN_PROCESSMANAGER_H

#include "NotificationManager.h"

/**
 * This class handles the execution of nordvpn processes and interacts
 * with the NotificationManager to handle the notifications
 */
class ProcessManager : public QObject {
public:
    static void connectVPN(bool notify, const QString &target);

    static void disconnectVPN(bool notify);

    static void vpnStatus();

protected:

    /**
     * Gets called by public functions, makes logic reusable
     */
    static void abstractVPNCommand(const QString &program, const QStringList &args,
                                   bool notify, NotificationManager::displayFunction *notificationFunction);
};


#endif //NORDVPN_PROCESSMANAGER_H
