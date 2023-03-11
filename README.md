# DOS Build GitHub action

This action calls `dosbox-x`, and provides several compilers (like Borland Turbo C) in order to compile in DOS.

## Inputs

### `run`

The contents of a DOS compatible batch file, which is invoked when DOS is first booted.

### `programs`

Mount paths to programs to host in DOS. This should be a multiline string, which is formatted as `SOURCE:TARGET`. For programs that are hosted in this project, use the prefix `$/`.

For example `$/turbo-asm/2.01:c:\tasm` will allow access to the Turbo Assembly v2.01 program contents in `c:\tasm`.

Note that if the `config.json` in the repository has a `PATH` field, then the `PATH` environment variable will get the mounted path included.

Default `""`.

### `conf`

Optional configuration for DOSBox-X. Default `""`.

## Example usage

```yml
- name: 'Build'
  uses: joncloud/dos-build-action@v1
  with:
    run: |
      C:
      CD SRC
      TASM /zi MAIN.ASM
      TLINK /v MAIN.OBJ
      MAIN.EXE > .\RUN.TXT
    programs: |
      $/turbo-asm/2.01:C:\TASM
      sample:C:\SRC
    conf: |
      [dosbox]
      memsize=256

      [cpu]
      cycles=max
      turbo=true
```

## Sample Projects

* [joncloud/got](https://github.com/joncloud/got)
