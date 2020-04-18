#include <QProcess>
#include <QDebug>
#include "ProcessManager.h"
#include "Utilities.h"
#include "NotificationManager.h"

void ProcessManager::connectVPN(bool notify, const QString &target) {
    abstractVPNCommand("nordvpn", {"c", target}, notify, &NotificationManager::displayConnectNotification);
}

void ProcessManager::disconnectVPN(const bool notify) {
    abstractVPNCommand("nordvpn", {"d"}, notify, &NotificationManager::displayDisconnectNotification);
}

void ProcessManager::abstractVPNCommand(const QString &program, const QStringList &args, bool notify,
                                        NotificationManager::displayFunction *notificationFunction) {
    auto *process = new QProcess;
    process->start(program, args);
    connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode)
                Q_UNUSED(exitStatus)
                if (notify) {
                    (*notificationFunction)(QString::fromLocal8Bit(process->readAll()));
                }
                process->close();
                process->deleteLater();
            }
    );
}
