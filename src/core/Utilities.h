#ifndef NORDVPN_UTILITIES_H
#define NORDVPN_UTILITIES_H

#include "Status.h"
#include <QString>

/**
 * Shome useful utility functions
 */
class Utilities
{
public:
    /**
     * Filters loading characters and useless linebreaks etc. from the beginning
     */
    static QString filterBeginning(QString &str);

    /**
     * If the term and the status have the same target
     */
    static bool sameTarget(QString &target, const Status &status);

    /**
     * Creates config file if it does not exist
     */
    static QString initializeConfigFile();
};

#endif // NORDVPN_UTILITIES_H
