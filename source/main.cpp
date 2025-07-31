#include <switch.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <switch/services/bpc.h>
#include <switch/types.h>

#define CONFIG_PATH "sdmc:/atmosphere/config/system_settings.ini"
#define TEMPLATE_PATH "sdmc:/switch/nxfixcheat/system_settings_template.ini"
#define BACKUP_PATH "sdmc:/atmosphere/config/system_settings_backup.ini"

void pauseit(u64 nanoseconds = 250000000ULL) {
    svcSleepThread(nanoseconds);
}

enum CheatStatus {
    STATUS_UNKNOWN,
    STATUS_ENABLED,
    STATUS_DISABLED
};

// Read lines from a file
std::vector<std::string> readLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Write lines to file
void writeLines(const std::string& path, const std::vector<std::string>& lines) {
    std::ofstream file(path, std::ios::trunc);
    for (const auto& line : lines) {
        file << line << "\n";
    }
}

// Copy file (binary)
bool copyFile(const std::string& src, const std::string& dst) {
    std::ifstream in(src, std::ios::binary);
    std::ofstream out(dst, std::ios::binary);
    if (!in || !out) return false;
    out << in.rdbuf();
    return true;
}

// Detect current cheat status
CheatStatus getCheatStatus() {
    auto lines = readLines(CONFIG_PATH);
    for (auto& line : lines) {
        if (line.find("dmnt_cheats_enabled_by_default") != std::string::npos) {
            if (line.find("0x1") != std::string::npos)
                return STATUS_ENABLED;
            else if (line.find("0x0") != std::string::npos)
                return STATUS_DISABLED;
        }
    }
    return STATUS_UNKNOWN;
}

CheatStatus getCheatMemoryStatus() {
    auto lines = readLines(CONFIG_PATH);
    for (auto& line : lines) {
        if (line.find("dmnt_always_save_cheat_toggles") != std::string::npos) {
            if (line.find("0x1") != std::string::npos)
                return STATUS_ENABLED;
            else if (line.find("0x0") != std::string::npos)
                return STATUS_DISABLED;
        }
    }
    return STATUS_UNKNOWN;
}


// Apply new setting
bool updateCheatSetting(bool enable) {
    FILE* fp = fopen(CONFIG_PATH, "r");
    if (!fp) {
        printf("[!] Config not found. Attempting to copy template...\n");
        return copyFile(TEMPLATE_PATH, CONFIG_PATH);
    }
    fclose(fp);

    printf("[*] Backing up config...\n");
    copyFile(CONFIG_PATH, BACKUP_PATH);

    auto lines = readLines(CONFIG_PATH);
    bool found = false;

    for (auto& line : lines) {
        if (line.find("dmnt_cheats_enabled_by_default") != std::string::npos) {
            line = std::string("dmnt_cheats_enabled_by_default = u8!") + (enable ? "0x1" : "0x0");
            found = true;
            break;
        }
    }

    if (!found) {
        lines.push_back(std::string("dmnt_cheats_enabled_by_default = u8!") + (enable ? "0x1" : "0x0"));
    }

    writeLines(CONFIG_PATH, lines);
    return true;
}

bool updateCheatMemorySetting(bool enable) {
    auto lines = readLines(CONFIG_PATH);
    bool found = false;

    for (auto& line : lines) {
        if (line.find("dmnt_always_save_cheat_toggles") != std::string::npos) {
            line = std::string("dmnt_always_save_cheat_toggles = u8!") + (enable ? "0x1" : "0x0");
            found = true;
            break;
        }
    }

    if (!found) {
        lines.push_back(std::string("dmnt_always_save_cheat_toggles = u8!") + (enable ? "0x1" : "0x0"));
    }

    writeLines(CONFIG_PATH, lines);
    return true;
}


