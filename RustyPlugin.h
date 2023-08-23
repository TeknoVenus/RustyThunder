#pragma once

#include "Module.h"

#include <interfaces/IGreeter.h>

namespace WPEFramework {
namespace Plugin {
    class RustyPlugin : public PluginHost::IPlugin, public PluginHost::JSONRPC {
    private:
        class Notification : public RPC::IRemoteConnection::INotification {
        public:
            explicit Notification(RustyPlugin* parent)
                : _parent(*parent)
            {
                ASSERT(parent != nullptr);
            }

            ~Notification() override = default;

            Notification(Notification&&) = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(Notification&&) = delete;
            Notification& operator=(const Notification&) = delete;

        public:
            void Activated(RPC::IRemoteConnection* /* connection */) override
            {
            }
            void Deactivated(RPC::IRemoteConnection* connectionId) override
            {
                _parent.Deactivated(connectionId);
            }

            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            END_INTERFACE_MAP

        private:
            RustyPlugin& _parent;
        };

    public:
        RustyPlugin()
            : _connectionId(0)
            , _service(nullptr)
            , _greeter(nullptr)
            , _notification(this)
        {
        }
        ~RustyPlugin() override = default;

        RustyPlugin(RustyPlugin&&) = delete;
        RustyPlugin(const RustyPlugin&) = delete;

        RustyPlugin& operator=(RustyPlugin&&) = delete;
        RustyPlugin& operator=(const RustyPlugin&) = delete;

        BEGIN_INTERFACE_MAP(RustyPlugin)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        INTERFACE_AGGREGATE(Exchange::IGreeter, _greeter)
        END_INTERFACE_MAP

        // IPlugin Methods
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;

    private:
        void Deactivated(RPC::IRemoteConnection* connection);

    private:
        uint32_t _connectionId;
        PluginHost::IShell* _service;

        Exchange::IGreeter* _greeter;
        Core::Sink<Notification> _notification;
    };
}
}