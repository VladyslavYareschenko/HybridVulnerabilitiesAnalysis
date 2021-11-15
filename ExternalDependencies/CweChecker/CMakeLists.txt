include(ExternalProject)

set(CWE_CHECKER_LIB_DIR "cwe_checker_lib")
set(CWE_CHECKER_SRC_DIR "cwe_checker_lib")

ExternalProject_Add(
        CweCheckerSetupLib
        PREFIX                             "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_LIB_DIR}"
        TMP_DIR                            "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_LIB_DIR}/tmp"
        STAMP_DIR                          "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_LIB_DIR}/stamp"
        DOWNLOAD_DIR                       "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_LIB_DIR}/${CWE_CHECKER_SRC_DIR}"
        GIT_REPOSITORY                     "https://github.com/VladyslavYareschenko/cwe_checker"
        GIT_TAG                            "master"
        CONFIGURE_COMMAND                  ""
        SOURCE_DIR                         "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_LIB_DIR}/${CWE_CHECKER_SRC_DIR}"
        BUILD_COMMAND                      cargo build --release --lib --target-dir=${CMAKE_BINARY_DIR}/${CWE_CHECKER_COMPONENT_NAME}
        BINARY_DIR                         "${EXTERNAL_DEPENDENCIES_PATH}/${CWE_CHECKER_LIB_DIR}/${CWE_CHECKER_SRC_DIR}"
        INSTALL_COMMAND                    ""
        TEST_BEFORE_INSTALL                ON
        TEST_COMMAND                       cargo test --release
        LOG_CONFIGURE                      1
        LOG_BUILD                          0
        LOG_INSTALL                        1
        LOG_TEST                           0)