/*
 *
 * Copyright (c) 2025 Lichens
 *
 * See LICENSE file for more details.
 *
 * Created on Tue Sep 23 2025
 */

#include <gtest/gtest.h>

#include "LichensCPP/log/Logger.h"

class GlobalEnvironment : public ::testing::Environment 
{
public:
    void SetUp() override {
        // runs once before all tests
        LichensCPP::Logger::instance()
            .add_console_logger(LichensCPP::LogLevel::DEBUG)
            .init("test_logger");
    }

    void TearDown() override {
        // runs once after all tests
    }
};

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // Register the global environment
    ::testing::AddGlobalTestEnvironment(new GlobalEnvironment);

    return RUN_ALL_TESTS();
}