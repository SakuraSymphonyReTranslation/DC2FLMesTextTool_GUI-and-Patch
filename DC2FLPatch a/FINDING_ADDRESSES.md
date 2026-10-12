# Finding DC2FL Memory Addresses with x32dbg

## Method: The String Search (Recommended)

To find game-specific hooks and addresses directly by searching for text:

1. **Restart Debugging** (`Ctrl+F2` or File -> Reload `DC2FL.EXE`).
2. **Go to CPU tab**.
3. **Right-click** in the code window -> **Search for** -> **All modules** -> **String references**.
4. **Wait** for the search to complete (progress bar at bottom).
5. **Filter** for: `DC2FL` or `CIRCUS`.
6. **Double-click** the result.
7. **Scroll down** in the code view. Look for a call to `RegisterClassA`.
   - The code just before it prepares the `WNDCLASSA` structure.
   - Look for an instruction like `mov [ebp-XX], <ADDRESS>`.
   - That `<ADDRESS>` is likely the **WndProc**!

## Expected Pattern in Code
You'll see something like this:

```assembly
push <WndProc_Address>  ; The address we want!
pop ...
mov [ebp-04], ...       ; Storing WndProc
...
call RegisterClassA
```

Or:

```assembly
mov dword ptr [ebp-1C], <WndProc_Address>  ; Address found
...
call RegisterClassA
```
