add_library(core_api_static STATIC Status.cpp NotificationManager.cpp ProcessManager.cpp Utilities.cpp)
target_link_libraries(core_api_static Qt::Network KF5::ConfigCore KF5::Notifications KF5::CoreAddons)

install(FILES krunner_nordvpn.notifyrc DESTINATION ${KNOTIFYRC_INSTALL_DIR})
