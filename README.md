# paleta

<a href="https://travis-ci.org/dylanaraps/paleta"><img src="https://travis-ci.org/dylanaraps/paleta.svg?branch=master"></a>

Change terminal colors on the fly.

A list of hex colors are supplied via command-line arguments, `stdin` or a file and `paleta` sends these colors to all open terminals, modifying their palette. The purpose of this tool is to allow theme managers and individual scripts to effortlessly modify the current color-scheme without needing a reload of running terminals.

## Installation

- Add `paleta` to your path.

**Full Installation.**

1. Download `paleta`.
    - Release: https://github.com/dylanaraps/paleta/releases/latest
    - Git: `git clone https://github.com/dylanaraps/paleta`
2. Change working directory to `paleta`.
    - `cd paleta`
3. Run `make install` inside the script directory to install the script.
    - **NOTE**: You may have to run this as root.

**NOTE:** `paleta` can be uninstalled easily using `make uninstall`. This removes all of files from your system.

## Usage

`paleta` takes `16` to `19` colors as input. If supplied, arguments `17`, `18`, and `19` are used for the foreground, background, and cursor color, respectively. The foreground and cursor color will default to color `15`, and the background color will default to color `0`. When less than `16` colors are supplied, `paleta` repeats the last color till `16`.

**NOTE**: The input colors can optionally contain `#`, it doesn't matter.

```sh
# ARGUMENTS

# arguments
paleta 1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7

# arguments (string)
paleta "1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7"

# arguments (file)
# Colors inside of a file can be separated by either new-lines or spaces.
paleta file


# STDIN

# stdin
echo 1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 | paleta

# stdin (file)
# Colors inside of a file can be separated by either new-lines or spaces.
paleta < file

# stdin (string)
paleta <<< "1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7"


# EXTRA

# Two tone palette.
# Color 0 is black and colors 1-15 are white.
paleta "000000" "FFFFFF"
```

### Applying colors to newly opened terminals.

Add the following line to your shell configuration file.

```sh
# Inside .bashrc, .zshrc, .profile or equivalent.
(paleta -r &)

# Faster more verbose method (bash only).
{ read -r < ~/.cache/paleta/colors; printf %b "$REPLY"; } & disown
```
