#pragma once
#include "Module.h"

// @stubgen:include <com/IIteratorType.h>

namespace WPEFramework {
namespace Exchange {
    /* @json */
    struct EXTERNAL IBeer : virtual public Core::IUnknown {
        enum { ID = ID_BEER };

        struct Beer {
            uint32_t id;
            string name;
            string description;
        };
        using IBeerIterator = RPC::IIteratorType<Beer, ID_BEER_ITERATOR>;

        virtual Core::hresult GetBeers(IBeerIterator*& beers /* @out */) = 0;

        virtual Core::hresult GetBeersByName(string name /* @in */, IBeerIterator*& beers /* @out */) = 0;

        virtual Core::hresult GetBeer(uint32_t id /* @in */, Beer& beer /* @out */) = 0;
    };
}
}
