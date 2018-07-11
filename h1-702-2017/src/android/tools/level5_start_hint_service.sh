#!/bin/bash

adb shell am stopservice -a com.h1702ctf.ctfone5.action.HINT -e com.h1702ctf.ctfone5.extra.PARAM1 orange com.h1702ctf.ctfone5/.CruelIntentions
adb shell am startservice -a com.h1702ctf.ctfone5.action.HINT -e com.h1702ctf.ctfone5.extra.PARAM1 orange com.h1702ctf.ctfone5/.CruelIntentions
