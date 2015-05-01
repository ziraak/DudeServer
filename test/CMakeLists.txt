include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# If pkg-config is not installed on the system, then the
# CHECK_INSTALL_DIR variable must be set to the install
# location of Check. For example, on Windows, this may
# be: C:/Program Files/check
# set(CHECK_INSTALL_DIR "C:/Program Files/check")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

set(TEST_SOURCES
    main.c utils/check_utils.c utils/check_utils.h
)

# entire test executable
add_executable(run_unit_tests ${TEST_SOURCES})
target_link_libraries(run_unit_tests DudeServerLib ${CHECK_LIBRARIES} pthread)

#add_executable(DudeTestClient ${HEADERS} client/test_client.c)
#target_link_libraries(DudeTestClient DudeServerLib)
