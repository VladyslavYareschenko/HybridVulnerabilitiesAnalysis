include(ExternalProject)

set(CPP_CHECK_LIB_DIR "cppcheck_lib")
set(CPP_CHECK_SRC_DIR "cppcheck_lib")
set(CPP_CHECK_BUILD_DIR "build_cppcheck")

ExternalProject_Add(
        ${CPP_CHECK_COMPONENT_NAME}
        PREFIX                             "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}"
        TMP_DIR                            "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/tmp"
        STAMP_DIR                          "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/stamp"
        DOWNLOAD_DIR                       "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/${CPP_CHECK_SRC_DIR}"
        GIT_REPOSITORY                     "https://github.com/danmar/cppcheck"
        GIT_TAG                            "main"
        CONFIGURE_COMMAND                  cmake ${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/${CPP_CHECK_SRC_DIR} -DUSE_MATCHCOMPILER=ON
        SOURCE_DIR                         "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/${CPP_CHECK_SRC_DIR}"
        BUILD_COMMAND                      make -j4
        BINARY_DIR                         "${CMAKE_BINARY_DIR}/${CPP_CHECK_COMPONENT_NAME}"
        BUILD_IN_SOURCE                    0
        INSTALL_COMMAND                    "")