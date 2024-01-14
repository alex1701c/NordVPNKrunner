#include "Utilities.h"

#include <QDir>
#include <QFile>
#include <QRegularExpression>

QString Utilities::filterBeginning(QString &str)
{
    static const QRegularExpression cleanupRegex(R"(^[\r\\\|/\- ]*)");
    // https://github.com/alex1701c/NordVPNKrunner/issues/6
    str.remove(QLatin1String("\r-\r"));
    str.remove(QLatin1Char('\r'));

    str = str.remove("A new version of NordVPN is available! Please update the application.\n");
    static const QRegularExpression newFeatureRegex(QStringLiteral("^New feature -.*\n"));
    str = str.remove(newFeatureRegex).trimmed();
    return str.remove(cleanupRegex);
}

bool Utilities::sameTarget(QString &target, const Status &status)
{
    static const QRegularExpression countryRegex("[a-zA-z ]{2,50}$");
    const bool countryOnly = target.contains(countryRegex);
    const bool emptyTarget = target.remove(' ').isEmpty();

    // Empty target is interpreted as same target
    if (emptyTarget) {
        return true;
    }
    // Same country, no server specified
    if (countryOnly && target.compare(status.country, Qt::CaseInsensitive) == 0) {
        return true;
    }

    // Country and server specified and they match exactly the target
    return target.compare(status.country + status.server, Qt::CaseInsensitive) == 0;
}

QString Utilities::initializeConfigFile()
{
    const QString configFolder = QDir::homePath() + QStringLiteral("/.config/krunnerplugins/");
    const QString configFilePath = configFolder + QStringLiteral("nordvpnrunnerrc");
    const QDir configDir(configFolder);
    if (!configDir.exists()) {
        configDir.mkpath(configFolder);
    }
    // Create file
    QFile configFile(configFilePath);
    if (!configFile.exists()) {
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    return configFilePath;
}
