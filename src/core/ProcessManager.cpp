#include <QProcess>
#include <QDebug>
#include "ProcessManager.h"
#include "Utilities.h"
#include "NotificationManager.h"

void ProcessManager::connectVPN(bool notify, const QString &target) {
    auto *process = new QProcess;
    process->start("nordvpn", {"c", target});
    connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode)
                Q_UNUSED(exitStatus)
                if (notify) {
                   NotificationManager::displayConnectNotification(QString::fromLocal8Bit(process->readAll()));
                }
                process->close();
                process->deleteLater();
            }
    );
}

void ProcessManager::disconnectVPN(const bool notify) {
    auto *process = new QProcess;
    process->start("nordvpn", {"d"});
    connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode)
                Q_UNUSED(exitStatus)
                if (notify) {
                    NotificationManager::displayDisconnectNotification(QString::fromLocal8Bit(process->readAll()));
                }
                process->close();
                process->deleteLater();
            }
    );
}
