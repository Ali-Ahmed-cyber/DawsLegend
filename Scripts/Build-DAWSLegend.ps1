param(
    [string]$EngineRoot = $(if ($env:UE_5_4_ROOT) { $env:UE_5_4_ROOT } else { "C:\Program Files\Epic Games\UE_5.4" }),
    [string]$ProjectPath = (Join-Path $PSScriptRoot "..\DAWSLegend.uproject"),
    [string]$OutputDirectory = (Join-Path $PSScriptRoot "..\Build\Windows"),
    [switch]$Package
)

$ErrorActionPreference = "Stop"
$ProjectPath = [System.IO.Path]::GetFullPath($ProjectPath)
$EngineRoot = [System.IO.Path]::GetFullPath($EngineRoot)
$BuildTool = Join-Path $EngineRoot "Engine\Build\BatchFiles\Build.bat"
$AutomationTool = Join-Path $EngineRoot "Engine\Build\BatchFiles\RunUAT.bat"

if (-not (Test-Path $ProjectPath)) {
    throw "Project file not found: $ProjectPath"
}
if (-not (Test-Path $BuildTool)) {
    throw "Unreal Engine 5.4 Build.bat not found under $EngineRoot. Install UE 5.4 or pass -EngineRoot."
}

Write-Host "Building DAWS LEGEND Editor target with Unreal Engine 5.4..."
& $BuildTool "DAWSLegendEditor" "Win64" "Development" "-Project=$ProjectPath" "-WaitMutex" "-NoHotReloadFromIDE"
if ($LASTEXITCODE -ne 0) {
    throw "Unreal Editor target build failed with exit code $LASTEXITCODE."
}

Write-Host "Editor target build succeeded."
if ($Package) {
    if (-not (Test-Path $AutomationTool)) {
        throw "RunUAT.bat not found: $AutomationTool"
    }
    $OutputDirectory = [System.IO.Path]::GetFullPath($OutputDirectory)
    Write-Host "Packaging a Windows Shipping build to $OutputDirectory ..."
    & $AutomationTool "BuildCookRun" "-project=$ProjectPath" "-noP4" "-platform=Win64" "-clientconfig=Shipping" "-build" "-cook" "-stage" "-pak" "-archive" "-archivedirectory=$OutputDirectory" "-map=/Engine/Maps/Entry" "-utf8output"
    if ($LASTEXITCODE -ne 0) {
        throw "Windows packaging failed with exit code $LASTEXITCODE."
    }
    Write-Host "Packaging completed."
}
