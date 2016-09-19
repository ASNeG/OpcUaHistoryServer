
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# build 
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
include_directories(
    ${INSTALL_PREFIX_OpcUaStack}/usr/include/OpcUaStack
    ${PROJECT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)

file(GLOB OpcUaHistory_SRC ${PROJECT_SOURCE_DIR}/OpcUaHistory/*/*.cpp)
add_library(OpcUaHistory SHARED ${OpcUaHistory_SRC})

target_link_libraries(
    OpcUaHistory
    ${OpcUaStackCore}
    ${OpcUaStackServer}
    ${OpcUaStackClient}
    ${pthread}
    ${Boost_LIBRARIES}
)

set_target_properties(
    OpcUaHistory
    PROPERTIES 
    VERSION ${VERSION}
    SOVERSION ${VERSION_MAJOR}
)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# install
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
install(TARGETS OpcUaHistory DESTINATION ${LIB_DIR}/OpcUaStack)

install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/OpcUaServer.xml DESTINATION ${CONF_DIR}/OpcUaStack/OpcUaHistory) 
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/Opc.Ua.NodeSet.xml DESTINATION ${CONF_DIR}/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/Config/OpcUaHistoryModel.xml DESTINATION ${CONF_DIR}/OpcUaStack/OpcUaHistory)
install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/log/README DESTINATION ${LOG_DIR}/OpcUaStack/OpcUaHistory)
install(DIRECTORY DESTINATION ${LOG_DIR}/OpcUaStack/OpcUaHistory)

if (WIN32)
else() 
    install(PROGRAMS ${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/etc/init.d/OpcUaHistory DESTINATION etc/init.d)
    install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/etc/init/OpcUaHistory.conf DESTINATION etc/init)
    install(FILES ${PROJECT_SOURCE_DIR}/OpcUaHistory/linux/usr/lib/systemd/system/OpcUaHistory.service DESTINATION usr/lib/systemd/system)
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
