# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog v0.3.0](http://keepachangelog.com/en/0.3.0/) and this project adheres to [Semantic Versioning](http://semver.org/).

## [unreleased] - xxxx-xx-xx

### Added

- Add host information to /info endpoint (request)

### Changed

- Stats are now separate for each mod (#6). Player stats are saved and read from `players\<fs_game>` folder. Keep in mind this also means that level, XP and classes will not be synchronized with the main stats file after this point.

## [0.6.0] - 2018-12-30

### Added

- Implement unbanclient command.
- Implement /serverlist api endpoint on dedicated servers
- Add dvar to control the server query rate (net_serverQueryLimit & net_serverFrames)

### Changed

- Update dependencies
 
### Fixed

- Fix mods not working in private matches.
- Fix multiple game structures (map tools)
- Fix multiple vulnerability's
- Fix lag spikes on lower end PCs
- Fix invalid name check
- Fix openLink command crash issue
- Fix lobby server map downloading
- Fix steam integration

### Known issues

- HTTPS is not supported for fast downloads at the moment.

## [0.5.4] - 2017-07-09

### Added

- Integrate IW4MVM by luckyy.

### Changed

- Displayed stats for Dragunov have been changed, has no effect on actual game play.

### Fixed

- Fix fast download failing when the target host is missing a trailing slash.
- Fix servers not being listed in the server browser.
- Fix some FPS drop issues caused by compression code.

### Known issues

- HTTPS is not supported for fast downloads at the moment.

## [0.5.3] - 2017-07-02

### Added

- Increase webserver security.

### Fixed

- Reduce lags introduced by nodes.
- Fix modlist download.

## [0.5.2] - 2017-07-02

### Fixed

- Fix dedicated server crash.

## [0.5.1] - 2017-07-02

### Added

- Add fast download option for custom mods/maps based on Call of Duty 4.
- Display a toast when an update is available.
- Use the hourglass cursor while loading assets (with the native cursor feature).
- Show bots in parenthesis after the number of players in the serverlist (request).
- Add GSC event `level waittill("say", string, player);` (request).
- Restrict unauthorized mod download for password protected servers.
- Add OMA support for 15 classes.

### Changed

- Show friend avatars when they play IW4x (request).
- Rewrite and optimize the entire node system.
- Remove syncnode command for node system.
- Remove steam start.

### Fixed

- Fix lags and frame drops caused by server sorting.
- Fix demos on custom maps.
- Can no longer join a lobby or server with an incorrect password.
- Fix crashes caused by a bug in file/data compression.
- Improve overall stability.

## [0.5.0] - 2017-06-04

### Added

- Add GSC functionality to download files via HTTP(S) (request).
- Implement preliminary custom map support.

### Changed

- Add new nicknames for bots.
- Bumped Fastfile version. If you built fastfiles with the zone builder (e.g. mod.ff) you have to rebuild them!
- Default `sv_network_fps` to `1000` on dedicated game servers.
- Increase maximum FOV to 120.

### Fixed

- Fix `iw4x_onelog` dvar.
- Fix server list only showing local servers by default instead of Internet servers.
- Fix some deadlock situations on game shutdown.

## [0.4.2] - 2017-03-16

### Changed

- Disable unnecessary dvar update in server browser.
- Update bot names.

### Fixed

- Fix process permissions.
- Fix classic AK-47 color bleedthrough.
- Re-aligned the MG4's Raffica iron sights.

## [0.4.1] - 2017-03-10

### Fixed

- Fix command line argument parsing.

## [0.4.0] - 2017-03-10

### Added

- Set played with status.
- Add support for 15 classes.
- Add `iw4x_onelog` dvar.
- Add show server/playercount in server browser.

### Changed

- Do not show friend status notifications with stream friendly UI.
- Reduce loaded modules for dedis.
- Use joystick emblem for friend status.
- Disable XP bar when max level.
- Change fs_game display postition.
- Replace Painkiller with Juiced from IW5.

### Fixed

- Fix AK weaponfiles.
- Fix brightness slider.
- Fix text length for column mod in server browser.
- Changed the L86 LSW to use the correct HUD icon.
- Re-aligned the M93 Raffica's iron sights.
- Re-aligned the Desert Eagle's iron sights.

## [0.3.3] - 2017-02-14

### Added

- Add mapname to friend status (request).
- Add option to toggle notify friend state.
- Add support for mod.ff.

### Changed

- Disabled big minidump message box.
- Limit dedicated servers to 15 instances per IP.
- Move build number location.
- Remove news ticker and friends button from theater.

### Fixed

- Fix audio bug in options menu.
- Fix crash caused by faulty file download requests to game hosts.
- Fix friend sorting.
- Fix game not starting issue under certain circumstances.
- Fix menu crash.
- Fix typo in security increase popmenu.
- Fix vid_restart crash with connect protocol.
- Fix weapon crash issue.
- Potentially fix no-ammo bug.

## [0.3.2] - 2017-02-12

This is the third public Beta version.

### Added

- Add working friend system.
- Add colored pings in the server list.
- Add credits.
- Add first launch menu.
- Add AK-47 (Classic) attachments.
- Add HUD icon for night vision goggles.

### Changed

- Join when pressing enter in server list (request).
- Redesign and refactor all fullscreen menus.
- Increase weapon and configstring limit.
- Allow creating full crash dumps if wanted.
- Set default name from steam.

### Fixed

- Fix missing models on village.
- Fix custom server motd (request).
- Fix various memory leaks.
- Fix mouse pitch (request).
- Fix compatibility with B3 (request).
- Fix RCon bug (request).
- Fix dedicated server crash on linux.
- Fix crash in mod download.
- Fix peacekeeper reload sound.
- Fix cl_maxpackets 125 in settings (request).
- Fix deserteaglegold_mp icon.

### Known issues

- IW4x on Linux currently requires gnutls to be installed to access the Tor service via HTTPS.

## [0.3.1] - 2017-01-21

This is the second public Beta version.

### Added

- Add classic AK-47 to CAC.
- Add servers to favorites when ingame.
- Add delete favorites button in the serverlist.

### Changed

- Change maplist to a dynamic list.

### Fixed

- Fix list focus.
- Fix mod restart loop.
- Fix mod download status.
- Fix modelsurf crash.
- Fix floating AK-74u.

### Known issues

- IW4x on Linux currently requires gnutls to be installed to access the Tor service via HTTPS.

## [0.3.0] - 2017-01-14

This is the first public Beta version.

### Added

- Add com_logFilter dvar.
- Add peacekeeper.
- Add support for maps from DLC 8 (Recycled Pack)

  - Chemical Plant (mp_storm_spring)
  - Crash: Tropical (mp_crash_tropical)
  - Estate: Tropical (mp_estate_tropical)
  - Favela: Tropical (mp_fav_tropical)
  - Forgotten City (mp_bloc_sh)

### Changed

- Improve node synchronization handling.
- Improve security by modifying GUIDs to allow 64-bit certificate fingerprints.
- Optimize fastfiles, they are now a lot smaller.
- Replace intro.

### Fixed

- Fix concurrent image loading bug.
- Fix issues when spawning more than one bot.
- Fix no ammo bug.
- Fix server crash on startup.
- Fix splash screen hang.

### Known issues

- IW4x on Linux currently requires gnutls to be installed to access the Tor service via HTTPS.

## [0.2.1] - 2016-12-14

This is the second public Alpha version.

### Added

- Add support for CoD:Online maps.

    - Firing Range (mp_firingrange)
    - Rust (mp_rust_long)
    - Shipment (mp_shipment/mp_shipment_long)

- Add `sv_motd` Dvar for server owners to set custom motd (will be visible in the loadscreen).
- Add Zonebuilder support for sounds and fx.
- Add command setviewpos.
- Add high-definition loadscreens.

### Changed

- Rename Arctic Wet Work map to it's official name (Freighter).
- Complete redesign of the main menus.
- Allow `cl_maxpackets` to be set up to 125.

### Fixed

- Fix crash when using the Harrier killstreak.
- Disable code that downloads news/changelog when in zonebuilder mode.
- Fix freeze on game shutdown.
- Disable unlockstats while ingame to prevent a crash.

### Known issues

- IW4x on Linux currently requires gnutls to be installed to access the Tor service via HTTPS.

## [0.2.0] - 2016-10-09

This is the first public Alpha version.

### Added

- Support for CoD:Online maps.

    - Arctic Wet Work (mp_cargoship_sh)
    - Bloc (mp_bloc)
    - Bog (mp_bog_sh)
    - Crossfire (mp_cross_fire)
    - Killhouse (mp_killhouse)
    - Nuketown (mp_nuked)
    - Wet Work (mp_cargoship)

### Fixed

- Fix techniques in Zonebuilder.
- Fix possible memory leak.
- Fix timeout bug when connecting to server via iw4x link.
- Partially fix deadlock in decentralized networking code.

### Known issues

- Running IW4x on Linux currently requires gnutls to be installed additional to Wine as it needs to access the Tor service via HTTPS.

## [0.1.1] - 2016-09-19

This version is an internal Pre-Alpha version.

### Added

- Add IW5 material embedding system.

### Changed

- Enhance mod download with detailed progress display.

### Fixed

- Fix and optimize network logging commands.
- Fix console not displaying command inputs properly.
- Fix crash when running multiple instances of the IW4x client from the same directory.
- Fix crash when the `securityLevel` command is used on a game server.
- Fix possible data corruption in code for decentralized networking.
- Fix possible deadlock during client shutdown.

## [0.1.0] - 2016-09-03

This version is an internal Pre-Alpha version.

### Added

- Add `banclient` command which will permanently ban a client from a server. The ban will persist across restarts.
- Add capabilities to save played games and replay them ("Theater").
- Add code for generating and sending minidumps for debugging purposes. This feature is meant to be used only during the Open Beta and will be removed once the code goes to stable release.
- Add commands that allow forwarding console and games log via UDP to other computers ("network logging").
- Add D3D9Ex.
- Add filters for server list.
- Add handling for `iw4x://` URLs ("connect protocol"). For example, if IW4x is properly registered in Windows as URL handler for `iw4x://` URLs you can type `iw4x://ip:port`. If possible, this will connect to the server in an already running IW4x client.
- Add lean support through new key bindings.
- Add native cursor as replacement for the sometimes laggy in-game cursor. This change can be reverted in the settings menu.
- Add news ticker.
- Add remote console ("RCon").
- Add support for BigBrotherBot.
- Add support for hosting game mods in order to allow players to just join modded servers out of the box ("mod download").
- Add Warfare2 text coloring.
- Add zone builder. For more information see the respective documentation.
- Implement a completely decentralized peering network.
- Implement playlists which can be used for flexible map and gametype rotation.
- Introduce security levels. This ensures that you need to "pay" with CPU power to verify your identity once before being able to join a server which reduces the interval at which people who get banned can circumvent server bans through using new identities. The default security level is 23.
- Move IW4x resource files into their own folder to prevent clogging up the main game directories.
- Reintroduce parties, now also available for dedicated servers ("lobby servers").

### Changed

- Move logs to `userraw` folder.
- Replace main menu background music.
