# SendKeyboardKeys

Simulate keyboard input on Windows using C++ and the Win32 API.

## Features

- Type any Unicode text into the active window
- Optionally send an Enter key after typing
- Wait for a specified number of seconds before typing

## Build Instructions

1. Open **x64 Native Tools Command Prompt for VS** (Visual Studio).
2. Navigate to the folder containing `SendKeys.cpp`.
3. Run:
   ```
   cl.exe SendKeys.cpp /Fe:SendKeys.exe
   ```

## Usage

```
SendKeys.exe --text <text> [--new-line] [--wait <seconds>]
```

### Options

- `--text <text>`: The text to type.
- `--new-line`: Send an Enter key after typing.
- `--wait <seconds>`: Wait before typing (default: 3 seconds).

### Example

Type "Hello World" and press Enter after 5 seconds:

```
SendKeys.exe --text "Hello World" --new-line --wait 5
```

## License

[MIT](LICENSE)
