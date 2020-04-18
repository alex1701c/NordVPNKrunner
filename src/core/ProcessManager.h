#ifndef NORDVPN_PROCESSMANAGER_H
#define NORDVPN_PROCESSMANAGER_H

/**
 * This class handles the execution of nordvpn processes and interacts
 * with the NotificationManager to handle the notifications
 */
class ProcessManager : public QObject {
public:
    static void disconnectVPN(bool notify);
};


#endif //NORDVPN_PROCESSMANAGER_H
