#pragma once

#include "ui_nordvpn_config.h"
#include <KCModule>
#include <KConfigGroup>

class NordVPNConfigForm : public QWidget, public Ui::NordVPNConfigUi
{
    Q_OBJECT

public:
    explicit NordVPNConfigForm(QWidget *parent);
};

class NordVPNConfig : public KCModule
{
    Q_OBJECT

public:
    explicit NordVPNConfig(QWidget *parent = nullptr, const QVariantList &args = QVariantList());

public Q_SLOTS:

    void save() override;
    void load() override;
    void defaults() override;

    void exampleStatus();

    void openIconFileChooser();
    void setDefaultIcon();

private:
    void setCurrentSettings();
    QStringList getStatusNotificationKeys();
    void writeConfigText(const QString &key, const QString &text);

    NordVPNConfigForm *m_ui;
    KConfigGroup config;
    QString newIcon;
    const QString defaultIcon = "nordvpn";
    const QString exampleData = QStringLiteral(
        "Status: Connected\n"
        "Current server: us4276.nordvpn.com\n"
        "Country: United States\n"
        "City: Manassas\n"
        "Your new IP: 23.82.14.23\n"
        "Current protocol: UDP\n"
        "Transfer: 26.9 KiB received, 8.6 KiB sent\n"
        "Uptime: 7 seconds");
};
