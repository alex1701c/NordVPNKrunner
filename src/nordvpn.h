#ifndef NORDVPN_H
#define NORDVPN_H

#include <KRunner/AbstractRunner>
#include "Status.h"

class NordVPN : public Plasma::AbstractRunner {
Q_OBJECT

public:
    NordVPN(QObject *parent, const QVariantList &args);

    ~NordVPN() override;

    void createRunOptions(QWidget *widget) override;

    void reloadConfiguration() override;

    KConfigGroup vpnConfigGroup;

    QString ICON_PATH;

    Status vpnStatus;

    QString statusSource;
    QString changeScript;
    QString defaultTarget;
    bool wasActive=false;

protected Q_SLOTS:

    void init() override;

    void prepareForMatchSession();

    void matchSessionFinished();


public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;

    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
};

#endif
