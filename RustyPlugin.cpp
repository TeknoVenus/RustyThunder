/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Metrological
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RustyPlugin.h"
#include <interfaces/json/JBeer.h>

namespace WPEFramework {

namespace Plugin {

    namespace {

        static Metadata<RustyPlugin> metadata(
            // Version
            1, 0, 0,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            {});
    }

    RustyPlugin::RustyPlugin()
        : _connectionId(0)
        , _service(nullptr)
        , _beer(nullptr)
        , _notification(this)
    {
    }

    const string RustyPlugin::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_connectionId == 0);

        string result {};

        _service = service;
        _service->AddRef();

        _service->Register(&_notification);

        _beer = _service->Root<Exchange::IBeer>(_connectionId, 2000, _T("RustProxy"));
        if (_beer) {
            Exchange::JBeer::Register(*this, _beer);
        } else {
            result = "Failed to create RustProxy instance for IBeer";
        }

        return result;
    }

    void RustyPlugin::Deinitialize(PluginHost::IShell* service)
    {
        if (service != nullptr) {
            ASSERT(_service == service);

            _service->Unregister(&_notification);

            if (_beer != nullptr) {
                RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));

                VARIABLE_IS_NOT_USED uint32_t result = _beer->Release();
                ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);
                _beer = nullptr;

                if (connection != nullptr) {
                    connection->Terminate();
                    connection->Release();
                }
            }

            _service->Release();
            _service = nullptr;
            _connectionId = 0;
        }
    }

    string RustyPlugin::Information() const
    {
        return {};
    }

    void RustyPlugin::Deactivated(RPC::IRemoteConnection* connection)
    {
        if (connection->Id() == _connectionId) {
            ASSERT(_service != nullptr);
            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service,
                PluginHost::IShell::DEACTIVATED,
                PluginHost::IShell::FAILURE));
        }
    }
}
}