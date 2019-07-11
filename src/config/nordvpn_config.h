#ifndef NORDVPNCONFIG_H
#define NORDVPNCONFIG_H

#include "ui_nordvpn_config.h"
#include <KCModule>
#include <KConfigCore/KConfigGroup>

class NordVPNConfigForm : public QWidget, public Ui::NordVPNConfigUi {
Q_OBJECT

public:
    explicit NordVPNConfigForm(QWidget *parent);
};

class NordVPNConfig : public KCModule {
Q_OBJECT

public:
    explicit NordVPNConfig(QWidget *parent = nullptr, const QVariantList &args = QVariantList());

    KConfigGroup config;

    QString newIcon;

    const QString defaultIcon = "/usr/share/icons/nordvpn.png";

public Q_SLOTS:

    void save() override;

    void load() override;

    void defaults() override;

    void showExampleStatusNotification();

    void openIconFileChooser();

    void setDefaultIcon();

    void exampleStatus();

private:
    NordVPNConfigForm *m_ui;

    void setCurrentSettings();

    QString getStatusNotificationKeys();

    void writeConfigText(QString key, QString text);
};

#endif
