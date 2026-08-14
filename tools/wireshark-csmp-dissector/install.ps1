# -------------------------------------------------------------------------------------
# Wireshark CoAP/CSMP Dissector installer script for Windows
#
# Usage : .\install.ps1 [-Target "$env:APPDATA\Wireshark\plugins"] [-DryRun]
# Author: Manojna CSL <mcsl@cisco.com>, <manojnacsl@gmail.com>
# -------------------------------------------------------------------------------------

[CmdletBinding()]
param(
    [Parameter()]
    [string] $Target,

    [Parameter()]
    [switch] $DryRun
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$ScriptDirectory = Split-Path -Parent $MyInvocation.MyCommand.Path
$LuaFileName = 'coap_csmp_dissector.lua'
$ProtoFileName = 'csmp.proto'
$PluginFileNames = @(
    $LuaFileName
    $ProtoFileName
)
$TemporaryFile = $null

function Find-TShark {
    $command = Get-Command 'tshark.exe' -ErrorAction SilentlyContinue
    if ($null -ne $command) {
        return $command.Source
    }

    $candidates = @()
    if (-not [string]::IsNullOrWhiteSpace($env:ProgramFiles)) {
        $candidates += Join-Path $env:ProgramFiles 'Wireshark\tshark.exe'
    }
    if (-not [string]::IsNullOrWhiteSpace(${env:ProgramFiles(x86)})) {
        $candidates += Join-Path ${env:ProgramFiles(x86)} 'Wireshark\tshark.exe'
    }

    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath $candidate -PathType Leaf) {
            return $candidate
        }
    }

    return $null
}

function Find-PersonalLuaPluginDirectory {
    $tshark = Find-TShark
    if ($null -ne $tshark) {
        $folderOutput = & $tshark -G folders 2>$null
        foreach ($line in $folderOutput) {
            if ($line -match '^Personal Lua Plugins:\s*(.+?)\s*$') {
                return $Matches[1]
            }
        }
    }

    if (-not [string]::IsNullOrWhiteSpace($env:APPDATA)) {
        return Join-Path $env:APPDATA 'Wireshark\plugins'
    }

    return $null
}

function Test-ValidSourceFile {
    param([Parameter(Mandatory)][string] $Path)

    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "Required source file is missing: $Path"
    }
}

function Test-SameFileContent {
    param(
        [Parameter(Mandatory)][string] $First,
        [Parameter(Mandatory)][string] $Second
    )

    if (-not (Test-Path -LiteralPath $Second -PathType Leaf)) {
        return $false
    }

    return (Get-FileHash -LiteralPath $First -Algorithm SHA256).Hash -eq
        (Get-FileHash -LiteralPath $Second -Algorithm SHA256).Hash
}

function Install-CSMPFile {
    param(
        [Parameter(Mandatory)][string] $Source,
        [Parameter(Mandatory)][string] $Destination
    )

    $sourcePath = [System.IO.Path]::GetFullPath($Source)
    $destinationPath = [System.IO.Path]::GetFullPath($Destination)

    if ($sourcePath.Equals(
            $destinationPath,
            [System.StringComparison]::OrdinalIgnoreCase)) {
        Write-Host "Already installed: $destinationPath"
        return
    }

    if (Test-SameFileContent -First $sourcePath -Second $destinationPath) {
        Write-Host "Up to date: $destinationPath"
        return
    }

    if ($DryRun) {
        Write-Host "Would install: $sourcePath -> $destinationPath"
        return
    }

    $script:TemporaryFile = Join-Path $TargetDirectory (
        ".{0}.tmp.{1}" -f [System.IO.Path]::GetFileName($destinationPath), $PID
    )

    Copy-Item -LiteralPath $sourcePath -Destination $script:TemporaryFile -Force
    Move-Item -LiteralPath $script:TemporaryFile -Destination $destinationPath -Force
    $script:TemporaryFile = $null

    if (-not (Test-SameFileContent -First $sourcePath -Second $destinationPath)) {
        throw "Verification failed for $destinationPath"
    }

    Write-Host "Installed: $destinationPath"
}

if ([string]::IsNullOrWhiteSpace($Target)) {
    $Target = Find-PersonalLuaPluginDirectory
    if ([string]::IsNullOrWhiteSpace($Target)) {
        throw 'Unable to locate the plugin directory; specify it with -Target.'
    }
}

$TargetDirectory = [System.IO.Path]::GetFullPath($Target)

foreach ($fileName in $PluginFileNames) {
    Test-ValidSourceFile -Path (Join-Path $ScriptDirectory $fileName)
}

Write-Host "Wireshark personal Lua plugin directory: $TargetDirectory"

if (-not $DryRun) {
    New-Item -ItemType Directory -Path $TargetDirectory -Force | Out-Null
}

try {
    foreach ($fileName in $PluginFileNames) {
        Install-CSMPFile `
            -Source (Join-Path $ScriptDirectory $fileName) `
            -Destination (Join-Path $TargetDirectory $fileName)
    }
}
finally {
    if ($null -ne $TemporaryFile -and
        (Test-Path -LiteralPath $TemporaryFile)) {
        Remove-Item -LiteralPath $TemporaryFile -Force
    }
}

if ($DryRun) {
    Write-Host 'Dry run complete; no files were changed.'
}
else {
    Write-Host 'Installation complete. Restart Wireshark or reload Lua plugins.'
}
