# Windows Notifications Reposition
Reposition the system notifications on Windows 10/11

## Build
- Clone or fork this repository
- Open `win-notif-reposition.sln` with Visual Studio and build the solution
- **OR** compile `project/main.cpp` using your favorite compiler =))) 

## Usage
- Run the program using the `win-notif-reposition.exe` executable file
- Add a shortcut to it into `%appdata%\Microsoft\Windows\Start Menu\Programs\Startup` if you want it to automatically run on startup
- Run the application with command-line arguments to customize the position of notifications:
```cmd
> win-notif-reposition.exe [-help] [-position=$p] [-hmargin=$h] [-vmargin=$v]
```
### Argument list
|   Argument  |                                    Description                                    |
|-------------|-----------------------------------------------------------------------------------|
| -position   | Set window position (0: top-left, 1: top-right, 2: bottom-left, 3: bottom-right)  |
| -hmargin    | Set horizontal margin (range: 50-200 px)                                          |
| -vmargin    | Set vertical margin (range: 50-200 px)                                            |
| -help or /? | Display the help message                                                          | 

## Credit:
Based on [vanlocvo/top-right-noti](https://github.com/vanlocvo/top-right-noti)
