# 💾 arial - A x86 Hobby Operating System
arial is hobbyist operating system developed in my spare time in-between school and work. It's 32 bit and targets Intel's i686 platform, with a philosophy of creating a system that is simple to use and understand both from a user and developer perspective. The current project goal is to reach a userland and have a basic window manager.
## Milestone 1 Todo
Whilst Milestone 1 is far away this serves a nice goal post before i tackle more complex problems like paging and threading for milestone 2.
- [x] Refine toolchain for compatibility on Linux.
- [ ] Graphics rendering using VBE.
    - [x] `printf` function to get better debug information.
    - [x] Support to draw different shapes. Squares, circles, individual pixels and text.
    - [x] Render a basic version of arial's logo using graphics library.
    - [ ] Draw a color image.
    - [ ] Create a text mode fallback.incase VBE doesn't start properly.
- [x] GDT, IDT and Interrupts.
    - [x] Create and flush GDT and IDT tables
    - [x] Get hardware interrupts.
    - [x] Map software interrupts.
- [x] Simple PS/2 driver for keyboard and mouse.
    - [x] Get interrupts 1 and 12 for activity on input devices.
    - [x] Spawn relevant events for each device after their interrupts.
    - [x] Get basic keyboard driver working and draw characters to screen.
    - [x] Get basic mouse driver working and draw cursor to screen.
    - [x] Handle clicks / modifier keys and store that information in event
    - [x] Key stroke buffer for keyboard
- [ ] General house keeping 
    - [ ] Get a system timer created. Create a clock from interrupt 33 or something?
    - [x] Create function to find the SMBios.
    - [ ] Use SMBios to get information about cpu, bios and manufacture of machine.
    - [ ] Pull current world Date/Time from CMOS. 

## Screenshots
<p align="center">
<img src="https://cdn.discordapp.com/attachments/811364311795630130/1028977281948983316/Screen_Shot_2022-10-10_at_23.26.06.png" />
<br>
arial build: <a href="https://github.com/AsteroidsGithub/arial/commit/ebf615df6e6f2d11cf17a63ac441a6b11c27570b">ebf615d</a> running on QEMU 6.2
