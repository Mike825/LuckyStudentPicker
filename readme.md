<!--========================================================================
    LuckyStudentPicker Project Overview
========================================================================

This project demonstrates how to get started writing WinUI3 apps directly
with standard C++, using the Windows App SDK and C++/WinRT packages and
XAML compiler support to generate implementation headers from interface
(IDL) files. These headers can then be used to implement the local
Windows Runtime classes referenced in the app's XAML pages.

Steps:
1. Create an interface (IDL) file to define any local Windows Runtime
    classes referenced in the app's XAML pages.
2. Build the project once to generate implementation templates under
    the "Generated Files" folder, as well as skeleton class definitions
    under "Generated Files\sources".
3. Use the skeleton class definitions for reference to implement your
    Windows Runtime classes.

========================================================================
Learn more about Windows App SDK here:
https://docs.microsoft.com/windows/apps/windows-app-sdk/
Learn more about WinUI3 here:
https://docs.microsoft.com/windows/apps/winui/winui3/
Learn more about C++/WinRT here:
http://aka.ms/cppwinrt/
========================================================================-->

# 🎲 Lucky Student Picker

## 🗯️ Overview

This program is written by Mike825 using WinUI3 and C++/WinRT. It saves you from worrying about how to pick a student from the class when you want to ask a question. It will randomly pick a student for you and display the name on the screen. You can also add or remove students from the list. 

## 💡 Features

- Randomly pick a student from a configurable JSON file.

- Control whether to read the student's name out or to allow the same student to be picked multiple times. 

- Edit the text displayed on the screen when a student is picked or lock it. 

- Read the program's running history in log files. 

- Toggle the window's behavior between normal mode and compact overlay mode. 

- Use the build-in seat arranger to arrange seat for your students (Beta). 

## 💾 Installation process

### System prerequisites

- Windows 10.0.19045 (Windows 10 Version 22H2) or above. 

- Windows App Runtime SDK 1.7 or above (download [here](https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/downloads)).

### Installing this app

- Download the .msix and .pfx file from [here](https://github.com/Mike825/LuckyStudentPicker/releases)

> Note: Because the .msix file is signed with a self-signed certificate, you will need to install the .pfx file before installing the .msix file. This certificate expires annually, so each year I'll publish a new one. 

- Install the certificate: 
    
    1. Double click the .pfx file, and select the "Local Machine" store location. 

    2. Click "Next" and permit all actions, until you see the "Private key protection" page. 

    3. Enter the password `20260314LuckyStu` and then click "Next" until you finish the installation process. 

- Install the app: Double click the .msix file, and click "Install" to install the app.

### Updating this app

- Check if there is a new release on the [GitHub releases page](https://github.com/Mike825/LuckyStudentPicker/releases). 

- If so, download the new .msix file, and repeat the installation process. 

- You don't need to install the .pfx file again unless you see a new .pfx file in the release, which means the certificate has been updated.

## 🤯 Learn more

### Frameworks
  - [Windows App SDK](https://docs.microsoft.com/windows/apps/windows-app-sdk/)
  - [WinUI3](https://docs.microsoft.com/windows/apps/winui/winui3/)
  - [C++/WinRT](http://aka.ms/cppwinrt/)

### Coding environment
  - [Visual Studio](https://visualstudio.microsoft.com/vs/)
  - [C++ development workload](https://docs.microsoft.com/windows/apps/windows-app-sdk/set-up-your-development-environment#install-visual-studio-with-the-c-development-workload)

### License
  - [GNU General Public License v3.0 (GPL-3.0)](https://www.gnu.org/licenses/gpl-3.0.en.html)