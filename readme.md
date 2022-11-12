# Fuck MS Teams
Microsoft Teams hijacks the play/pause media key after playing the ringtone.
This project prevents Teams from registering for the play/pause media key with Windows and ruining everyone's day.

## Installation
Build the library using:
```
cmake -B build
cmake --build build --config Release
```

Register the library as AppInit DLL in your registry:
* Set `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\RequireSignedAppInit_DLLs = 0` (dword)
* Set `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\LoadAppInit_DLLs = 1` (dword)
* Add `<PROJECT_DIR>\build\Release\fuck-ms-teams.dll` to `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\AppInit_DLLs` (string)

## FAQ
### How does it work?
Windows provides a mechanism called «AppInit DLLs» for injecting custom code into every process.
Using this mechanism and the amazing `MinHook` library we can inject code into Teams.exe that patches the `RegisterHotKey` function.

### Is it safe?
Most likely not. Use at your own risk.
