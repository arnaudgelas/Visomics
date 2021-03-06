
# -------------------------------------------------------------------------
# Find and install QT
# -------------------------------------------------------------------------

SET(QT_INSTALL_LIB_DIR ${Visomics_INSTALL_LIB_DIR})

SET(QTLIBLIST QTCORE QTGUI QTNETWORK QTXML QTTEST QTSQL QTOPENGL QTWEBKIT PHONON QTXMLPATTERNS QTSCRIPT)
IF(UNIX OR APPLE)
  LIST(APPEND QTLIBLIST QTDBUS)
ENDIF()
FOREACH(qtlib ${QTLIBLIST})
  IF (QT_${qtlib}_LIBRARY_RELEASE)
    IF(APPLE)
      INSTALL(DIRECTORY "${QT_${qtlib}_LIBRARY_RELEASE}"
        DESTINATION ${QT_INSTALL_LIB_DIR} COMPONENT Runtime)
    ELSEIF(UNIX)
      # Install .so and versioned .so.x.y
      GET_FILENAME_COMPONENT(QT_LIB_DIR_tmp ${QT_${qtlib}_LIBRARY_RELEASE} PATH)
      GET_FILENAME_COMPONENT(QT_LIB_NAME_tmp ${QT_${qtlib}_LIBRARY_RELEASE} NAME)
      INSTALL(DIRECTORY ${QT_LIB_DIR_tmp}/
        DESTINATION ${QT_INSTALL_LIB_DIR} COMPONENT Runtime
        FILES_MATCHING PATTERN "${QT_LIB_NAME_tmp}*"
        PATTERN "${QT_LIB_NAME_tmp}*.debug" EXCLUDE)
    ELSEIF(WIN32)
      INSTALL(FILES ${QT_BINARY_DIR}/${qtlib}4.dll
        DESTINATION ${QT_INSTALL_LIB_DIR} COMPONENT Runtime)
    ENDIF()
  ENDIF()
ENDFOREACH()

# -------------------------------------------------------------------------
# Install R
# -------------------------------------------------------------------------

#IF(R_LIBRARY_DIR)
#  #message(STATUS "R_LIBRARY_DIR:${R_LIBRARY_DIR}")
#  IF(APPLE OR UNIX)
#    SET(files)
#    FIND_LIBRARY(R_LIBRARY_BASE R HINTS ${R_LIBRARY_DIR} NO_DEFAULT_PATH)
#    IF(R_LIBRARY_BASE)
#      #message(STATUS "R_LIBRARY_BASE:${R_LIBRARY_BASE}")
#      LIST(APPEND files ${R_LIBRARY_BASE})
#    ENDIF()
#    FIND_LIBRARY(R_LIBRARY_BLAS Rblas HINTS ${R_LIBRARY_DIR} NO_DEFAULT_PATH)
#    IF(R_LIBRARY_BLAS)
#      #message(STATUS "R_LIBRARY_BLAS:${R_LIBRARY_BLAS}")
#      LIST(APPEND files ${R_LIBRARY_BLAS})
#    ENDIF()
#    FIND_LIBRARY(R_LIBRARY_LAPACK Rlapack HINTS ${R_LIBRARY_DIR} NO_DEFAULT_PATH)
#    IF(R_LIBRARY_LAPACK)
#      #message(STATUS "R_LIBRARY_LAPACK:${R_LIBRARY_LAPACK}")
#      LIST(APPEND files ${R_LIBRARY_LAPACK})
#    ENDIF()
#    INSTALL(FILES ${files}
#	    DESTINATION ${Visomics_INSTALL_LIB_DIR} COMPONENT Runtime)
#  ELSEIF(WIN32)
#    #TODO
#  ENDIF()
#ENDIF()

# -------------------------------------------------------------------------
# Build a CPack installer 
# -------------------------------------------------------------------------

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Visomics - Application allowing to import, analyze and visualize metabolomics and genomics data.")
SET(CPACK_PACKAGE_VENDOR "Kitware, Inc.")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/NOTICE")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/NOTICE")
SET(CPACK_PACKAGE_VERSION_MAJOR "${Visomics_MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${Visomics_MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${Visomics_BUILD_VERSION}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Visomics ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "Visomics-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_PACKAGE_EXECUTABLES "Visomics;Visomics")
SET(CPACK_CREATE_DESKTOP_LINKS "visomics ${Visomics_VERSION}")

IF(Q_WS_WIN)
  INSTALL(FILES ${Visomics_SOURCE_DIR}/Base/Resources/Icons/visomics.ico
    DESTINATION ${Visomics_INSTALL_BIN_DIR} COMPONENT Runtime)
ENDIF()

CONFIGURE_FILE(
  ${PROJECT_SOURCE_DIR}/CMake/VisomicsCPackOptions.cmake.in
  ${PROJECT_BINARY_DIR}/VisomicsCPackOptions.cmake @ONLY)

SET(CPACK_PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/VisomicsCPackOptions.cmake")

# -------------------------------------------------------------------------
# Install Visomics
# -------------------------------------------------------------------------
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};Visomics;Runtime;/")


# -------------------------------------------------------------------------
# Install VTK
# -------------------------------------------------------------------------
if(EXISTS "${VTK_DIR}/CMakeCache.txt")
  # Note: To work as expected, VTK has to be configured with -DVTK_INSTALL_LIB_DIR:PATH=lib
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${VTK_DIR};VTK;RuntimeLibraries;/")
endif()

INCLUDE (InstallRequiredSystemLibraries)
INCLUDE(CPack)

