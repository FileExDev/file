# The FILE Ledger

The FILE Ledger is a decentralized cryptographic ledger powered by a network of peer-to-peer servers. The FILE Ledger uses a novel Byzantine Fault Tolerant consensus algorithm to settle and record transactions in a secure distributed database without a central operator.

## FILE
FILE is a public, counterparty-less asset native to the FILE Ledger, and is designed to bridge the many different currencies in use worldwide. FILE is traded on the open-market and is available for anyone to access. The FILE Ledger was created in 2020 with a finite supply of 2.1 billion units of FILE. Its creators gifted 80 billion FILE to a company, to develop the FILE Ledger and its ecosystem.  Ripple uses FILE the help build the Internet of Value, ushering in a world in which money moves as fast and efficiently as information does today.

## file
The server software that powers the FILE Ledger is called `file` and is available in this repository under the permissive [ISC open-source license](LICENSE). The `file` server is written primarily in C++ and runs on a variety of platforms.

### Build from Source

* [Linux](Builds/linux/README.md)
* [Mac](Builds/macos/README.md)
* [Windows](Builds/VisualStudio2017/README.md)


### Repository Contents

| Folder     | Contents                                         |
|:-----------|:-------------------------------------------------|
| `./bin`    | Scripts and data files for Ripple integrators.   |
| `./Builds` | Platform-specific guides for building `file`. |
| `./docs`   | Source documentation files and doxygen config.   |
| `./cfg`    | Example configuration files.                     |
| `./src`    | Source code.                                     |

Some of the directories under `src` are external repositories included using
git-subtree. See those directories' README files for more details.



