/*
 * Created on Tue Sep 09 2025
 *
 * Copyright (c) 2025 Lichens Inc. All rights reserved.
 */

#include "LichensCPP/mqtt/MqttHelper.h"

#include <cstdlib>
#include <regex>
#include <unordered_map>

#include <mqtt/async_client.h>

#include "LichensCPP/log/LoggerHelper.h"

namespace LichensCPP
{

// See https://github.com/eclipse-paho/paho.mqtt.cpp/blob/v1.5.3/include/mqtt/connect_options.h#L457
enum class MqttVersion
{
    MQTT_VERSION_3_1 = 3,
    MQTT_VERSION_3_1_1 = 4,
    MQTT_VERSION_5 = 5
};

struct Subscription
{
    std::string topic;
    std::regex topic_regex;
    CallbackMqttMessage callback;
};

struct MqttHelper::MqttHelperPrivate
{
    std::string mqtt_name;
    std::string mqtt_broker;
    int mqtt_port;
    int qos;
    size_t subscribe_id;
    std::shared_ptr<mqtt::async_client> mqtt_client;
    std::unordered_map<size_t, Subscription> subscriptions;

    MqttHelperPrivate(const std::string& name, const std::string& broker, int port)
        : mqtt_name(name)
        , mqtt_broker(broker)
        , mqtt_port(port)
        , qos(1)
        , subscribe_id(1u)
        , mqtt_client(nullptr)
        , subscriptions()
    {
        const std::string address = "tcp://" + mqtt_broker + ":" + std::to_string(mqtt_port);
        LOG_INFO_F("Creating client to MQTT broker at %s with name: %s", address.c_str(), mqtt_name.c_str());
        mqtt_client = std::make_shared<mqtt::async_client>(address, mqtt_name);
        mqtt_client->set_message_callback([this](mqtt::const_message_ptr msg) {
            this->on_message_rx(msg);
        });
    }

    size_t get_next_subscribe_id()
    {
        return subscribe_id++;
    }

    std::regex to_regex(const std::string& topic)
    {
        std::string regex_pattern;
        regex_pattern.reserve(topic.size() * 2u);

        // Full anchors match
        regex_pattern += "^";

        for (size_t i = 0u; i < topic.size(); ++i)
        {
            char c = topic[i];
            if (c == '+')
            {
                regex_pattern += "[^/]+";  // one level
            } 
            else if (c == '#')
            {
                // '#' must be at end
                if (i + 1 != topic.size())
                {
                    throw std::invalid_argument("Invalid MQTT filter: '#' must be last");
                }
                regex_pattern += ".*"; // match rest
            }
            else
            {
                // Escape regex special chars
                if (std::string(".^$|()[]*+?{}\\").find(c) != std::string::npos)
                {
                    regex_pattern += '\\';
                }
                regex_pattern += c;
            }
        }

        // Ending anchors
        regex_pattern += "$";

        // full match anchors
        return std::regex(regex_pattern, std::regex::ECMAScript | std::regex::optimize);
    }

    void on_message_rx(mqtt::const_message_ptr msg)
    {
        LOG_TRACE_F("Received message on topic %s", msg->get_topic().c_str());
        for(const auto& [_, s] : subscriptions)
        {
            if(s.callback && std::regex_match(msg->get_topic(), s.topic_regex))
            {
                const auto& payload = msg->get_payload();
                s.callback(payload.data(), payload.size());
            }
        }    
    }

    void on_connection_lost(const std::string& cause)
    {
        LOG_WARNING_F("MQTT connection lost: %s", cause.c_str());
        LOG_INFO("Attempting to reconnect to MQTT broker...");
        try
        {
            auto tok = mqtt_client->reconnect();
            tok->wait();
            
            if(tok->get_return_code() != mqtt::SUCCESS)
            {
                LOG_FATAL_F("Reconnection failed with return code: %d", tok->get_return_code());
                std::exit(-2000);
            } 
            else
            {
                LOG_INFO("Reconnected to MQTT broker successfully");
            }
        }
        catch (const mqtt::exception& e)
        {
            LOG_FATAL_F("MQTT reconnection error: %s", e.what());
            std::exit(-2001);
        }
    }

