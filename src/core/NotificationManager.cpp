#include "NotificationManager.h"
#include "Utilities.h"

#include <KConfigGroup>
#include <KNotification>
#include <KSharedConfig>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

void NotificationManager::displaySimpleNotification(const QString &event, const QString &status, const QString &title)
{
    // NordVPN icon is used, param changes nothing
#if QT_VERSION_MAJOR == 6
    KNotification::event(event, title, status, QPixmap(), KNotification::CloseOnTimeout, QStringLiteral("krunner_nordvpn"));
#else
    KNotification::event(event, title, status, nullptr, KNotification::CloseOnTimeout, QStringLiteral("krunner_nordvpn"));
#endif
}

void NotificationManager::displayConnectNotification(const QString &processOutput)
{
    auto resList = processOutput.split('\n');
    for (auto &filteredRes : resList) {
        if (filteredRes.startsWith(QLatin1String("Connecting to"))) {
            continue;
        }
        filteredRes = Utilities::filterBeginning(filteredRes);
        if (!filteredRes.isEmpty()) {
            static const QString eventID = QStringLiteral("connect");
            NotificationManager::displaySimpleNotification(eventID, filteredRes);
            break;
        }
    }
}

void NotificationManager::displayDisconnectNotification(const QString &processOutput)
{
    auto resList = processOutput.split('\n');
    if (!resList.isEmpty()) {
        NotificationManager::displaySimpleNotification(QStringLiteral("disconnect"), resList.first());
    }
}

void NotificationManager::displayStatusNotification(const KConfigGroup &grp, const QString &processOutput)
{
    // Because the static context is used the keys are fetched for every notification
    const static QStringList defaultKeys = {"Status", "Current server", "Transfer", "Your new IP"};
    QStringList keys = grp.readEntry("status_filter_keys", defaultKeys);
    displayStatusNotification(processOutput, keys, grp.readEntry("ip", false));
}

void NotificationManager::displayStatusNotification(const QString &processOutput, const QStringList &keys, bool ip)
{
    QString notifyText;
    const auto resList = processOutput.split('\n');
    for (const auto &line : resList) {
        if (line.contains(':')) {
            const auto key = line.split(':').first();
            if (keys.contains(key)) {
                notifyText.append(line + '\n');
            }
        }
    }
    static const QString eventID = QStringLiteral("status");
    if (!ip) {
        displaySimpleNotification(eventID, notifyText);
    } else {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(QUrl(QStringLiteral("https://ifconfig.me/ip")));
        auto reply = manager->get(request);

        // Make sure to cancel request after two seconds, then delete pointers
        QTimer::singleShot(2000, [=]() {
            if (!reply->isFinished()) {
                reply->abort();
            }
            // Deletes also reply
            delete manager;
        });
        // Display notification when network request is finished
        QObject::connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
            const QString ipLine = QLatin1String("Fetched Ip Address: %1")
                                       .arg(reply->error() == QNetworkReply::NoError ? QString::fromLocal8Bit(reply->readAll()) : reply->errorString());
            displaySimpleNotification(eventID, notifyText + ipLine);
        });
    }
}
