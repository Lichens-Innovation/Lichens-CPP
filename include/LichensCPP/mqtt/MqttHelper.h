/*
 * Created on Tue Sep 09 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace LichensCPP
{

using CallbackMqttMessage = std::function<void(const void* message_ptr, size_t message_size)>;

class MqttHelper
{
public:
    MqttHelper(const std::string& name, const std::string& broker, int port = 1883);
    ~MqttHelper();

    bool connect();
    void disconnect();

    size_t subscribe(const std::string& topic, CallbackMqttMessage callback) const;
    void unsubscribe(size_t subscribe_id) const;

    void publish(const std::string& topic, const std::vector<std::byte>& message) const;
    void publish(const std::string& topic, const std::string& message) const;
    void publish(const std::string& topic, const void* message, size_t size) const;

    static std::string msg_to_string(const void* const message_ptr, size_t message_size);
    static std::vector<std::byte> msg_to_bytes(const void* const message_ptr, size_t message_size);

    std::string name() const;
    std::string broker() const;
    int port() const;

private:
    struct MqttHelperPrivate;
    std::unique_ptr<MqttHelperPrivate> p_;
};

} // namespace LichensCPP
