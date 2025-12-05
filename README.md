# Bithide – A Primitive BMP Steganography Tool

[![License: MIT](https://img.shields.io/badge/License-MIT-green)](https://github.com/Arx0xEF/bithide/blob/main/LICENSE)
[![Status: WIP](https://img.shields.io/badge/Status-WIP-orange)](https://github.com/Arx0xEF/bithide)
[![GitHub Repo](https://img.shields.io/badge/GitHub-Repo-blue?logo=github)](https://github.com/Arx0xEF/bithide)

---

## Overview

Bithide is a steganography tool written in C, specifically supporting **24-bit BMP images**. It embeds a secret file into a cover file by hiding bits in the **Least Significant Bit (LSB)** of pixel data. Since modifying the LSB is barely visible to the human eye, this method efficiently hides data in another file.

Currently, the program can:

- Print information about BMP files
- Embed secret files into BMP images

Planned features:

- Extraction of hidden files
- Argument parsing for easier usage
- Basic encryption for extra security

The project is a **Work In Progress (WIP)** and is pushed to GitHub to mark my first programming project journey. I dedicate my first project to my hb Kiran and The creator himself Mr. Ritchie.

Although there are some errors of using `malloc()` to store the BMP structs, but since i've got exams on my priority list, i will make sure to fix it after the exams are over and also fix some other naming errors. The Repo is a mess, i hope it's normal for someone novice like myself to not have his repo well maintained in the first try, but i will try to refine my git skills, maintaining a clean repo and also a clean and efficient code gradually.

---

## Build Instructions

1. Make sure you have a C compiler installed (e.g., `gcc`).
2. Build the program using the Makefile:
```bash
make
```
Run the program:
``` bash
./bithide
```
License

This project is licensed under the MIT License.
See the LICENSE

file for details.

TODO
- Implement argument parsing
- Implement extraction of hidden files
- Add simple encryption for hidden files
- Improve error handling and user messages
- Remove the `malloc()` for the BMP header `structs`
