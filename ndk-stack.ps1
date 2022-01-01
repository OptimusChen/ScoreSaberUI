$NDKPath = $env:ANDROID_NDK_HOME

$stackScript = "$NDKPath/ndk-stack"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $stackScript += ".cmd"
}

Get-Content ./log.txt | & $stackScript -sym ./build/debug/ > log_unstripped.log