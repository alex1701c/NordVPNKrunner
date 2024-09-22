add_library(core_api_static STATIC Status.cpp NotificationManager.cpp ProcessManager.cpp Utilities.cpp)
set_property(TARGET core_api_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(core_api_static Qt::Network KF${QT_MAJOR_VERSION}::ConfigCore KF${QT_MAJOR_VERSION}::Notifications KF${QT_MAJOR_VERSION}::CoreAddons)

install(FILES krunner_nordvpn.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
