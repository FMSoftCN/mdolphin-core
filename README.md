# mDolphin Core

## Introduction

mDolphin is a WebKit-based browser for MiniGUI 3.0 or later.
This is the core library of mDolphin.


## Depedencies

  * MiniGUI V3.0.12
  * c-ares V1.0.4
  * curl V7.21.0
  * libxml2 V2.7.6
  * libxslt V1.1.26
  * openssl V0.9.8k
    * On Ubuntu Linux 16.04, use 'sudo apt install libcurl4-dev' command to install the dev package.
  * Cairo on MiniGUI V1.8.6
    * If you want to enable SVG support.

You can download all source code tarballs of above software from the following page:

    http://www.minigui.com/en/download/


## Configure and install on Ubuntu Linux 16.04

### Install the depedencies

  1. MiniGUI: download the source code and run configure script, then make and install.
  2. Run the following command to install other dependencies:

        sudo apt install libxml2-dev libcurl4-openssl-dev libssl-dev libxslt-dev libc-ares-dev

## Configure and install mDolphin lib

  Run

    $ ./configure
    $ make
    $ sudo make install

## Cross compliation

    Please see the sample scripts in 'CrossBuild/' directory.


