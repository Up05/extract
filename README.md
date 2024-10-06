# extract
simple unix utility to just decompress a god damn file. 

# Usage

```sh
extract [filter-text]
```
If no `filter-text` is supplied it will try to extract all files, so that there will not be any:  
Currently supported file types: `.gz`, `.tar`, `.rar`, `.zip` & `.7z`.

*Original archives are moved to `./tmp` (except for afaik `.gz`)*

# Installation

```sh
git clone https://github.com/Up05/extract
cd extract
sh compile.sh
```
*Or just run the pre-compiled executable in the repo...*

# Extending the program

in `extract.c` on line `83` call the function:
```c
void try_extract(char* file_extension, char* program, char* program_flags, char* file)
```
And also you may increase the `repeat(5) { ...` -> `repeat(<AMOUNT OF try_extract CALLS YOU HAVE>) { ...`
