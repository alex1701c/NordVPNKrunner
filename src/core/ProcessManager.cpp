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
                    QString res = QString::fromLocal8Bit(process->readAll());
                    auto resList = Utilities::filterBeginning(res).split('\n');
                    for (auto &filteredRes: resList) {
                        if (filteredRes.startsWith(QLatin1String("Connecting to"))) {
                            continue;
                        }
                        filteredRes = Utilities::filterBeginning(filteredRes);
                        if (!filteredRes.isEmpty()) {
                            NotificationManager::displayStatusNotification(filteredRes);
                            break;
                        }
                    }
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
