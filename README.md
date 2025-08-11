# Numerical Approximator

This small numerical calculator is an updated version of an old university project I did back in 2018. The original project was written in C# using Windows Forms and served as a launcher for a series of console apps written in C. The apps performed various calculus operations described in “Numerical Methods for Engineers” by Steven Chapra and Raymond Canale. In the new version, I rewrote all the calculators in C++, keeping the original logic unchanged wherever possible. The new GUI uses the Qt6 library and mimics the original console-style input and output.

All files are uploaded *as is*, together with the VS solution.

To regenerate the VS project:
- clone the repository
- delete the root/out/build sub-folder
- run createvsproject.bat
- run deploydlls.bat

To run the app build:
- pull the root\out\build\code\application\Debug folder
- run application.exe
