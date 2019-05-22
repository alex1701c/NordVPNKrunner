add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.nordvpn\")

set(nordvpn_SRCS nordvpn.cpp Status.cpp Status.h )

add_library(krunner_nordvpn MODULE ${nordvpn_SRCS})
target_link_libraries(krunner_nordvpn KF5::Runner KF5::I18n)

install(TARGETS krunner_nordvpn DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES plasma-runner-nordvpn.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
