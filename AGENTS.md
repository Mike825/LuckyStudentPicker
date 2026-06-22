# Lucky Student Picker — Agent Guide

This file is written for AI coding agents working on the **Lucky Student Picker** project. The project is a Windows desktop application written in **C++/WinRT** using **WinUI 3** and the **Windows App SDK**. All instructions and comments found in the source code are in English, while the user-facing UI text is partly in Simplified Chinese.

## Project Overview

**Lucky Student Picker** is a single-project MSIX-packaged WinUI 3 desktop application that randomly selects a student name from a configurable JSON namelist. It also supports text-to-speech, on-screen text editing, logging, compact overlay mode, and a beta seat-arranger utility.

- **Display name:** Lucky Student Picker
- **Current version:** 1.4.2.0 (as declared in `Package.appxmanifest` and `AboutPage.xaml`)
- **Build date:** 2026.6.7 (per `AboutPage.xaml`)
- **Author:** Mike825 / lsr
- **License:** GNU General Public License v3.0 (`LICENSE`)
- **Repository:** https://github.com/Mike825/LuckyStudentPicker

## Technology Stack

| Layer | Technology |
|-------|------------|
| Language | C++ (standard: C++23) |
| UI framework | WinUI 3 / Windows App SDK 2.1.3 |
| Windows Runtime projection | C++/WinRT 3.0.260520.1 |
| Build system | MSBuild (Visual Studio 2022 v143 toolset) |
| Package format | MSIX (single-project packaging, `EnableMsixTooling=true`) |
| JSON parsing | nlohmann.json 3.12.0 |
| Helpers | WIL (Windows Implementation Library), WebView2, Microsoft.Windows.AI.MachineLearning |

### Target / minimum Windows versions

- Target platform version: `10.0`
- Minimum platform version: `10.0.17763.0`
- Default app language: `zh-CN`
- Supported platforms: x86, x64, ARM64

## Project Structure

The repository keeps all source files in the project root. Each UI component is represented by up to four files:

- `.idl` — C++/WinRT interface definition (runtime class)
- `.xaml` — XAML UI markup
- `.xaml.h` — generated-aware C++ header
- `.xaml.cpp` — implementation

### Core application files

| File | Purpose |
|------|---------|
| `LuckyStudentPicker.sln` | Visual Studio solution |
| `LuckyStudentPicker.vcxproj` | Main MSBuild project |
| `LuckyStudentPicker.vcxproj.filters` | Solution Explorer filters |
| `packages.config` | NuGet package references (legacy `packages/` layout) |
| `Package.appxmanifest` | MSIX manifest, identity, capabilities, visual assets |
| `app.manifest` | Win32 application manifest declaring Windows 10 compatibility and PerMonitorV2 DPI awareness |
| `pch.h` / `pch.cpp` | Precompiled header (required by all `.cpp` files) |
| `readme.md` | User-facing README |

### UI components

| Component | XAML | IDL | Description |
|-----------|------|-----|-------------|
| `App` | `App.xaml` | inline | Application object, launches `MainWindow` |
| `MainWindow` | `MainWindow.xaml` | `MainWindow.idl` | Main picker window and core logic |
| `EditTextDialog` | `EditTextDialog.xaml` | `EditTextDialog.idl` | ContentDialog to edit the picked text |
| `SettingsWindow` | `SettingsWindow.xaml` | `SettingsWindow.idl` | Settings window with NavigationView |
| `SettingsHomePage` | `SettingsHomePage.xaml` | `SettingsHomePage.idl` | Placeholder settings home page |
| `AboutPage` | `AboutPage.xaml` | `AboutPage.idl` | Version, shortcuts, data-folder/namelist actions |
| `SeatArrangerWindow` | `SeatArrangerWindow.xaml` | `SeatArrangerWindow.idl` | Beta seat-arranger utility |
| `SeatArrangerErrorPrompt` | `SeatArrangerErrorPrompt.xaml` | `SeatArrangerErrorPrompt.idl` | Error dialog for seat arranger |
| `NamelistTroubleshootingWindow` | `NamelistTroubleshootingWindow.xaml` | `NamelistTroubleshootingWindow.idl` | Diagnostic window for broken namelist |

### Helper classes

| File | Purpose |
|------|---------|
| `NamelistHelper.h` / `.cpp` | Singleton that creates/reads `%LocalAppData%\Packages\<pkg>\LocalState\LuckyStudentPicker\UnluckyStudentsNamelist.json` |
| `LoggingHelper.h` / `.cpp` | Singleton that writes timestamped logs to `...\LuckyStudentPicker\UnluckyStudentsLog.log` |

### Generated files

- `Generated Files/` contains XAML compiler output (`*.g.cpp`, `*.g.h`, `*.xbf`) and C++/WinRT generated sources (`module.g.cpp`).
- `Generated Files/sources/` may contain skeleton class implementations after the first build.
- These directories are generated and should not be hand-edited.

### Assets

`Assets/` contains all MSIX visual assets (tiles, splash screen, store logo, badge, icon). Scales from `100` to `400` are included.

## Build and Test Commands

### Build requirements

- Windows 10.0.19045 (Windows 10 22H2) or later is recommended for development.
- Visual Studio 2022 with the **C++ desktop development workload**.
- The following individual components should be installed:
  - Windows App SDK (or restore NuGet packages)
  - C++/WinRT
  - Windows SDK (targeting 10.0.19041 or newer)

### Restore NuGet packages

Open a **Developer Command Prompt for Visual Studio** and run:

```cmd
msbuild LuckyStudentPicker.vcxproj -t:Restore
```

Alternatively, open `LuckyStudentPicker.sln` in Visual Studio and choose **Restore NuGet Packages**.

