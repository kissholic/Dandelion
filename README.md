# Dandelion

Dandelion is a fast file distribution network. It's designed for transferring large binary files, but also works on small files.

Dandelion is lightweight and portable.

## Usage


Connect to peers
``` shell
$ ./dandelion -c config.json
```

The config format:

``` json
{
    "peers": [
        {
            "ip": "127.0.0.1",
            "port": 10101
        },
        {
            "ip": "127.0.0.1",
            "port": 10102
        }
    ],

    "base.dir": "Dir/To/Base/Path",

    "base.files": [
        "example1.so",
        "example2.obj",
        "example3.exe"
    ]
}
```


## Design

Dandelion is divided into two parts: the CDC part and the p2p network part.

To minimize the data transferred over network, Dandelion exploits CDC to locates different content between two files, and synchronizes those data between two hosts.

Also, dadelon uses p2p network to further accelerate the transfer speed.

## Installation
### From Source
#### Prerequisites
- Compilers supporting C++20 (clang is recommended)
- xmake

``` shell
// build
$ xmake

// run
$ xmake run
```

## Contribution
Any contributes to Dandelion is welcomed!