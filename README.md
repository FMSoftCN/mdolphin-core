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


## Configure and install on Ubuntu Linux 16.04 with MiniGUI 3.2.3

1. Download ubuntu-16.04.6-desktop-i386.iso from:
    http://releases.ubuntu.com/16.04/?_ga=2.94179613.1098001605.1581330736-948418017.1558570895

2. Install Ubuntu 16.04.6, or setup your virtual machine and install;

3. In Ubuntu 16.04, install the packages after you set root password:
```css
    $ sudo passwd root
    $ sudo apt-get install vim openssh-server autoconf cmake libpciaccess-dev libjpeg-dev libpng12-dev libfreetype6-dev libsqlite3-dev libxml2-dev chipmunk-dev libgtk2.0-dev libcurl4-openssl-dev libssl-dev libxslt-dev libc-ares-dev libbison-dev build-essential libtool
    $ sudo apt-get install -y gcc-4.8 g++-4.8
    $ cd /usr/bin
    $ sudo rm gcc
    $ sudo ln -s gcc-4.8.5 gcc
    $ sudo rm g++
    $ sudo ln -s g++-4.8.5 g++
```

4. Download MiniGUI software package 3.2.3 from www.fmsoft.com, and build them according to the menual;

5. In the older version of Ubuntu, libicu-dev was not in the distribution, but in Ubuntu 16.04, it was installed by default. In the compiling process, the compiler will use libicu-dev package, but not the code in the icu directory. So remove it first:
```css
    $ sudo apt-get autoremove libicu-dev
        Reading package lists... Done
        Building dependency tree
        Reading state information... Done
        The following package was automatically installed and is no longer required:
          icu-devtools
        Use 'sudo apt autoremove' to remove it.
        The following packages will be REMOVED:
          libicu-dev libxml2-dev libxslt1-dev
        0 upgraded, 0 newly installed, 3 to remove and 299 not upgraded.
        After this operation, 45.3 MB disk space will be freed.
        Do you want to continue? [Y/n] n
        Abort.
```
    Then answers: "Y".

6. Download libxml2-dev and libxslt1-dev from http://www.minigui.com/download, compile and install them with:
```css
    $ tar zxf libxxxxxxx
    $ cd libxxxxxxx
    $ ./configure
    $ sudo make install
```

7. Get mdolphine-core and mdolphin-sample from git:
```css
    $ git clone https://github.com/FMSoftCN/mdolphin-core.git
    $ git clone https://github.com/FMSoftCN/mdolphin-samples.git
```
    
8. Compile mdolpine library:
```css
    $ cd mdolphin-core
    $ ./autogen.sh
    $ ./configure
    $ make
    $ sudo make install
```

9. Compile
```css
    $ cd mdolphin-samples
    $ ./autogen.sh
    $ configure
    $ cd pc
    $ make
```
   Then you get the app: mdolphin_app
   
10. After develop mdolphin, do not forget installing libicu-dev:
```css
    $ sudo apt-get install libicu-dev
```


## Cross compliation

    Please see the sample scripts in 'CrossBuild/' directory.


