# 💾 arial.os - A x86 Hobby Operating System
A simple x86 hobby OS built in C. Designed to help myself feel more conformable with working in lower level software development, the end goal is to get into userspace and create a semi good looking widget kit.
## Milestones
- [x] Setup toolchain for macOS
- [x] VBE Graphics rendering
    - [x] `printf` function to get better debug information
    - [x] Switch existing Debug and Info functions to VBE over VGA
    - [ ] Allow easy switching of printing function from VGA to VBE and vice-versa
    - [x] Have a pretty logo in the center of the screen
- [ ] GDT, IDT and irqs
    - [ ] Get hardware interrupts
    - [ ] Map software interrupts
    - [ ] Simple Keyboard driver
    - [ ] PS/2 Mouse Driver

## Screenshots
<p align="center">
<img src="https://media.discordapp.net/attachments/871682522629677109/958658363552575508/Screen_Shot_2022-03-30_at_10.24.03_PM.png?width=1404&height=1065" />
<br>
arial.os build: <a href="https://github.com/AsteroidsGithub/arial/commit/554dc66df5dd2899af4e94a24aa7a7a086b5361d">554dc66</a> running on QEMU 6.2

## Project Design
Overview on how I plan to build out this project and how parts of the codebase will be designed.

</p>