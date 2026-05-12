#include "nvapi_private.h"
#include "nvapi_globals.h"
#include "nvapi/nvapi_adapter_registry.h"
#include "util/util_statuscode.h"

using namespace dxvk;

// Stubs for NVIDIA-internal NVAPI function IDs that aren't published in any
// released NVAPI SDK through R595, but are queried by Streamline 2.x's closed
// DLSS-G plugin, NVIDIA's _nvngx.dll runtime, and by RE Engine titles
// (PRAGMATA) on RTX 50-series under Proton. Without these the proton log fills
// with "Unknown function ID" entries and Streamline disables DLSS-G —
// cascading into RE Engine grayingt out RT/PT/DLSS-RR menu options.
//
// Each stub returns NVAPI_OK only on Blackwell GPUs (NV_GPU_ARCHITECTURE_GB200
// and later); other architectures get NVAPI_NOT_SUPPORTED, preserving prior
// behavior for Ada/Ampere/Turing users.
//
// Behavior is the minimum needed for Streamline's "function reachable + capable"
// gate to pass. If runtime needs more (specific out-struct fields), iterate
// based on the next proton-log fingerprint after these land.

namespace {
    // Returns true when the first NVIDIA adapter is Blackwell-class.
    // Mirrors the architecture predicate used in nvapi_d3d12.cpp:450.
    bool isBlackwellOrLater() {
        if (nvapiAdapterRegistry == nullptr)
            return false;

        auto adapter = nvapiAdapterRegistry->GetFirstAdapter();
        if (adapter == nullptr)
            return false;

        return adapter->GetArchitectureId() >= NV_GPU_ARCHITECTURE_GB200;
    }
}

// Tier 2 evidence: only NVAPI function-name string present in PRAGMATA.exe.
// Called directly by the game; on failure PRAGMATA falls back to NvAPI_Unload
// (gives up entirely). Returning OK for Blackwell satisfies the existence check.
NVAPI_FUNCTION NVAPI_Notify_PresentBarrierSupported() {
    constexpr auto n = __func__;

    if (log::tracing())
        log::trace(n);

    if (!isBlackwellOrLater())
        return NoImplementation(n);

    return Ok(n);
}

// Tier 4 (blind): Streamline DLSS-G plugin (sl.dlss_g.dll) queries this first
// during plugin startup. Function pointer is stored at sl.dlss_g.dll global
// 0x180068338 / 0x180068350 and used later. Returning a non-null reachable
// function lets Streamline proceed past the "QueryInterface returned NULL" gate.
NVAPI_FUNCTION Streamline_Private_ad298d3f() {
    constexpr auto n = __func__;

    if (log::tracing())
        log::trace(n);

    if (!isBlackwellOrLater())
        return NoImplementation(n);

    return Ok(n);
}

// Tier 4 (blind): Streamline DLSS-G plugin secondary capability query, called
// after NvAPI_Initialize succeeds. Stored at sl.dlss_g.dll global 0x180068358.
NVAPI_FUNCTION Streamline_Private_33c7358c() {
    constexpr auto n = __func__;

    if (log::tracing())
        log::trace(n);

    if (!isBlackwellOrLater())
        return NoImplementation(n);

    return Ok(n);
}

// Tier 4 (blind): Streamline DLSS-G plugin tertiary capability query, gated
// on the previous two succeeding. Stored at sl.dlss_g.dll global 0x180068360.
NVAPI_FUNCTION Streamline_Private_593e8644() {
    constexpr auto n = __func__;

    if (log::tracing())
        log::trace(n);

    if (!isBlackwellOrLater())
        return NoImplementation(n);

    return Ok(n);
}

// Tier 4 (blind): NVIDIA NGX runtime (_nvngx.dll) private callback. Called in
// a 4-step register/use/unregister sequence around a stack buffer at
// _nvngx.dll+0x180002d10, suggesting a per-context handle/notification API.
NVAPI_FUNCTION NGX_Private_a782ea46() {
    constexpr auto n = __func__;

    if (log::tracing())
        log::trace(n);

    if (!isBlackwellOrLater())
        return NoImplementation(n);

    return Ok(n);
}
