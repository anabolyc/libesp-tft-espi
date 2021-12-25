#!/usr/bin/pwsh
$cwd = Get-Location
$pioinitgt = Join-Path -Path $(Get-Location) -ChildPath "platformio.ini"

Get-ChildItem -Include *.ino -Recurse | %{
    $ino = $_
    $folder = $_.Directory
    Write-Host "Found: $($_.Name) at $($_.Directory)"
    
    $srcFolder = Join-Path -Path $folder -ChildPath "src"
    # create src folder
    if (Test-Path -Path $srcFolder) {
        # Write-Host "src folder already exists"
    } else {
        New-Item -Path $folder -Name src -ItemType "directory"
    }

    Set-Location $folder
    $pioinirel = Resolve-Path $pioinitgt -Relative
    $pioini = Join-Path -Path $folder -ChildPath "platformio.ini"
    # link pip    
    if (Test-Path -Path $pioini) {
        # Write-Host "ini file already exists"
        # Remove-Item -Path $pioini
    } else {
        Write-host "$pioinitgt -> $pioinirel"
        New-Item -Path $folder -Name "platformio.ini" -ItemType "SymbolicLink" -Target $pioinirel
    }
    Set-Location $cwd

    # move .h to src 
    Get-ChildItem -Path $folder -File | ?{ $_.Extension -Ne ".ini"} | %{
        if ($_.Extension -Ne ".ino") {
            Move-Item -Path $_ -Destination $srcFolder
        } else {
            $newpath = Join-Path -Path $srcFolder -ChildPath "main.cpp"
            Move-Item -Path $_ -Destination $newpath
        }
    }
   
}