set(TS_FILES translations/SWUpdateNoticed_zh_CN.ts)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

set(SOURCES
    main.cpp
    swupdate.cpp
    qml.qrc
    ${QM_FILES}
)

add_executable(SWUpdateNoticed ${SOURCES})

set_property(
     TARGET SWUpdateNoticed APPEND
     PROPERTY COMPILE_DEFINITIONS
     SWUPDATENOTICED_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}"
)

target_compile_definitions(SWUpdateNoticed
  PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(SWUpdateNoticed
  PRIVATE
    Qt5::Gui
    Qt5::Quick
    ${SWUPDATE_LIBRARY}
)

install(TARGETS
    SWUpdateNoticed
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)
