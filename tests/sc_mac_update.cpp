// planMacUpdate: the decision that decides whether WCL's MAC replay restarts the driver.
//
// WCL calls setHardwareAddress() after every driver-available notification. The
// decision must be Unchanged once the driver has adopted the requested address,
// otherwise the driver is disabled/enabled in a loop and DHCP never completes.
#include <WCL.hpp>
#include <cassert>
#include <cstdio>

using AirportWCL::MacUpdate;
using AirportWCL::planMacUpdate;

int main()
{
    const uint8_t priv[6] = {0x9a, 0x50, 0xfc, 0xe8, 0xd7, 0x52};
    const uint8_t hw[6]   = {0xbc, 0xd2, 0x2c, 0x39, 0x3e, 0xf6};

    // Same address: no restart, whatever the state.
    assert(planMacUpdate(priv, priv, 6, false) == MacUpdate::Unchanged);
    assert(planMacUpdate(priv, priv, 6, true) == MacUpdate::Unchanged);

    // A real change: apply, or defer while the interface is still initialising.
    assert(planMacUpdate(priv, hw, 6, false) == MacUpdate::Change);
    assert(planMacUpdate(priv, hw, 6, true) == MacUpdate::Busy);

    // Rejected inputs: wrong length, multicast bit set, all-zero address.
    assert(planMacUpdate(priv, hw, 5, false) == MacUpdate::Invalid);
    const uint8_t multicast[6] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    assert(planMacUpdate(priv, multicast, 6, false) == MacUpdate::Invalid);
    const uint8_t zero[6] = {0};
    assert(planMacUpdate(priv, zero, 6, false) == MacUpdate::Invalid);

    std::printf("sc_mac_update: ok\n");
    return 0;
}
