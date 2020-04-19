#include "Utilities.h"

#include <QRegularExpression>
#include <QFile>
#include <QDir>

QString Utilities::filterBeginning(QString &str) {
    static const QRegularExpression cleanupRegex(R"(^[\r\\\|/\- ]*)");
    return str.remove(cleanupRegex);
}

bool Utilities::sameTarget(QString &target, const Status &status) {
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

QString Utilities::initializeConfigFile() {
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
