include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../source)

# If pkg-config is not installed on the system, then the
# CHECK_INSTALL_DIR variable must be set to the install
# location of Check. For example, on Windows, this may
# be: C:/Program Files/check
# set(CHECK_INSTALL_DIR "C:/Program Files/check")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})


set(TEST_HEADERS
     main.h utils/check_utils.h server/test_suite_commands.h
)

set(TEST_SOURCES
    main.c utils/check_utils.c server/test_suite_commands.c
)

# entire test executable
add_executable(run_unit_tests ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(run_unit_tests DudeServerLib ${CHECK_LIBRARIES} pthread)

# Server test client
add_executable(DudeTestClient client/test_client.c)
target_link_libraries(DudeTestClient DudeServerLib)

# remove all older database files
add_custom_command(TARGET run_unit_tests PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E remove_directory
$<TARGET_FILE_DIR:run_unit_tests>/xml)
add_custom_command(TARGET run_unit_tests POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/xml $<TARGET_FILE_DIR:run_unit_tests>/xml)