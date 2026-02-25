set(TS_FILES translations/SWUpdateNoticed_zh_CN.ts)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

set(SOURCES
    main.cpp
    swupdate.cpp
    qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SWUpdateNoticed MANUAL_FINALIZATION ${SOURCES})
else()
    add_executable(SWUpdateNoticed ${SOURCES})
endif()

set(SWUPDATENOTICED_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}")

set_property(
     TARGET SWUpdateNoticed APPEND
     PROPERTY COMPILE_DEFINITIONS
     SWUPDATENOTICED_DATA_DIR="${SWUPDATENOTICED_DATA_DIR}"
)

target_compile_definitions(SWUpdateNoticed
  PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(SWUpdateNoticed
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    ${SWUPDATE_LIBRARY}
)

install(TARGETS
    SWUpdateNoticed
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)

install(FILES
    ${QM_FILES}
    DESTINATION "${SWUPDATENOTICED_DATA_DIR}"
    COMPONENT Runtime
)
