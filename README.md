# pal (Name Pending)

A unix-style program to change terminal color-schemes on the fly.

A list of hex colors are supplied via command-line arguments, `stdin` or a file and `pal` sends these colors to all open terminals, modifying their palette. The purpose of this tool is to allow theme managers and individual scripts to effortlessly modify the current color-scheme without needing a reload of running terminals.

## Usage

`pal` takes `16` colors as input. Color `0` is assumed to be the background color and color `15` is assumed to be the foreground color. When less than `16` colors are supplied, `pal` repeats the last color till `16`.

**NOTE**: The input colors can optionally contain `#`, it doesn't matter.

```sh
# ARGUMENTS

# arguments
pal 1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7

# arguments (string)
pal "1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7"

# arguments (file)
# Colors inside of a file can be separated by either new-lines or spaces.
pal file


# STDIN

# stdin
echo 1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 | pal

# stdin (file)
# Colors inside of a file can be separated by either new-lines or spaces.
pal < file

# stdin (string)
pal <<< "1f221f a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7 575957 a0ab9e b6bfb4 67a96c 90a992 79a985 47a961 c7c7c7"


# EXTRA

# Two tone palette.
# Color 0 is black and colors 1-15 are white.
pal "000000" "FFFFFF"
```

### Applying colors to newly opened terminals.

Add the following line to your shell configuration file.

```sh
# Inside .bashrc, .zshrc, .profile or equivalent.
(pal -r &)
```

## TODO

- [ ] TTY support.
- [ ] macOS support.
