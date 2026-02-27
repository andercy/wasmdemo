# WASM Demo Programs
by Cy Anderson

## Introduction
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

## Environment
WebAssembly runs on an array of platforms. That is one of its main advantages.
For simplicity and ease of reproduction, these demos were developed primarily on Kubuntu Linux 24.04.3.
They have been simultaneously devloped on an AWS EC2 instance running Ubuntu 24.
Instructions assume you are familiar with Ubuntu or another Debian flavor of Linux, but they should work similarly for other distributions with only small modifications.
These demos should also be able to work on Windows, but it easier to show exact command line steps for setup on Linux.

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
You can change this if you want, but for simplicity, all the demos assume webfiles will be placed under this directory.
More specifically, they will be placed in the subdirectory `/var/www/html/wasmdemo`.
Then under that directory, when a demo is built, it will create a subdirectory named `demoXX`, where XX is the demo number, and place all the web files in there.
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
```

Note that most distributions come with git, and many also come with gcc and make installed, so you might not have to install those.
The clang package, however, is most likely not on your system to start.
You can quickly determine if a package is already installed with `apt list [packagename]`.
Ultimately, if a package is not installed, you will get `command not found` error when you try to run it.

You can use whatever editor you want to modify source files if you need, but this demo was built using MS Visual Studio Code.
Some instructions are given for if you are using VSCode.
If you use some other IDE, you may encounter similar problems but with slightly different fixes.
To install VSCode on KUbuntu, first download the `.deb` package file from Microsoft here: [Install VSCode on Linux](https://code.visualstudio.com/docs/setup/linux#_install-vs-code-on-linux).
Then run the commands
```sh
cd [DownloadDirectory]
sudo apt install ./code_x.xxx.x-xxxxxxxxxx_amd64.deb
```
Where the `x`'s are replaced with whatever version number you downloaded.
On KUbuntu, `[DownloadDirectory]` is often `~/Downloads`.

Last, you need to download the demo files by cloning the git repository.
Assuming you put repositories under the `src` directory in your home directory, run this command
```sh
git clone https://github.com/andercy/wasmdemo.git
```
If you are using VSCode, you can then open the entire wasmdemo workspace by opening the file `wasmdemo.code-workspace` under File>Open Workspace.

That's it! Once you are at this point, you are ready to start building the demos.
Just start at 00_helloworldcpp and work your way up numerically.
Always read the README file first then the Makefile then follow the instructions for Building and Running.

Keep in mind that this project is currently in progress and new demos are being added regularly.

Feel free to modify any code you want and do all the testing you like, but do not distribute this project with any modifications.
