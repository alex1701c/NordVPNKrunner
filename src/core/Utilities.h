#ifndef NORDVPN_UTILITIES_H
#define NORDVPN_UTILITIES_H

#include <QString>

/**
 * Shome useful utility functions
 */
class Utilities {

public:
    /**
     * Filters loading characters and useless linebreaks etc. from the beginning
     */
    static QString filterBeginning(QString &str);
};


#endif //NORDVPN_UTILITIES_H
