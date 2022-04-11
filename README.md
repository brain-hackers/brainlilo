BrainLILO
=========

*Jump to U-Boot directly from Windows CE running on a SHARP Brain.*


Build on x86_64 Linux
---------------------

1. Get [cegcc](https://github.com/brain-hackers/cegcc-build/releases) and unzip it
2. Copy the directory `cegcc` into `/opt`
3. Run `make` with the `PREFIX` specified

Equivalent shell snippet:

```shell
wget -O cegcc.zip https://github.com/brain-hackers/cegcc-build/releases/download/2022-04-11-133546/cegcc-2022-04-11-133546.zip
unzip -q cegcc.zip
mkdir -p /opt  # it may need sudo
cp -r cegcc /opt/  # it may need sudo
make PREFIX=/opt/cegcc
```


Install (using the pre-built binary)
------------------------------------

1. Download the ZIP in [the latest release](https://github.com/brain-hackers/brainlilo/releases) and unzip it
2. Create `アプリ` directory in the SD card's root directory and copy `BrainLILO` into it
3. Create `LOADER` directory in the SD card's root directory and copy `u-boot.bin` into it


Install (using your binary)
---------------------------

1. Create a directory named `BrainLILO`
2. Copy dlls and the exe into it, renaming BrainLILO.exe to AppMain.exe
3. Create a blank `index.din` in the directory
4. Follow the step 2 and 3 in [Install (using the pre-built binary)](#install-using-the-pre-built-binary)

Equivalent shell snippet:

```shell
mkdir BrainLILO
cp *.dll BrainLILO/
cp BrainLILO.exe BrainLILO/AppMain.exe
touch BrainLILO/index.din
```
