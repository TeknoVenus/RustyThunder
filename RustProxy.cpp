#include "Module.h"

#include "RustProxy.h"
#include <openssl/evp.h>
#include <rust_beer_bridge/lib.h>

namespace WPEFramework {
namespace Plugin {
    SERVICE_REGISTRATION(RustProxy, 1, 0);

    Core::hresult RustProxy::GetBeers(IBeerIterator*& beers /* @out */)
    {
        Core::hresult success = Core::ERROR_NONE;

        std::list<Exchange::IBeer::Beer> beerList;

        try {
            // Call the rust code
            auto result = get_beers();

            // Convert the rust results to our COM-RPC struct
            for (const auto& beer : result) {
                beerList.emplace_back(Exchange::IBeer::Beer {
                    beer.id,
                    std::string(beer.name),
                    std::string(beer.description) });
            }

        } catch (const std::exception& exception) {
            printf("[C++] Exception occurred in rust code: %s\n", exception.what());
            success = Core::ERROR_GENERAL;
        }

        // Create COM-RPC iterator
        beers = Core::Service<RPC::IteratorType<Exchange::IBeer::IBeerIterator>>::Create<Exchange::IBeer::IBeerIterator>(beerList);
        return success;
    }

    Core::hresult RustProxy::GetBeersByName(string name /* @in */, IBeerIterator*& beers /* @out */)
    {
        Core::hresult success = Core::ERROR_NONE;

        std::list<Exchange::IBeer::Beer> beerList;

        try {
            // Call the rust code
            auto result = get_beer_by_name(name);

            // Convert the rust results to our COM-RPC struct
            for (const auto& beer : result) {
                beerList.emplace_back(Exchange::IBeer::Beer {
                    beer.id,
                    std::string(beer.name),
                    std::string(beer.description) });
            }
        } catch (const std::exception& exception) {
            printf("[C++] Exception occurred in rust code: %s\n", exception.what());
            success = Core::ERROR_GENERAL;
        }

        beers = Core::Service<RPC::IteratorType<Exchange::IBeer::IBeerIterator>>::Create<Exchange::IBeer::IBeerIterator>(beerList);
        return success;
    }

    Core::hresult RustProxy::GetBeer(uint32_t id /* @in */, Beer& beer /* @out */)
    {
        Core::hresult success = Core::ERROR_NONE;

        try {
            // Call the rust code
            auto result = get_beer_by_id(id);

            beer.id = result.id;
            beer.description = std::string(result.description);
            beer.name = std::string(result.name);
        } catch (const std::exception& exception) {
            printf("[C++] Exception occurred in rust code: %s\n", exception.what());
            success = Core::ERROR_GENERAL;
        }
        return success;
    }
}
}
