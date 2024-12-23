# NetworkRightsChecker

*Windows Only*

This tool will mount a network drive to the computer and tests the access rights on the directories. Therefore it tries to read the directory listing and tries to create a file in the directories. As output there is a HTML file, providing all information.

The NetworkRightsChecker will always use the config.ini from the executing dirctory. 

## Example Config

```
[general]
; Subdirectories to check
level=4
; Drive Name
drive=Y

[servers]
server1=\\192.168.0.100\media

[users]
username1=password1
username2=password2
```