void drawHeader() {
    printf("\x1b[2;1H");
    printf("\x1b[44;37m=====================================================\n");
	printf("                                                     \n");
    printf("              N X  F I X C H E A T   V1.0            \n");
	printf("                                                     \n");
	printf("        https://github.com/jiraiya78/nxfixcheat      \n");
	printf("                                                     \n");	
    printf("=====================================================\x1b[0m\n\n");
}

void showStatus(CheatStatus status) {
    printf("Cheat Auto-Enable         : ");
    switch (status) {
        case STATUS_ENABLED:
            printf("\x1b[31mENABLED\x1b[0m\n"); // Red
            break;
        case STATUS_DISABLED:
            printf("\x1b[32mDISABLED\x1b[0m\n"); // Green
            break;
        default:
            printf("\x1b[33mUNKNOWN\x1b[0m\n"); // Yellow
            break;
    }
	
}

void showMemoryStatus(CheatStatus status) {
	printf("Cheat Remember State      : ");
    switch (status) {
        case STATUS_ENABLED:
            printf("\x1b[32mENABLED\x1b[0m\n"); // Green
            break;
        case STATUS_DISABLED:
            printf("\x1b[31mDISABLED\x1b[0m\n"); // Yellow
            break;
        default:
            printf("\x1b[33mUNKNOWN\x1b[0m\n"); // Yellow
            break;
    }
}

void showPostApplyMenu() {
	
	PadState pad;
    padInitializeDefault(&pad);
	
    while (appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) {
            consoleExit(NULL);
            spsmInitialize();
            spsmShutdown(true);  // Reboot to HBMenu
            spsmExit();
            break;  // Will not return
        }

        if (kDown & HidNpadButton_B) {
			break;
		 // Go back to main menu
		}
    }
}

void waitForButtonRelease(PadState* pad) {
    while (appletMainLoop()) {
        padUpdate(pad);
        u64 kDown = padGetButtonsDown(pad);
        if (kDown == 0) break;
        consoleUpdate(NULL);
    }
}

void rebootNormal() {
	consoleClear();
	drawHeader();
	printf("\n\nRebooting to payload (if you have one) in 10 secs...");
	printf("\n\nIf you experience black screen, try to press volume - or +.");
	consoleUpdate(NULL);
	svcSleepThread(10000000000ULL); // optional delay
	consoleExit(NULL);
	spsmInitialize();
	spsmShutdown(true);  // Reboot to HBMenu
	spsmExit();	
}

