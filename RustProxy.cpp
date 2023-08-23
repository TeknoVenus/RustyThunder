#include "Module.h"

#include "RustProxy.h"

namespace WPEFramework {
namespace Plugin {
    SERVICE_REGISTRATION(RustProxy, 1, 0);

    Core::hresult RustProxy::SayHello(const string name, string& message)
    {
        printf("[CPP Plugin] About to call rust code\n");
        char* greeting = greeting_generate(name.c_str());
        printf("[CPP Plugin] Rust code returned %s\n", greeting);

        message = std::string(greeting, strlen(greeting));
        greeting_free(greeting);

        return Core::ERROR_NONE;
    }
}
}
