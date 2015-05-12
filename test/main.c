//
// Created by osboxes on 01/05/15.
//

#include "utils/check_utils.h"
#include "database/check_user.h"

int main()
{
    int utils = utils_tests();
     utils += user_tests();

    return (utils > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}