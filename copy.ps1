param (
    [Parameter(Mandatory=$false)]
    [Switch]$debug_so,
    [Parameter(Mandatory=$false)]
    [Switch]$log
)

& ./build.ps1
if (-not ($LastExitCode -eq 0)) {
    echo "build failed, not copying"
    exit
}

if ($debug_so.IsPresent) {
    & adb push build/debug/libScoreSaberUI.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libScoreSaberUI.so
} else {
    & adb push build/libScoreSaberUI.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libScoreSaberUI.so
}

& adb shell am force-stop com.beatgames.beatsaber
& adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
if ($log.IsPresent) {
    & ./log.ps1
}