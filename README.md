# WASM Demo Programs
by Cy Anderson

## Introduction
Hello! This is my repository for experiments I am doing with WebAssembly.
I made this repository on Github, so that they are backed up and easily transferable to different machines.
I decided to make it public, so anyone could see how I managed to get WebAssembly to work.
As of the time I am writing this, I am still working on getting some demos to work well enough to share and am working on more to come afterwards.
Specifically, I am working on using SDL with WASM, which should open up some very interesting possibilities.

---

WebAssembly is an exciting new standard for the Internet.
It offers cross platform compatibility, near native speed and flexibility in choice of language.
Unfortunately, documentation for WebAssembly is somewhat lacking, and available code samples often do not work without modification.
This set of demo programs is meant to help learn WebAssembly by taking a gradual approach to getting WebAssembly to work.
It starts with the basic "Hello World" program in C, that you should be familiar with.
From there it adds to the process, so new concepts are more isolated and easy to follow.
The best way to learn from these demos is to read the README file for the demo then look at the source code, including Makefiles.
Code is documented fairly well, but some details are explained only in the README files.

In order to build WebAssembly, or WASM, code, we must use a compiler that can output WebAssembly as its target.
The compilier most often used with Linux, gcc, is unable to do this, so we will use clang instead.
Note that most WebAssembly tutorials use emscripten to generate WASM.
The goal of these demos is to show how WASM works from a more basic level, so most of these do not use emscripten.
emscripten generates a large amount of overhead, including JavaScript and HTML that obfuscate some of what is being done under the hood.

## Prerequisites
You do not have to be an advanced programmer to use these demos, but you do need to have at least a beginning level of experience with the following to understand and get the most out of these demos.
* Linux / bash
* HTML
* JavaScript
* C/C++
* make
* Browser Dev Tools / Debugging
* Basic understanding of binary / decimal / hexidecimal
* Basic understanding of program structure, such as memory, the stack, the heap, compiling and linking

## Environment
WebAssembly runs on an array of platforms. That is one of its main advantages.
For simplicity and ease of reproduction, these demos were developed primarily on a Virtual Machine running Kubuntu Linux 24.04.3.
They have been simultaneously devloped on an AWS EC2 instance also running Ubuntu 24.
Instructions assume you are familiar with Ubuntu or another Debian flavor of Linux, but they should work similarly for other distributions with only small modifications.
These demos should also be able to work on Windows, but it is easier to show exact command line steps for setup on Linux.

Several packages are needed in the process of building these demos.
They do not all have to be installed at once, however.
Instructions will be provided for installing necessary packages in the demos they are first used.
In case you want to skip around, here is a list of all packages that are installed in the process of doing the demos.
You can install them all at once to be sure you do not have any dependency errors.
Depending on the distribution you are using, some of these might not be needed to install, because they already are in the image.
These packages are listed in order to make this environment reproducible as exactly as possible.

* net-tools
* apache2
* gcc
* git
* make
* clang
* lld-18
* cmake
* wasi-libc
* libsdl2-dev
* python3 (needed for emscripten)

You may not need to install all of these packages if you do not do all the demos.
For example, python3 is only needed for demos involving emscripten.

## Setup
This section describes the basic setup that needs to be done before starting any demos.
Remember again, that this is assuming you are starting from a fresh, new Linux machine.
If you are using an existing Linux machine, many of these steps might not be necessary.

### Setup the Webserver

For this demo, we use the Apache2 webserver.
You can use another webserver if you like, but Apache is one of the most common and widely supported.
First, you need to make sure your package list for apt is updated and install some basic network utilities.

```sh
sudo apt update
sudo apt install net-tools
```
This installs some basic networking tools like `ifconfig` and `netstat`.
These are very useful not just for these demos, but also for everyday use.
Note that on many distributions, these will likely already be available.
Next, install the actual webserver software.

```sh
sudo apt install apache2
```

By default, Apache will start running once it is installed.
You can verify it is running with the command
```sh
service apache2 status
```
This should contain a line with the text `active (running)`.

You can verify the webserver is listening on the correct port with
```sh
netstat -atn
```
This should contain a line like `tcp6 0 0 :::80 :::* LISTEN`.
This means it is listening for incoming traffic from the internet on port 80, the regular port for Web requests.
(Note that `tcp6` actually means it is listening on the newer IPv6 protocol, but Apache will accept regular IPv4 traffic also.)

