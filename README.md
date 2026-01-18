# AUM GPS System

A C++ console-based program that simulates a GPS system for the **Applied Science University (AUM)** campus.  
It allows **Admin and User modes** to manage and navigate the campus locations, including Buildings, Floors, and Rooms.

---

## Features

### Admin Features:
- Add Buildings, Floors, and Rooms dynamically.
- Open or close locations (affects child nodes recursively).
- View the entire campus map in a tree structure.
- Save and load campus data to/from a file.

### User Features:
- View campus map with open/closed status.
- Search for a specific room and view its location path.
- Find paths between any two locations in the campus.

### Additional Features:
- Tree-based campus data structure for efficient management.
- Ensures hierarchical rules: Rooms → Floors → Buildings → University.
- Persistent storage via text files.
- Professional CLI experience with input validation.

---

## Installation & Compilation

1. Make sure you have a **C++ compiler** (g++, clang, or Visual Studio).
2. Download or clone the repository:

```bash
git clone https://github.com/YOUR_USERNAME/AUM_GPS_System.git
cd AUM_GPS_System
 
 3. Compile the program:
 g++ AUMGps.cpp -o AUMGps

4.Run the program:
./AUMGps    # On Linux/Mac
AUMGps.exe  # On Windows

Usage:

Launch the program.

Select Admin or User mode:

Admin Mode: Requires password (JOTM by default).

User Mode: Explore the campus, search rooms, or find paths.

Follow the on-screen prompts to add, open/close, or navigate locations.

All changes are automatically saved to AUM_GPS_Data.txt.

Example Map Output

Welcome to AUM GPS System

1. Admin
2. User
3. Exit
Choice: 2

--- User Menu ---
1. View Map
2. Search Room
3. Find Path
4. Back
5. Exit
Choice: 1
- AUM ( UNIVERSITY - Open )
  - SA ( BUILDING - Open )
    - GROUND ( FLOOR - Open )
      - G-11 ( ROOM - Open )
      - G-12 ( ROOM - Open )
      - G-17 ( ROOM - Open )
    - FIRST ( FLOOR - Open )
      - F-11 ( ROOM - Open )
      - F-15 ( ROOM - Open )
  - SB ( BUILDING - Open )
    - GROUND ( FLOOR - Open )
      - SB-G09 ( ROOM - Open )
      - SB-G03 ( ROOM - Open )
    - FIRST ( FLOOR - Open )
      - SB-F07 ( ROOM - Open )
      - SB-F12 ( ROOM - Open )
  - BA ( BUILDING - Open )
    - GROUND ( FLOOR - Open )
      - G-15 ( ROOM - Open )
      - G-06 ( ROOM - Open )
      - G-07 ( ROOM - Open )
  - STC ( BUILDING - Open )

AUMGps.cpp         # Main C++ program
README.md          # Project documentation
.gitignore         # Ignore build/executable files
AUM_GPS_Data.txt   # Auto-generated saved campus data

Author:
Tamer Alomari

GitHub: https://github.com/TamerAlomari

License:
This project is licensed under the MIT License. See the LICENSE
 file for details.
 
 Future Enhancements:

Add a GUI interface for easier visualization.

Implement multi-user authentication for Admin mode.

Allow importing/exporting map data in JSON or CSV format.

Add real-time availability updates for rooms.
