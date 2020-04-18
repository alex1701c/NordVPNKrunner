#include "Utilities.h"

#include <QRegularExpression>

QString Utilities::filterBeginning(QString &str) {
    static const QRegularExpression cleanupRegex(R"(^[\r\\\|/\- ]*)");
    return str.remove(cleanupRegex);
}