int runMainMenu() {
	
	PadState pad;
    padInitializeDefault(&pad);
	
	const int MENU_COUNT = 4;  // total number of menu options

	enum MenuOption {
		TOGGLE_AUTO_CHEAT,
		TOGGLE_CHEAT_STATE,
		EXIT,
		REBOOT
	};

    int selected = 0;
    bool settingApplied = false;

    //CheatStatus current = getCheatStatus();
	CheatStatus cheatStatus = getCheatStatus();
	CheatStatus memoryStatus = getCheatMemoryStatus();

    while (appletMainLoop() && !settingApplied) {
        consoleClear();
        drawHeader();
        showStatus(getCheatStatus());
        showMemoryStatus(getCheatMemoryStatus());

		// PRINT MENU
		// =================================================
        printf("\n\n\nUse UP and  DOWN to navigate, A to select:\n\n");

		printf("%s [%s] Toggle Auto-Cheat\n", 
			   (selected == TOGGLE_AUTO_CHEAT ? "\x1b[44;37m-->\x1b[0m" : "   "),
			   cheatStatus == STATUS_ENABLED ? "\x1b[31m ENABLED  \x1b[0m" : "\x1b[32m DISABLED \x1b[0m");

		printf("%s [%s] Toggle Remember Cheat State\n\n",
			   (selected == TOGGLE_CHEAT_STATE ? "\x1b[44;37m-->\x1b[0m" : "   "),
			   memoryStatus == STATUS_ENABLED ? " ENABLED  " : " DISABLED ");

		printf("%s Press B to exit without reboot\n", selected == EXIT ? "\x1b[44;37m-->\x1b[0m" : "   ");

		printf("%s Press + to reboot\n", selected == REBOOT ? "\x1b[44;37m-->\x1b[0m" : "   ");
		
        consoleUpdate(NULL); //update the screen & print the menu

        // Handle input
        // =================================================
		padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

		// Button UP
        // =================================================
		
        if (kDown & HidNpadButton_Up) {
            selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
        }
		
		// Button DOWN
        // =================================================
        if (kDown & HidNpadButton_Down) {
            selected = (selected + 1) % MENU_COUNT;
        }

		// Button A
        // =================================================
		if (kDown & HidNpadButton_A) {
			
			consoleClear(); //clear screen for next screen
			drawHeader();
			consoleUpdate(NULL);
			
			// Select 'Toogle Cheat Remember'
			// =================================================
			if (selected == TOGGLE_CHEAT_STATE) {
				bool targetState = memoryStatus == STATUS_DISABLED;

				printf("Toggle Remember Cheat State: %s?\n", targetState ? "ON" : "OFF");
				printf("\nA = Confirm, \nB = Cancel\n");

				consoleUpdate(NULL);
				bool confirm = false;

				while (appletMainLoop()) {
					padUpdate(&pad);
					u64 confirmDown = padGetButtonsDown(&pad);
					if (confirmDown & HidNpadButton_A) {
						confirm = true;
						break;
					}
					if (confirmDown & HidNpadButton_B) break;
					consoleUpdate(NULL);
				}

				if (confirm) {
					consoleClear();
					drawHeader();
					printf("[+] Toggle Remember Cheat State to %s...\n", targetState ? "ON" : "OFF");
					pauseit();
					updateCheatMemorySetting(targetState);
					memoryStatus = getCheatMemoryStatus();
					printf("[OK] Updated successfully.\n");
					pauseit();
					settingApplied = true;

				}
			}
			
			// Select 'Toogle Cheat Auto-Enable'
			// =================================================
			if (selected == TOGGLE_AUTO_CHEAT) {
				bool targetState = cheatStatus == STATUS_DISABLED;
				printf("Are you sure you want to %s auto-cheats?\n", targetState ? "ENABLE" : "DISABLE");
				printf("\nA = Confirm, \nB = Cancel\n");

				consoleUpdate(NULL);
				bool confirm = false;

				while (appletMainLoop()) {
					padUpdate(&pad);
					u64 confirmDown = padGetButtonsDown(&pad);
					if (confirmDown & HidNpadButton_A) {
						confirm = true;
						break;
					}
					if (confirmDown & HidNpadButton_B) break;
					consoleUpdate(NULL);
				}

				if (confirm) {
					consoleClear();
					drawHeader();
					printf("[+] Setting Auto-Cheat: %s...\n", targetState ? "ENABLED" : "DISABLED");
					pauseit();
					updateCheatSetting(targetState);
					cheatStatus = getCheatStatus();
					printf("[OK] Updated successfully.\n");
					pauseit();
					settingApplied = true;
				}
			}
			
			// Select 'Exit'
			// =================================================
 			if (selected == EXIT) {
				//consoleExit(NULL);
				return -1;
				break;
				} 
				
			// Select 'Reboot'
			// =================================================
			if (selected == REBOOT) {
				rebootNormal();
				break;  // Will not return	
				
			}
		} //end if Button A
		
		// Keypress 'B'
		// =================================================
        if (kDown & HidNpadButton_B) {
			//consoleExit(NULL);
			return -1;
			break;
		}
		
		// Keypress '+'
		// =================================================		
        if (kDown & HidNpadButton_Plus) {
				rebootNormal();
				break;
        }
		
		
    }//end while
	return 0;
}//end function

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

	while (true) {
		runMainMenu();
		int result = runMainMenu();
		if (result == -1) {
			break; // Exit main loop and proceed to consoleExit()
		}

	}
	
    consoleExit(NULL);
    return 0;
}

