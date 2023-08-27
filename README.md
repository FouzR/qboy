rMBoy
====

A Gameboy Emulator in C++ with Qt
(TBD: porting to rM)

Dependencies
------------
- C++
- Qt 4 or 5

Features
--------

- Emulates the monochrome gameboy
- CPU that passes the cpu_instrs rom
- GPU
- Keypad
- Timer
- MMU
  - MBC0
  - MBC1
  - MBC3 with realtime-clock
  - MBC5
  - Support for savegames

TODO
----

- [ ] Render the game screen
- [ ] Have buttons on screen(D-Pad, A,B, Start, Select)
- [ ] Forced refresh button
- [ ] Additional buttons as required for emulator
- [ ] [Extra] Add GB front plate as a theme

Special thanks to
-----------------

* http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-The-CPU
* http://nocash.emubase.de/pandocs.htm
