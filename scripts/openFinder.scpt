#!/bin/bash
echo '
RevealInFinder("'$1'")

on RevealInFinder(currentPath)
	set posixFile to POSIX file currentPath
	tell application "Finder" 
		reveal posixFile
		activate
	end tell
end RevealInFinder
' | osascript

