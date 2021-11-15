include(ExternalProject)

set(VALGRIND_HOME_DIR "${VALGRIND_COMPONENT_NAME}")
set(VALGRIND_SRC_DIR "valgrind")
set(VALGRIND_BIN_DIR "build_${VALGRIND_SRC_DIR}")
set(VALGRIND_BIN_PATH "${EXTERNAL_DEPENDENCIES_PATH}/${VALGRIND_HOME_DIR}/${VALGRIND_BIN_DIR}")

ExternalProject_Add(
        ${VALGRIND_COMPONENT_NAME}
        PREFIX                             "${EXTERNAL_DEPENDENCIES_PATH}/${VALGRIND_HOME_DIR}"
        TMP_DIR                            "${EXTERNAL_DEPENDENCIES_PATH}/${VALGRIND_HOME_DIR}/tmp"
        STAMP_DIR                          "${EXTERNAL_DEPENDENCIES_PATH}/${VALGRIND_HOME_DIR}/stamp"
        DOWNLOAD_DIR                       "${EXTERNAL_DEPENDENCIES_PATH}/${VALGRIND_HOME_DIR}/${VALGRIND_SRC_DIR}"
        GIT_REPOSITORY                     "git://sourceware.org/git/valgrind.git"
        GIT_TAG                            "VALGRIND_3_18_1"
        CONFIGURE_COMMAND                  ./autogen.sh && ./configure --prefix=${CMAKE_BINARY_DIR}/${VALGRIND_COMPONENT_NAME}
        SOURCE_DIR                         "${EXTERNAL_DEPENDENCIES_PATH}/${VALGRIND_HOME_DIR}/${VALGRIND_SRC_DIR}"
        BUILD_COMMAND                      make -j4
        BUILD_IN_SOURCE                    1
        INSTALL_COMMAND                    make install)