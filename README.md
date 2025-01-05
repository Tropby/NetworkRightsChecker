# Network Rights Checker

**WINDOWS ONLY**

This tool connects to a network drive and checks the read/write rights for directories in that network drive. The output is written to an output.html file.

**Please note:** The program uses "net use" commands to connect to the network drives. In windown only 1 username could be used at a time to connect to one server. If you have mouned another drive with the same server. This tool will not work as intended.

## Configuration

The executable loads a "config.ini" expecting the following values:

```INI
[general]
drive=Y
level=5
```

drive => Drive letter for the network drive
level => Direcory levels to scan

```INI
[servers]
server1=\\192.168.0.100\s1\
server2=\\192.168.0.100\s2\
```

servers => Contains at least 1 server UNC path

```INI
[users]
tropby=test12345
```

users => Contains at least 1 username with that the tool tries to connect to the network drive and tests the read/write rights
