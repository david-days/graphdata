#!/usr/bin/env pwsh
param (
    [switch]$clean = $false,
    [switch]$install = $false,
    [switch]$test = $false,
    [switch]$v = $false,
    [switch]$vv = $false,
    [switch]$package = $false,
    [switch]$d = $false,
    [switch]$l = $false,
    [switch]$help = $false
)

$projectDir=$PSScriptRoot

# build directory
$linuxBuildDir = Join-Path  $projectDir "build-linux-x64"
$winBuildDir = Join-Path $projectDir "build-win-x64"
$osxBuildDir = Join-Path $projectDir "build-osx"
$cmakeBuildDir = Join-Path $projectDir "cmake-build-debug"

# testing and binary folders
$binaryOutDir = Join-Path $projectDir "output"
$testDir = Join-Path $projectDir "tests"
$testBinaries = Join-Path $testDir "bin"
$toolChainDir = Join-Path $projectDir "cmake"
$nupkgDir = Join-Path $binaryOutDir "nupkgs"


# toolchain files
$linuxToolchain = Join-Path $toolChainDir "linux-x64.cmake"
$winToolchain = Join-Path $toolChainDir "win-x64.cmake"
$osxToolchain = Join-Path $toolChainDir "osx.cmake"

# nuspec and NuGet build projects
$linuxRuntimeProj = Join-Path $binaryOutDir "linux_runtime.csproj"
$winRuntimeProj = Join-Path $binaryOutDir "win_runtime.csproj"
# TODO:  set up OSX


$toolChainFile = $null
$projBuildDir = $null
$runtimeProject = $null
$nugetRuntime = $null

if ($IsMacOS) {
    $toolChainFile = $osxToolchain
    $projBuildDir = $osxBuildDir
    # TODO:  Set up OSX .csproj and nuget runtime option
} elseif ($IsLinux) {
    $toolChainFile = $linuxToolchain
    $projBuildDir = $linuxBuildDir
    $runtimeProject = $linuxRuntimeProj
    $nugetRuntime = "linux-x64"
} elseif ($IsWindows) {
    $toolChainFile = $winToolchain
    $projBuildDir = $winBuildDir
    $runtimeProject = $winRuntimeProj
    $nugetRuntime = "win-x64"
} else {
    Write-Error "Unsupported host.  Host type not detected or recognized"
    exit 1;
}

if ($help) {
    Write-Host "Usage: ./BuildgraphData.ps1 [-clean] [-test] [-install] [-v] [-vv] [-package] [-help]"
    Write-Host "    No args     (Re)Build"
    Write-Host "	-clean      Clean output folders and exit"
    Write-Host "	-install    Build and install"
    Write-Host "	-test       Build and run tests, then exit"
    Write-Host "	-v          Print out CMAKE build variables and exit"
    Write-Host "	-vv         Print all CMAKE build variables used during build"
    Write-Host "    -package    Package the file according to platform"
    Write-Host "    -d          Create local documentation for the code"
    Write-Host "    -l          Run the linter on the code during build"
    Write-Host "	-help       Print this help and exit"
    exit 0
    ;;
}

# Create the build directory if it doesn't exist
if (!(Test-Path $projBuildDir -PathType Container)) {
    New-Item -ItemType Directory -Force -Path $projBuildDir
}

if ($clean) {
    Write-Host "Cleaning..."
    if (Test-Path $projBuildDir -PathType Container) {
        $buildContents = Join-Path $projBuildDir "*"
        Remove-Item $buildContents -Recurse -Force
    }
    if (Test-Path $binaryOutDir -PathType Container) {
        $binaryContents = Join-Path $binaryOutDir "*"
        Remove-Item $binaryContents -Recurse -Force
    }
    if (Test-Path $testBinaries -PathType Container) {
        $testContents = Join-Path $testBinaries "*"
        Remove-Item $testContents -Recurse -Force
    }
    if (Test-Path $cmakeBuildDir -PathType Container) {
        $cbuildContents = Join-Path $cmakeBuildDir "*"
        Remove-Item $cbuildContents -Recurse -Force
    }
    exit 0
}

if ($v) {
    # Printing out all cached variables
    cmake . -LH --toolchain $toolChainFile -B $projBuildDir
    exit 0
}

$printAllVars = 0
if ($vv) {
    $printAllVars = 1
}

$runLinter = 0
if ($l) {
    $runLinter = 1
}

& cmake . --toolchain $toolChainFile -B $projBuildDir -DPRINT_ALL_VARS=$printAllVars -DRUN_LINTER=$runLinter 
& cd $projBuildDir 
& make all 

# From here on we're in the build directory unless we explicity move out

if ($d) {
    & make docs
}

if ($test) {
    & make test
}

if ($package) {
    # build the cmake packages
    & make package
    # Native runtime NuGet package
    & dotnet restore $runtimeProject
    & dotnet pack $runtimeProject --include-symbols --no-build -o $nupkgDir --runtime $nugetRuntime -c Release
}