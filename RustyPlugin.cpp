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
#include <interfaces/json/JGreeter.h>

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

    const string RustyPlugin::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_connectionId == 0);

        string result {};

        _service = service;
        _service->AddRef();

        _service->Register(&_notification);

        _greeter = _service->Root<Exchange::IGreeter>(_connectionId, 2000, _T("RustProxy"));
        if (_greeter) {
            Exchange::JGreeter::Register(*this, _greeter);
        } else {
            result = "Failed to create RustProxy instance";
        }

        return result;
    }

    void RustyPlugin::Deinitialize(PluginHost::IShell* service)
    {
        if (service != nullptr) {
            ASSERT(_service == service);

            _service->Unregister(&_notification);

            if (_greeter != nullptr) {
                RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));

                VARIABLE_IS_NOT_USED uint32_t result = _greeter->Release();
                ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);
                _greeter = nullptr;

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
}
}