# wifi: add Intel AX211 on SC firmware (8086:e440) support

## What

This adds support for the Intel AX211 CNVi Wi-Fi on the SC firmware family
(`iwlwifi-sc-a0-gf-a0-100.ucode`, PCI id `8086:e440`). The card needs the
firmware's MLD command API: the legacy `ADD_STA` / `SCAN_CFG` / queue
commands this driver uses for 9000/AX2xx parts are not in SC's command
table, so without the MLD path association and RX block-ack cannot complete.

## Changes

1. **Device + firmware** (`if_iwxreg.h`, `ItlIwx.cpp`): `0xe440`, the
   `sc-a0-gf-a0` firmware name and its 70-section layout, BZ/SC register
   semantics, MSI-X setup.
2. **MLD API** (`ItlIwx.cpp`, `if_iwxreg.h`, `if_iwxvar.h`): MAC/LINK/STA
   config, `STA_REMOVE`, `SEC_KEY` (0x518) and the station-state bookkeeping
   it replaces.
3. **SC command versions**: `SCAN_CFG` v5, `SCAN_REQ_UMAC` v17,
   `SCD_QUEUE_CONFIG` v3, RX BAID allocation v2 (`0x516`).
4. **RX block-ack**: `iwx_sta_rx_agg()` uses `RX_BAID_ALLOCATION_CONFIG` on
   MLD firmware and takes the BAID from the command response; the legacy
   `ADD_STA` status decode is only used for non-MLD firmware.
5. **WCL callbacks**: `setWCL_LINK_UP_DONE` and `setWCL_QOS_PARAMS` return
   success on this stack, which macOS 26 needs before it feeds data frames
   to the driver.

## Gating

Every change that can run on an existing device is behind either
`sc->sc_use_mld_api` (set only when the firmware advertises MLD) or
`sc->sc_device_family >= IWX_DEVICE_FAMILY_SC`:

- `bcast_sta_id = 0` only for SC (was already gated in the original work).
- interrupt re-arm on `SW_ERR`/`HW_ERR` only for SC; older families keep the
  original `goto out` path.
- `iwx_sta_rx_agg()` keeps the legacy `ADD_STA` path when not MLD.
- SEC_KEY / MAC / LINK / STA config are new call sites, unreachable for
  non-SC devices.

## Testing

- Builds with `tools/build.py --target tahoe` against 0.6.0-beta.2.
- The same code (0.5.4-based tree) was run on the target machine:
  1732 Mb/s MCS9/160 MHz link, 281 Mb/s TCP download on a 300 M line,
  2.9 ms gateway RTT, 0% loss, 440 Mb/s LAN.
- **Not yet booted from this 0.6.0-based branch**; needs a hardware pass
  before merging.

## Notes

- The bring-up diagnostics from the local tree (`xe3-*` / `xd*` ioreg
  properties, `sc_diag_*` counters and the publish helpers) are removed; the
  only remaining trace is the `sc_diag`-free MLD bookkeeping in `if_iwxvar.h`.
- The SC interrupt re-arm is kept behind the SC gate; it can be dropped later
  if a test run shows the SC firmware survives without it.
