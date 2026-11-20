# Local branches (not upstream)

Base: `69e88fa` (`v0.5.4-beta.2`), GPL-2.0.

- `e440-sc`: base + patches `0001`–`0006` from
  `Xe3/patches/forbiddenitlwm-sc/`.
  `0007-sec-key-async` is **not** applied here: it needs `iwx_mld_set_key()` and the MLD
  key path, which are not present in 0.5.4-beta.2 (they arrived with the later v333–v339
  work).
- `v339-local`: full local driver tree (v339), including all accumulated patches,
  MLD key path and `SEC_KEY` async. It still contains local diagnostics (`xe3-*`, `xd*`).

The upstream repo (`origin`) is `retropineaple/ForbiddenItlwm`; these branches are for
local record/push, not PR-ready.
