#!/bin/bash
echo '
OpenInTerminal("'$1'")

on OpenInTerminal(currentPath)
	tell application "Terminal"
		activate
		delay 0.25
		tell application "System Events" to tell process "Terminal" to keystroke "t" using command down
		do script "cd " & currentPath & "; clear" in front window
	end tell
end OpenInTerminal

on OpenInSublime(currentPath)
	tell application "Terminal"
		activate
		delay 0.25
		tell application "System Events" to tell process "Terminal" to keystroke "t" using command down
		do script "cd " & quoted form of currentPath & "; clear" in front window
	end tell
end OpenInTerminal
' | osascript

