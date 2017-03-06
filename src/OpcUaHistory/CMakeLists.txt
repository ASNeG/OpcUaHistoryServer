# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# prebuild
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaHistory/Build/BuildConfig.h.in"
    "${PROJECT_BINARY_DIR}/BuildConfig.h"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/etc/init/OpcUaHistory.conf.in"
    "${PROJECT_BINARY_DIR}/OpcUaHistory.conf"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/etc/init.d/OpcUaHistory.in"
    "${PROJECT_BINARY_DIR}/OpcUaHistory"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/usr/lib/systemd/system/OpcUaHistory.service.in"
    "${PROJECT_BINARY_DIR}/OpcUaHistory.service"
)


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# build 
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
include_directories(
    ${OPCUASTACK_INSTALL_PREFIX}/usr/include/OpcUaStack${OPCUASTACK_VERSION_MAJOR}
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}
    ${Boost_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

file(
    GLOB 
    OpcUaHistory_SRC 
    ${PROJECT_SOURCE_DIR}/OpcUaHistory/*/*.cpp
)

add_library(
    OpcUaHistory SHARED 
    ${OpcUaHistory_SRC}
)

target_link_libraries(
    OpcUaHistory
    ${pthread}
    ${OPCUASTACK_LIBRARY_CORE}
    ${OPCUASTACK_LIBRARY_CLIENT}
    ${Boost_LIBRARIES}
)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# install
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
install(TARGETS OpcUaHistory DESTINATION /usr/lib)

install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/OpcUaServer.xml DESTINATION /etc/OpcUaStack/OpcUaHistory) 
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/Opc.Ua.NodeSet.xml DESTINATION /etc/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/Opc.Ua.History.NodeSet.xml DESTINATION /etc/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/HistoryModel.xml DESTINATION /etc/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/HistoryStoreModel.xml DESTINATION /etc/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/OpcUaClientModel.xml DESTINATION /etc/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/Opc.Ua.NodeSet.xml DESTINATION /etc/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/log/README DESTINATION /var/log/OpcUaStack/OpcUaHistory)
install(DIRECTORY DESTINATION /var/log/OpcUaStack/OpcUaHistory)

if (WIN32)
else() 
    install(PROGRAMS ${PROJECT_BINARY_DIR}/OpcUaHistory DESTINATION /etc/init.d)
    install(FILES ${PROJECT_BINARY_DIR}/OpcUaHistory.conf DESTINATION /etc/init)
    install(FILES ${PROJECT_BINARY_DIR}/OpcUaHistory.service DESTINATION /usr/lib/systemd/system)
endif()

if (CPACK_BINARY_DEB)
    set( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaHistory/linux/script/postinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaHistory/linux/script/preinst"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaHistory/linux/script/prerm"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/OpcUaHistory/linux/script/postrm")
endif()

if (CPACK_BINARY_RPM)
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/OpcUaHistory/linux/script/postinst)
    set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/OpcUaHistory/linux/script/prerm)
endif()
