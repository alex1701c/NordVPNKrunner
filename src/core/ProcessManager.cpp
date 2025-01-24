#include "ProcessManager.h"
#include "NotificationManager.h"
#include "Utilities.h"
#include <QProcess>

template<typename T>
void runNordVPNCommand(const QStringList &args, bool notify, T notificationFunction)
{
    auto *process = new QProcess;
    process->start(QStringLiteral("nordvpn"), args);
    QObject::connect(process, &QProcess::finished, [=]() {
        if (notify) {
            QString _out = QString::fromLocal8Bit(process->readAll());
            const QString out = Utilities::filterBeginning(_out);
            notificationFunction(out);
        }
        process->close();
        process->deleteLater();
    });
}

void ProcessManager::connectVPN(bool notify, const QStringList &args)
{
    runNordVPNCommand(args, notify, &NotificationManager::displayConnectNotification);
}

void ProcessManager::disconnectVPN(const bool notify, const QStringList &args)
{
    runNordVPNCommand(args, notify, &NotificationManager::displayDisconnectNotification);
}

void ProcessManager::vpnStatus(const KConfigGroup &grp, const QStringList &args)
{
    runNordVPNCommand(args, true, [grp](const QString &out) {
        NotificationManager::displayStatusNotification(grp, out);
    });
}

void ProcessManager::reconnectVPN(bool notify, const QStringList &args)
{
    auto *process = new QProcess;
    process->start(QStringLiteral("nordvpn"), {QStringLiteral("d")});
    connect(process, &QProcess::finished, [=]() {
        process->close();
        process->deleteLater();
        connectVPN(notify, args);
    });
}
