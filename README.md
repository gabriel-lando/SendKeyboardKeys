# SendKeyboardKeys

Simulate keyboard input on Windows using C++ and the Win32 API.

## Features

- Type any Unicode text into the active window
- Optionally send an Enter key after typing
- Wait for a specified number of seconds before typing
- Wait for Scroll Lock toggle before typing (useful for manual timing control)
- Keep repeating until Ctrl+C is pressed (useful for automation and testing)

## Build Instructions

1. Open **x64 Native Tools Command Prompt for VS** (Visual Studio).
2. Navigate to the folder containing `SendKeys.cpp`.
3. Run:
   ```
   cl.exe SendKeys.cpp /Fe:SendKeys.exe
   ```

## Usage

```
SendKeys.exe --text <text> [--new-line] [--wait <seconds> | --wait-scroll-lock] [--keep]
```

### Options

- `--text <text>`: The text to type.
- `--new-line`: Send an Enter key after typing.
- `--wait <seconds>`: Wait before typing (default: 2 seconds).
- `--wait-scroll-lock`: Wait for Scroll Lock toggle before typing (overrides --wait).
- `--keep`: Keep repeating until Ctrl+C is pressed.
- `--help`: Show help message.

### Examples

Type "Hello" and press Enter after 5 seconds:

```
SendKeys.exe --text "Hello" --new-line --wait 5
```

Type "World" and press Enter, waiting for Scroll Lock toggle:

```
SendKeys.exe --text "World" --new-line --wait-scroll-lock
```

### Scroll Lock Wait Behavior

When using `--wait-scroll-lock`:

- If Scroll Lock is **OFF**: Turn ON Scroll Lock, then turn it OFF to begin typing
- If Scroll Lock is **ON**: Turn OFF Scroll Lock to begin typing

This feature is useful when you need precise timing control, such as when typing passwords or sensitive information where you want to manually control exactly when the typing begins.

## License

[MIT](LICENSE)
