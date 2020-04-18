#include <QProcess>
#include <QDebug>
#include "ProcessManager.h"
#include "Utilities.h"
#include "NotificationManager.h"

void ProcessManager::connectVPN(bool notify, const QString &target) {
    runNordVPNCommand({"c", target}, notify, &NotificationManager::displayConnectNotification);
}

void ProcessManager::disconnectVPN(const bool notify) {
    runNordVPNCommand({"d"}, notify, &NotificationManager::displayDisconnectNotification);
}

void ProcessManager::vpnStatus() {
    runNordVPNCommand({"status"}, true, &NotificationManager::displayStatusNotification);
}

void ProcessManager::runNordVPNCommand(const QStringList &args, bool notify, ProcessCallback *notificationFunction) {
    auto *process = new QProcess;
    process->start("nordvpn", args);
    connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode)
                Q_UNUSED(exitStatus)
                if (notify) {
                    QString _out = QString::fromLocal8Bit(process->readAll());
                    const QString out = Utilities::filterBeginning(_out);
                    (*notificationFunction)(out);
                }
                process->close();
                process->deleteLater();
            }
    );
}
