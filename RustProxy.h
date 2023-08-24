#pragma once

#include "Module.h"

#include <interfaces/IBeer.h>


namespace WPEFramework {
namespace Plugin {
    class RustProxy : public Exchange::IBeer {
    public:
        RustProxy() = default;
        ~RustProxy() override = default;

        RustProxy(RustProxy&&) = delete;
        RustProxy(const RustProxy&) = delete;

        RustProxy& operator=(RustProxy&&) = delete;
        RustProxy& operator=(const RustProxy&) = delete;

        BEGIN_INTERFACE_MAP(RustProxy)
        INTERFACE_ENTRY(Exchange::IBeer)
        END_INTERFACE_MAP

        // IBeer methods
        Core::hresult GetBeers(IBeerIterator*& beers /* @out */) override;
        Core::hresult GetBeersByName(string name /* @in */, IBeerIterator*& beers /* @out */) override;
        Core::hresult GetBeer(uint32_t id /* @in */, Beer& beer /* @out */) override;
    };
}
}