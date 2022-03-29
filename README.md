# 💾 arial.os - A x86 Hobby Operating System
A simple x86 hobby OS built in C. Designed to help myself feel more conformable with working in lower level software development, the end goal is to get into userspace and create a semi good looking widget kit.

## 🎨 Project Design
Overview on how I plan to build out this project and how parts of the codebase will be designed.
### Kernel
arial.os's kernel is designed to be partial-monolithic, a lot similar to the Linux or macOS kernels. This will give flexibilities to create new kernel modules and drivers and will reduce the overhead between userspace and kernel level calls.

### Racks, Receivers and Kits
To achieve this hybrid model, arial.os's design follows three principles 

1. Optional parts of the kernel, ie Wifi Drivers are refereed to as Kits. Kits can be removed or added by the kernel at anytime and are focused around being plug and play
2. Permanent parts of the kernel, ie VBE graphics are refereed to as Racks. Racks are meant to be fixed both in memory and in state and easily accessible by Receivers with uninterrupted communication between the two.
3. When userspace needs to get information to or from a part of the kernel it can use a Receiver. Receivers are two-way tunnels controlled by the kernel, userspace can request to open a Receivers and send information but the kernel has full control over the creation, deletion and permissions this way ensuring that malicious code can't take over the kernel. 

## 🗿 Milestones
- [x] Setup toolchain for macOS
- [x] VBE Graphics rendering
    - [x] `printf` function to get better debug information
    - [x] Switch existing Debug and Info functions to VBE over VGA
    - [ ] Allow easy switching of printing function from VGA to VBE and vice-versa
    - [x] Have a pretty logo in the center of the screen
- [ ] GDT, irqs and ircs
    - [ ] Get hardware interrupts
    - [ ] Map software interrupts
    - [ ] Simple Keyboard driver
    - [ ] PS/2 Mouse Driver

## 📸 Screenshot of last milestone
![Screenshot of arial.os](https://media.discordapp.net/attachments/871682522629677109/958344064128909352/Screen_Shot_2022-03-30_at_1.08.56_AM.png?width=1334&height=1066)

<!-- ## 🎨 Formatting Guide
### Functions
Function naming should follow camelCase, variables should have space between brackets and curly brackets must be on a new line.

```c
// This is the correct way to format a function
void main ( void )
{
    initTerminal();
}
```

### Variables -->
