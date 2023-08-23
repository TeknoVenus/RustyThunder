#pragma once

#include "Module.h"

#include <interfaces/IGreeter.h>

// Methods from Rust
extern "C" {
    extern char *greeting_generate(const char *name);
    extern void greeting_free(char *);
}
// End methods from Rust

namespace WPEFramework {
namespace Plugin {
    class RustProxy : public Exchange::IGreeter {
    public:
        RustProxy() = default;
        ~RustProxy() override = default;

        RustProxy(RustProxy&&) = delete;
        RustProxy(const RustProxy&) = delete;

        RustProxy& operator=(RustProxy&&) = delete;
        RustProxy& operator=(const RustProxy&) = delete;

        BEGIN_INTERFACE_MAP(RustProxy)
        INTERFACE_ENTRY(Exchange::IGreeter)
        END_INTERFACE_MAP

        // IGreeter methods
        Core::hresult SayHello(const string name, string& message) override;
    };
}
}