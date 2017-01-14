# How to build

First either clone this repository with the command `git clone https://github.com/MitchellHansen/cs-445` or by downloading through github.

Next, you're going to need to get CMake to build the project. It can be found [here](https://cmake.org/download/). 
You can download the msi to install it, or the zip if you want a portable version.

Now you're also going to need to grab the SFML binaries [here](http://www.sfml-dev.org/download/sfml/2.4.1/).
You need to download the file for your version of Visual Studio, make sure that it is also the 64 bit version.
Unzip that wherever you want. I'll put it on my desktop.

With everything downloaded, open up the cmake-gui app and start filling in the info.

Where is the source code? That is going to be the path to your cs-445 folder that you cloned from github.
`C:/Users/yourname/Desktop/cs-445`

Where to build the binaries? That is just going to be the folder you previously pointed too with /build on top.
`C:/Users/yourname/Desktop/cs-445/build`

Now, hit the configure button and it'll ask to specify a generator for the project. Select your version
of Visual Studio. Mine is 2015 so I selected `Visual Studio 14 2015 Win64`. Make sure you use the Win64
ones. Hit finish.

Cmake will pop an error about invalid values or something. The `SFML_ROOT` name will have the value `root`, set that value to
the path where your SFML files are located

`C:\Users\yourname\Desktop\SFML-2.4.1-windows-vc14-64-bit\SFML-2.4.1`

Hit configure again and there should no longer be any errors.

Your output should look like this

```
Found SFML 2.4.1 in C:\Users\yourname\Desktop\SFML-2.4.1-windows-vc14-64-bit\SFML-2.4.1\include
SFML found:TRUE
Configuring done
```

Now hit the generate button and you'll find your visual studio solution file in
`C:\Users\mrh\Desktop\cs-445\build` or wherever you cloned the repo to.

We're almost there, open the `cs-445-labs.sln` file in /build. In the solution explorer you will see
```
ALL_BUILD
cs-445-labs
ZERO_CHECK
```

Right click `cs-445-labs` and select `SetAsStartUpProject`

Run the program and the program will crash complaining about .dll's. We solve this by copy and pasting the
dll files found in `C:\Users\yourname\Desktop\SFML-2.4.1-windows-vc14-64-bit\SFML-2.4.1\bin` and putting them
in `C:\Users\yourname\Desktop\cs-445\build\Debug`. Run the program again and everything should work!
