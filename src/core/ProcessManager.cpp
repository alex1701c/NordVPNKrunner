#include "ProcessManager.h"
#include "NotificationManager.h"
#include "Utilities.h"
#include <QProcess>

void ProcessManager::connectVPN(bool notify, const QStringList &args)
{
    runNordVPNCommand(args, notify, &NotificationManager::displayConnectNotification);
}

void ProcessManager::disconnectVPN(const bool notify, const QStringList &args)
{
    runNordVPNCommand(args, notify, &NotificationManager::displayDisconnectNotification);
}

void ProcessManager::vpnStatus(const QStringList &args)
{
    runNordVPNCommand(args, true, &NotificationManager::displayStatusNotification);
}

void ProcessManager::runNordVPNCommand(const QStringList &args, bool notify, ProcessCallback *notificationFunction)
{
    auto *process = new QProcess;
    process->start("nordvpn", args);
    connect(process, &QProcess::finished, [=]() {
        if (notify) {
            QString _out = QString::fromLocal8Bit(process->readAll());
            const QString out = Utilities::filterBeginning(_out);
            (*notificationFunction)(out);
        }
        process->close();
        process->deleteLater();
    });
}

void ProcessManager::reconnectVPN(bool notify, const QStringList &args)
{
    auto *process = new QProcess;
    process->start("nordvpn", {"d"});
    connect(process, &QProcess::finished, [=]() {
        process->close();
        process->deleteLater();
        connectVPN(notify, args);
    });
}
