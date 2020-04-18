#include <QProcess>
#include <QDebug>
#include "ProcessManager.h"
#include "Utilities.h"
#include "NotificationManager.h"

void ProcessManager::disconnectVPN(const bool notify) {
    auto *process = new QProcess;
    process->start("nordvpn", {"d"});
    connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
                Q_UNUSED(exitCode);
                Q_UNUSED(exitStatus);
                if (notify) {
                    QString res = QString::fromLocal8Bit(process->readAll());
                    auto resList = Utilities::filterBeginning(res).split('\n');
                    if (!resList.isEmpty()) {
                        NotificationManager::displayStatusNotification(resList.first());
                    }
                }
                process->close();
                process->deleteLater();
            }
    );
}
