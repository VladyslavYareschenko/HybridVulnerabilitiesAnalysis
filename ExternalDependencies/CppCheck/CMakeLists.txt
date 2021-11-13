include(ExternalProject)

message(STATUS "Processing ${CPP_CHECK_COMPONENT_NAME}")

set(CPP_CHECK_LIB_DIR "cppcheck_lib")
set(CPP_CHECK_SRC_DIR "cppcheck_lib")
set(CPP_CHECK_BUILD_DIR "build_cppcheck")
#set(CPP_CHECK_SHARED_LIB "libCPP_CHECK_lib.so")
#string(CONCAT CPP_CHECK_SHARED_LIB_PATH "${EXTERNAL_DEPENDENCIES_PATH}/"
#        "${CPP_CHECK_LIB_DIR}/"
#        "${CPP_CHECK_SRC_DIR}/"
#        "${CPP_CHECK_BINARIES_DIR}/"
#        "${CPP_CHECK_SHARED_LIB}")

ExternalProject_Add(
        ${CPP_CHECK_COMPONENT_NAME}
        PREFIX                             "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}"
        TMP_DIR                            "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/tmp"
        STAMP_DIR                          "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/stamp"
        DOWNLOAD_DIR                       "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/${CPP_CHECK_SRC_DIR}"
        GIT_REPOSITORY                     "https://github.com/danmar/cppcheck"
        GIT_TAG                            "main"
        CONFIGURE_COMMAND                  cmake ../cppcheck_lib -DUSE_MATCHCOMPILER=ON
        SOURCE_DIR                         "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/${CPP_CHECK_SRC_DIR}"
        BUILD_COMMAND                      make -j4
        BINARY_DIR                         "${EXTERNAL_DEPENDENCIES_PATH}/${CPP_CHECK_LIB_DIR}/${CPP_CHECK_BUILD_DIR}"
        BUILD_IN_SOURCE                    0
        INSTALL_COMMAND                    echo Skip CppCheck install)

#add_library(${CPP_CHECK_COMPONENT_NAME} SHARED IMPORTED GLOBAL)
#set_target_properties(CweChecker PROPERTIES IMPORTED_LOCATION "${CPP_CHECK_SHARED_LIB_PATH}")
#add_dependencies(${CPP_CHECK_COMPONENT_NAME} CppCheckSetupLib)