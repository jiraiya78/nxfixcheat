# NXFixCheat

A simple Nintendo Switch homebrew tool for easily toggling Atmosphere cheat settings, including auto-enable cheats and remembering cheat states.  
**NXFixCheat** provides a user-friendly graphical interface to quickly enable or disable cheat options without editing config files manually.

---

## Features

- **Toggle auto-enable cheats:** Quickly enable or disable default cheat activation (`dmnt_cheats_enabled_by_default`) in `system_settings.ini`.
- **Toggle remember cheat state:** Set whether Atmosphere should remember the last cheat toggle state (`dmnt_always_save_cheat_toggles`).
- **Automatic backup:** Backs up your current config file before applying any changes.
- **Template restoration:** If your config is missing, a template is automatically copied and used.
- **Simple menu navigation:** Navigate using your controller’s D-Pad and buttons.
- **Reboot option:** Optionally reboot to Homebrew Menu after applying changes.

---

## How It Works

NXFixCheat modifies the following file on your SD card:

- `sdmc:/atmosphere/config/system_settings.ini`

If the file is missing, a template is copied from:

- `sdmc:/switch/nxfixcheat/system_settings_template.ini`

A backup of your current config is saved to:

- `sdmc:/atmosphere/config/system_settings_backup.ini`

---

## Usage

1. **Copy the NRO**  
   Place the compiled `.nro` file in your `sdmc:/switch/nxfixcheat/` folder.

2. **(Optional) Template Config**  
   Ensure `system_settings_template.ini` exists in `sdmc:/switch/nxfixcheat/` for recovery if your config file is missing.

3. **Run NXFixCheat**  
   Launch the application via the Homebrew Menu.

4. **Navigate the menu**  
   - Use ⬆️/⬇️ to move between options.
   - Press **A** to select/toggle.
   - Press **B** to exit.
   - Press **+** to reboot to HBMenu.

---

## Building

Requires [devkitPro](https://devkitpro.org/) and the Switch homebrew libraries.

Typical build instructions:

```bash
make
```

Copy the resulting `nxfixcheat.nro` to your Switch SD card.

---

## Menu Overview

- **Toggle Auto-Cheat**  
  Enable or disable automatic cheat activation for all games.
- **Toggle Remember Cheat State**  
  Decide if Atmosphere should save and restore last cheat states.
- **Exit**  
  Close the app without rebooting.
- **Reboot**  
  Reboot to payload or Homebrew Menu for settings to take effect.

---

## Screenshots

![screenshot](https://github.com/jiraiya78/nxfixcheat/blob/main/screenshot.jpg)

---

## Disclaimer

- Use at your own risk.
- Modifying system files can cause unexpected behavior.
- Always keep backups of your configuration files.

---

## License

MIT License  
(C) 2024 [jiraiya78](https://github.com/jiraiya78)

---

## Links

- [Atmosphere CFW](https://github.com/Atmosphere-NX/Atmosphere)
- [devkitPro](https://devkitpro.org/)
- [SwitchBrew Wiki](https://switchbrew.org/)

---