### Build from the command line

```cmd
msbuild LuckyStudentPicker.sln -p:Configuration=Release -p:Platform=x64
```

Supported configurations: `Debug`, `Release`.  
Supported platforms: `x86` (`Win32`), `x64`, `ARM64`.

### Build and deploy from Visual Studio

1. Open `LuckyStudentPicker.sln` in Visual Studio 2022.
2. Select the desired configuration/platform (e.g., `Debug | x64`).
3. Press **F5** or choose **Debug > Start Debugging** to build, deploy, and run.

### Package and publish

The project is configured as a single-project MSIX app with packaging enabled:

- `AppxPackage=true`
- `EnableMsixTooling=true`
- Signing certificate: `LuckyStudentPicker_TemporaryKey.pfx`
- `AppxPackageSigningEnabled=true`
- `AppxBundle=Never`

To create an MSIX installer in Visual Studio, right-click the project and choose **Publish > Create App Packages**.

### Testing

There are **no automated unit tests** in this project. Testing is manual via running the app. Agents should not create a test project unless explicitly requested.

## Runtime Architecture

1. `App::OnLaunched` creates and activates `MainWindow`.
2. `MainWindow` initializes logging, the namelist file, the title bar, and text-to-speech on startup.
3. The namelist is stored as a JSON array of strings in the app’s local data folder.
4. Tapping the main button (`updRandName`) consumes the next randomized name.
5. Settings, seat arranger, and namelist troubleshooting are each separate windows that are created on demand.

### Namelist format

The namelist must be a valid JSON array of strings, e.g.:

```json
["Alice", "Bob", "Charlie"]
```

The file is located at:

```text
%LocalAppData%\Packages\<PackageFamilyName>\LocalState\LuckyStudentPicker\UnluckyStudentsNamelist.json
```

If the file is empty or invalid, the app shows `NamelistTroubleshootingWindow`.

### Log file

Runtime events are appended to:

```text
%LocalAppData%\Packages\<PackageFamilyName>\LocalState\LuckyStudentPicker\UnluckyStudentsLog.log
```

## Code Style Guidelines

- Use `winrt::` namespaces explicitly in headers; `using namespace winrt;` is acceptable in `.cpp` files.
- Follow the existing pattern for each WinRT class:
  1. Define the runtime class in `.idl`.
  2. Implement it in `winrt::LuckyStudentPicker::implementation` in the `.xaml.h`/`.xaml.cpp` files.
  3. Provide a `factory_implementation` specialization.
- Include `pch.h` as the first include in every `.cpp` file.
- Include the generated implementation file with the guard:

  ```cpp
  #if __has_include("ComponentName.g.cpp")
  #include "ComponentName.g.cpp"
  #endif
  ```

- XAML objects must **not** call `InitializeComponent` in their constructor; the generated base class handles this.
- Mark event-handler parameters with `[[maybe_unused]]` when unused.
- Prefer `hstring` for WinRT strings and `to_string`/`to_hstring` for conversions.
- Keep UI text in the XAML; keep logic in C++.
- Mixed English/Chinese comments exist in older files; new code should use English comments to match recent files.

## Development Conventions

- The project uses legacy NuGet `packages.config` with package contents committed/expected under `packages/`. Do not convert to `PackageReference` without checking build impact.
- `DefaultLanguage` is `zh-CN`; user-facing strings in XAML may contain Simplified Chinese.
- The certificate thumbprint and `.pfx` file are tracked. Do not commit private keys or real production certificates.
- The `MyProperty` placeholder in most IDL files is unused and throws `hresult_not_implemented()`.
- Helper classes are singletons accessed via `Instance()`.
- Async work uses `winrt::Windows::Foundation::IAsyncAction` or `fire_and_forget`.

## Security Considerations

- The MSIX is signed with a self-signed development certificate (`LuckyStudentPicker_TemporaryKey.pfx`). End users must install the certificate before installing the MSIX. The certificate is not a production trusted certificate.
- `Package.appxmanifest` requests `runFullTrust` and `graphicsCapture` capabilities. New capabilities should be justified in the manifest and documented.
- The app executes `ShellExecuteA` to open the log file, data folder, and namelist file. Validate any user-influenced paths before passing them to shell APIs.
- The app reads JSON directly into `winrt::hstring` values; malformed JSON is caught and treated as an empty list, but do not assume the input file is trusted in a multi-user scenario.

## Deployment Notes

- Releases are published manually on GitHub: https://github.com/Mike825/LuckyStudentPicker/releases
- Each release includes the `.msix` installer and the `.pfx` certificate.
- The certificate expires annually; a new `.pfx` is published when it is renewed.
- Installation steps for users are documented in `readme.md`.

## Common Tasks

### Add a new XAML page or window

1. Create `.idl`, `.xaml`, `.xaml.h`, and `.xaml.cpp` in the project root.
2. Add the four files to `LuckyStudentPicker.vcxproj` in the correct `<ItemGroup>` sections (`ClInclude`, `Page`, `ClCompile`, `Midl`).
3. Include the generated header (`#include "ComponentName.g.h"`) in the `.xaml.h`.
4. Include the generated implementation file in the `.xaml.cpp` with the `__has_include` guard.
5. Add any new headers to `pch.h` if they are used in multiple files.
6. Build once to generate skeleton code.

### Add a helper class

1. Create `*.h` and `*.cpp` in the project root.
2. Add them to the `<ClInclude>` and `<ClCompile>` item groups in `.vcxproj`.
3. Include `pch.h` first in the `.cpp`.

### Update app version

Update the version string in both:
- `Package.appxmanifest` (`<Identity Version="..." />`)
- `AboutPage.xaml` (`Version ...` text block)
