/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on 2025-11-24
 */

#include <gtest/gtest.h>
#include "LichensCPP/scope_action.h"

using namespace LichensCPP;

TEST(ScopeActionTest, scopeActionExecutesOnDestruction) 
{
    bool executed = false;
    {
        ScopeAction action([&executed]() {
            executed = true;
        });
        EXPECT_FALSE(executed);
    }
    EXPECT_TRUE(executed);
}