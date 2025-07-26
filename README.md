NXFixCheat
==========

NXFixCheat is a simple Nintendo Switch homebrew utility that allows users to easily manage Atmosphère's cheat behavior without editing config files manually.

This app was created to fix and toggle two common cheat-related settings in system_settings.ini:

- ✅ dmnt_cheats_enabled_by_default — whether cheats are auto-enabled on game launch
- ✅ dmnt_always_save_cheat_toggles — whether each cheat's ON/OFF state is remembered between sessions

Features
--------

- Toggle Auto-Cheat (enable/disable)
- Toggle Remember Cheat State (save per-cheat toggle)
- Detects current config and shows status with [✓] marks
- Simple text-based UI with D-Pad navigation
- Confirmation prompts before applying any change
- Designed for use in Atmosphère homebrew environment

Screenshot
----------

NXFixCheat Utility
========================
> [✓] Disable Auto-Cheat
> 
> [ ] Enable Auto-Cheat
> 
> [✓] Remember Cheat State
> 
> Exit without changes

Use ↑ ↓ to move, A to select

Installation
------------

1. Build using devkitPro or download the .nro release
2. Copy nxfixcheat.nro to your Switch SD card under /switch/nxfixcheat/
3. Launch from the Homebrew Menu (hbmenu)

Building From Source
--------------------

Requirements:
- devkitPro with devkitA64, libnx, switch-tools
- make, gcc, etc.

Build:

    make clean
    make

The result will be:

    nxfixcheat.nro

How It Works
------------

The app checks for /atmosphere/config/system_settings.ini.

- If it doesn't exist, a default template will be created
- The app scans for the lines:
  - dmnt_cheats_enabled_by_default
  - dmnt_always_save_cheat_toggles
- Missing lines will be added
- User can apply changes via menu

Example Config
--------------

    [atmosphere]
    dmnt_cheats_enabled_by_default = u8!0x1
    dmnt_always_save_cheat_toggles = u8!0x1

Credits
-------

- Built using devkitPro
- Developed by jiraiya78
- Special thanks to the switchbrew and libnx community

Disclaimer
----------

This is a homebrew utility meant for use on modded Nintendo Switch consoles.
Use responsibly. No warranties are provided. Atmosphère must be installed.
