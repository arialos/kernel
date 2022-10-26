<div align="center">
 <img src="https://raw.githubusercontent.com/arialos/.github/21b0dc9320f0fb7667987e72704531c94762cde4/resources/arial.png" width="200px">
 <h1>The Arial Operating System.</h1>
 <p align="center">
     <a href="https://spiky-morocco-d93.notion.site/Arial-Development-4bc9666c1b3741198fc7c4b11ca0fc61">
         Notion
     </a>
     - 
     <a href="https://www.figma.com/file/vBQiMkgFKpPxikPEEjg7AL/Arial-Design?node-id=0%3A1">
         Figma 
     </a>
     - 
     <a href="https://github.com/orgs/arialos/projects/1">
         Github Projects 
     </a>
 </p>
 <p>A homage to `90s User interface design, combined with modern development. Arial is centered around creating an operating system that is simple, beautiful and accessible.</p>
 </div>
 
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
