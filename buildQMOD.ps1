# Builds a .qmod file for loading with QuestPatcher
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

$ArchiveName = "scoresaberui_v0.1.0.qmod"
$TempArchiveName = "scoresaberui_v0.1.0.qmod.zip"

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk
Compress-Archive -Path "./libs/arm64-v8a/libscoresaberui.so", "./libs/arm64-v8a/libbeatsaber-hook_2_3_2.so", "./mod.json", "./resources/logo.jpg", "./resources/arrow_downactive.png", "./resources/arrowactive.png", "./resources/arrow.png", "./resources/arrow_down.png", "./resources/scoresaber.png", "./resources/bsmg.jpg", "./resources/country.png" -DestinationPath $TempArchiveName -Force
Move-Item $TempArchiveName $ArchiveName -Force