    bool connect()
    {
        if(mqtt_client->is_connected())
        {
            LOG_WARNING("MQTT client already connected");
            return true;
        }

        mqtt::connect_options conn_options;
        conn_options.set_mqtt_version(static_cast<int>(MqttVersion::MQTT_VERSION_3_1_1));
        conn_options.set_clean_start(true);
        conn_options.set_clean_session(true);
        conn_options.set_automatic_reconnect(true);
        conn_options.set_keep_alive_interval(60);
        conn_options.set_connect_timeout(30);

        try
        {
            LOG_INFO("Connecting to MQTT broker...");
            mqtt_client->connect(conn_options)->wait();
            LOG_INFO_F("Connected as %s to MQTT broker at %s:%d", mqtt_name.c_str(), mqtt_broker.c_str(), mqtt_port);
            mqtt_client->set_connection_lost_handler([this](const std::string& cause) {
                this->on_connection_lost(cause);
            });
        }
        catch (const mqtt::exception& e)
        {
            LOG_ERROR_F("MQTT connection error: %s", e.what());
        }
        return mqtt_client->is_connected();
    }

    void disconnect()
    {
        try
        {
            if (mqtt_client && mqtt_client->is_connected())
            {
                mqtt_client->disconnect()->wait();
                LOG_INFO_F("Disconnected as %s from MQTT broker at %s:%d", mqtt_name.c_str(), mqtt_broker.c_str(), mqtt_port);
            }
        }
        catch (const mqtt::exception& e)
        {
            LOG_ERROR_F("MQTT disconnection error: %s", e.what());
        }
    }

    size_t subscribe(const std::string& topic, CallbackMqttMessage callback)
    {
        size_t id = get_next_subscribe_id();

        LOG_TRACE_S("Subscribing to topic: ", topic);

        subscriptions[id].topic = topic;
        subscriptions[id].topic_regex = to_regex(topic);
        subscriptions[id].callback = callback;
        
        mqtt::topic sub_topic(*mqtt_client, topic, qos);
        auto sub_opts = mqtt::subscribe_options(false);
        sub_topic.subscribe(sub_opts)->wait();
        return id;
    }

    void publish(const std::string& topic, const void* message, size_t size)
    {
        try
        {
            mqtt_client->publish(topic, message, size)->wait();
            LOG_INFO_F("Published message to topic %s", topic.c_str());
        }
        catch (const mqtt::exception& e)
        {
            LOG_ERROR_F("MQTT publish error: %s", e.what());
        }
    }
};

MqttHelper::MqttHelper(const std::string& name, const std::string& broker, int port)
    : p_(std::make_unique<MqttHelperPrivate>(name, broker, port))
{
}

MqttHelper::~MqttHelper()
{
    disconnect();
}

std::string MqttHelper::name() const 
{ 
    return p_->mqtt_name;
}

std::string MqttHelper::broker() const 
{ 
    return p_->mqtt_broker;
}

int MqttHelper::port() const 
{ 
    return p_->mqtt_port; 
}

bool MqttHelper::connect()
{
    return p_->connect();
}

void MqttHelper::disconnect()
{
    p_->disconnect();
}

size_t MqttHelper::subscribe(const std::string& topic, CallbackMqttMessage callback) const
{
    return p_->subscribe(topic, callback);
}

void MqttHelper::publish(const std::string& topic, const std::vector<std::byte>& message) const
{
    p_->publish(topic, message.data(), message.size());
}

void MqttHelper::publish(const std::string& topic, const std::string& message) const
{
    p_->publish(topic, message.data(), message.size());
}

void MqttHelper::publish(const std::string& topic, const void* message, size_t size) const
{
    p_->publish(topic, message, size);
}

std::string MqttHelper::msg_to_string(const void* const message_ptr, size_t message_size)
{
    return {reinterpret_cast<const char*>(message_ptr), message_size};
}

std::vector<std::byte> MqttHelper::msg_to_bytes(const void* const message_ptr, size_t message_size)
{
    const std::byte* start = reinterpret_cast<const std::byte*>(message_ptr);
    return {start, start + message_size};
}

} // namespace LichensCPP