When Apache installs, it will by default create a new directory, `/var/www/html`, as your webroot.
You can change this if you want, but for simplicity, all the demos assume web files will be placed under this directory.
More specifically, they will be placed in the subdirectory `/var/www/html/wasmdemo`.
Then under that directory, when a demo is built, it will create a subdirectory named `wasmdemoXX`, where XX is the demo number, and place all the web files in there.
If you wish to use a different webroot, just change the value for the `WEBROOT` variable in the Makefile for the demo.

IMPORTANT! Be sure to set the correct permissions for the webroot directory before trying to use it.
You must have write permission in order to create files in the webroot, which must be done in these demos.
The Apache installation will by default leave the permissions on the directory as the same as what is on `/var`.
This means it will probably set it to be owned by root with write permission only for the owner, not group.
The easiest way to fix this is to change the group owner of `/var/www` and give it write permission with these commands
```sh
sudo chown -R :ubuntu /var/www
sudo chmod -R g+w /var/www
```
You can use whatever group you are a member of instead of `ubuntu` if you like.
Just avoid giving the directory write permission for everybody or working as root.
These are considered very bad security practices for Linux.

### Setup the Build Environment
In this section we install the tools needed for compiling and building programs.

```sh
sudo apt install gcc
sudo apt install make
sudo apt install git
sudo apt install clang
sudo apt install cmake
```

Note that most distributions come with git, and many also come with gcc and make installed, so you might not have to install those.
The clang package, however, is most likely not on your system to start.
You can quickly determine if a package is already installed with `apt list [packagename]`.
Ultimately, if a package is not installed, you will get the error `command not found` when you try to run it.

You can use whatever editor you want to modify source files if you need, but this demo was built using MS Visual Studio Code.
Some extra instructions are given for if you are using VSCode.
If you use some other IDE, you may encounter similar problems to those we encountered with VSCode but with slightly different fixes.
To install VSCode on Kubuntu, first download the `.deb` package file from Microsoft here: [Install VSCode on Linux](https://code.visualstudio.com/docs/setup/linux#_install-vs-code-on-linux).
Then run the commands
```sh
cd [DownloadDirectory]
sudo apt install ./code_x.xxx.x-xxxxxxxxxx_amd64.deb
```
Where the `x`'s are replaced with whatever version number you downloaded.
On Kubuntu, `[DownloadDirectory]` is often `~/Downloads`.

Last, you need to download the demo files by cloning the git repository.
Assuming you put repositories under the `src` directory in your home directory, run these commands
```sh
cd ~/src
git clone https://github.com/andercy/wasmdemo.git
```
If you are using VSCode, you can then open the entire wasmdemo workspace by opening the file `wasmdemo.code-workspace` under File>Open Workspace.

That's it! Once you are at this point, you are ready to start building the demos.
Just start at 00_helloworldcpp and work your way up numerically.
Always read the README file first then the Makefile then follow the instructions for Building and Running.
The source files are loaded with comments to help you understand more about how WASM works.

Keep in mind that this project is currently in progress and new demos are being added regularly.

Feel free to modify any code you want and do all the experimentation you like, but do not distribute this project with any modifications.

## References
The following sites were invaluable resources for making this project.
I highly recommend checking these out for more information.

* [Mozilla Developer Network WebAssembly](https://developer.mozilla.org/en-US/docs/WebAssembly) This is a good starting point for anyone interested in starting with WebAssembly and also contains some in depth reference material.
* [Clang Site](https://clang.llvm.org/) This is the official site for the clang compiler. It is interesting to learn more about Clang, but it is not specifically about WASM.
* [Emscripten Site](https://emscripten.org/docs/getting_started/Tutorial.html) Most WebAssembly projects use emscripten, which uses clang, but for simplicity we just use clang directly. This is still a great site for getting started in WebAssembly.
* [wasi-libc on Github](https://github.com/WebAssembly/wasi-libc) This site is vital for figuring out how to build and use wasi-libc correctly. It allows the use of many libc functions in WASM.
* [Depth First Compiling C to WebAssembly without emscripten](https://depth-first.com/articles/2019/10/16/compiling-c-to-webassembly-and-running-it-without-emscripten/) This site describes how to get started with WASM using only Clang. This is rare, since most tutorials just use emscripten.
* [Lazy Foo Productions](https://lazyfoo.net/tutorials/SDL/index.php) This is an excellent guide on getting started with SDL. SDL is a library for accessing input and output devices at a level closer to hardware. This does not discuss WASM, but we can use SDL from WASM to create graphics output and to get input from the user.
* [Surma Compiling C to WebAssembly](https://surma.dev/things/c-to-webassembly) An interesting site with some in depth information about WASM. It gets into some very deep stuff, like making your own allocation function, which we should not need if we use wasi-libc.
