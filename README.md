> [!IMPORTANT]\
> DO NOT SYNC FROM UPSTREAM!!!!!
> This repo is for _glabels-qt_, NOT the legacy _glabels-3_ version (which I have not maintained since 2018).

![gLabels Label Designer](glabels/images/glabels-label-designer.png)

![Cover Image](docs/images/cover-image.png)

[![Multi-Platform Build Tests](https://github.com/j-evins/glabels-qt/actions/workflows/build-tests.yml/badge.svg?branch=master&event=push)](https://github.com/j-evins/glabels-qt/actions/workflows/build-tests.yml)

---

## What is gLabels-qt?

gLabels-qt is the development version of the next major version of gLabels (a.k.a. glabels-4).

## What's new in gLabels 4?

- A complete rewrite, based on the Qt6 framework.
- A new UI layout based on common activities.
- Cross-platform support
- User-defined variables
- Support for continuous-roll labels
- Many new product templates

## How to build for Windows

## Fixed jpeg not loaded up issue

```powershell
# fetch the latest upstream references so we can confirm your fork’s baseline
git switch -c feature/windows-docker-jpeg-support
git fetch upstream --prune
#compare branch baseline with upstream/master to see how many commits you are ahead/behind
git rev-list --left-right --count upstream/master...HEAD
# Add the local Visual Studio build folders to .gitignore so .vs/ and out/ cannot accidentally be committed
Add-Content -LiteralPath '.gitignore' -Value "`r`n# Local Visual Studio files`r`n.vs/`r`nout/"
# add a clean Windows cross-build workflow from scratch, using Wine only inside Docker for the Windows Qt tools and executable deployment
New-Item -ItemType Directory -Path '.\docker\windows-mingw' -Force
# base a custom builder on Qt 6.7 with 64-bit MinGW, rather than the old Qt 6.2.3 image that produced the JPEG issue.
docker pull stateoftheartio/qt6:6.7-mingw-aqt
# Inspect the exact toolchain versions and confirm that the JPEG plugin exists:
docker run --rm stateoftheartio/qt6:6.7-mingw-aqt sh -lc 'qt-cmake --version; ninja --version; wine cmd /c "C:\Qt\Tools\mingw1120_64\bin\g++.exe --version"; find "$HOME/.wine/drive_c/Qt" -type f -iname "qjpeg.dll" -print'
# Inspect the JPEG plugin’s complete runtime dependencies before building anything:
docker run --rm stateoftheartio/qt6:6.7-mingw-aqt sh -lc 'wine C:/Qt/Tools/mingw1120_64/bin/objdump.exe -p C:/Qt/6.7.0/mingw_64/plugins/imageformats/qjpeg.dll | grep "DLL Name"'

# after some file editing in docker/windows-mingw/Dockerfile, build the new Windows cross-build image:
docker build --file '.\docker\windows-mingw\Dockerfile' --tag glabels-qt-windows-builder:6.7 .

# Configure the actual gLabels Windows build using the repository-owned image:
docker run --rm --mount "type=bind,source=$((Resolve-Path '.').Path),target=/workspace" glabels-qt-windows-builder:6.7 sh -lc 'qt-cmake -S Z:/workspace -B Z:/workspace/out/windows-mingw/glabels -G Ninja -DCMAKE_BUILD_TYPE=Release'

# Build only the two application targets, avoiding the currently broken unit-test executables:
docker run --rm --mount "type=bind,source=$((Resolve-Path '.').Path),target=/workspace" glabels-qt-windows-builder:6.7 sh -lc 'wine C:/Qt/Tools/CMake_64/bin/cmake.exe --build Z:/workspace/out/windows-mingw/glabels --target glabels-qt glabels-batch-qt --parallel'

# Before changing the installation rules, verify exactly what the post-build deployment produced, especially qjpeg.dll and the MinGW runtime DLLs.
Get-ChildItem -LiteralPath '.\out\windows-mingw\glabels\glabels' -Recurse -File | Where-Object { $_.Name -in @('glabels-qt.exe','qjpeg.dll','Qt6Core.dll','Qt6Gui.dll','libgcc_s_seh-1.dll','libstdc++-6.dll','libwinpthread-1.dll') } | Select-Object FullName,Length

# Update only the MinGW deployment block so windeployqt deploys the correct 64-bit runtime automatically, then remove the incorrect hardcoded libgcc_s_dw2-1.dll block:
# glabels/CMakeLists.txt
docker run --rm --mount "type=bind,source=$((Resolve-Path '.').Path),target=/workspace" glabels-qt-windows-builder:6.7 sh -lc 'rm -f /workspace/out/windows-mingw/glabels/glabels/glabels-qt.exe && wine C:/Qt/Tools/CMake_64/bin/cmake.exe --build Z:/workspace/out/windows-mingw/glabels --target glabels-qt --parallel'

# Verify that the correct 64-bit MinGW runtime and JPEG plugin were deployed:
Get-ChildItem -LiteralPath '.\out\windows-mingw\glabels\glabels' -Recurse -File | Where-Object { $_.Name -in @('glabels-qt.exe','qjpeg.dll','libgcc_s_seh-1.dll','libstdc++-6.dll','libwinpthread-1.dll') } | Select-Object FullName,Length

# update cmakelists and regenerate the build system after replacing both CMakeLists.txt files. This validates the new CMake syntax before rebuilding or installing anything.
docker run --rm --mount "type=bind,source=$((Resolve-Path '.').Path),target=/workspace" glabels-qt-windows-builder:6.7 sh -lc 'qt-cmake -S Z:/workspace -B Z:/workspace/out/windows-mingw/glabels -G Ninja -DCMAKE_BUILD_TYPE=Release'


# Rebuild both application targets so the revised windeployqt post-build rule runs:
docker run --rm --mount "type=bind,source=$((Resolve-Path '.').Path),target=/workspace" glabels-qt-windows-builder:6.7 sh -lc 'wine C:/Qt/Tools/CMake_64/bin/cmake.exe --build Z:/workspace/out/windows-mingw/glabels --target glabels-qt glabels-batch-qt --parallel'

# Retry installation into a clean deployment directory:
docker run --rm --mount "type=bind,source=$((Resolve-Path '.').Path),target=/workspace" glabels-qt-windows-builder:6.7 sh -lc 'rm -rf /workspace/out/windows-mingw/dist && wine C:/Qt/Tools/CMake_64/bin/cmake.exe --install Z:/workspace/out/windows-mingw/glabels --prefix Z:/workspace/out/windows-mingw/dist'
```

## Download

### Latest Release

There are currently no official releases of gLabels 4.

### Continuous Integration Snapshots

Currently there are no self-hosted binary snapshot releases available. I plan to make these available again once 4.0 is more imminent. In the mean time, I encourage you to try building the code yourself.

Some third-party packages may also be available:

| Platform  | Files                                                                                | Notes                                                          |
| :-------- | :----------------------------------------------------------------------------------- | :------------------------------------------------------------- |
| Archlinux | [Archlinux User Repository Page](https://aur.archlinux.org/packages/glabels-qt-git/) | Maintained by [Maud Spierings](https://github.com/SpieringsAE) |
| Ubuntu    | [PPA Page](https://code.launchpad.net/~krisives/+archive/ubuntu/glabels-qt)          | Maintained by [Kristopher Ives](https://github.com/krisives)   |
| Fedora    | [Copr Repository Page](https://copr.fedorainfracloud.org/coprs/mariobl/glabels-qt/)  | Maintained by [Mario Blättermann](https://github.com/mariobl)  |
| Windows   | [Github Page](https://github.com/ibsorn/glabels-windows)                             | Maintained by [ibsorn](https://github.com/ibsorn)              |

## Build Instructions

- [Linux Build Instructions](docs/BUILD-INSTRUCTIONS-LINUX.md)
- [Windows Build Instructions](docs/BUILD-INSTRUCTIONS-WINDOWS.md)
- [Mac Build Instructions](docs/BUILD-INSTRUCTIONS-MACOS.md)

## Help Needed

Please see [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md).

## License

gLabels-qt is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gLabels-qt is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

See [LICENSE](LICENSE) in this directory.

The following sub-components are also made available under less
restrictive licensing:

### Glbarcode

gLabels-qt currently includes a version of the glbarcode++ library, located in
the "glbarcode/" subdirectory. It is licensed under the GNU LESSER GENERAL
PUBLIC LICENSE (LGPL); either version 3 of the License, or (at your option)
any later version. See [glbarcode/LICENSE](glbarcode/LICENSE).

### Template Database

The XML files in the "templates/" subdirectory constitute the glabels
label database. No copyright is claimed on the facts contained within
the database and can be used for any purpose. The files themselves are
licensed using the MIT/X license. See [templates/LICENSE](templates/LICENSE).
