# il2cpp-cpp-class-generator
il2cpp-cpp-class-generator is program that generates C++ boilerplate code to make modding Il2Cpp games easier. The boilerplate code basically tries to turn a game's C# class code into C++ equivalent code so that it is easier to access their fields, call methods etc.

# To-do list
- [x] Make a function to detect the Unity version of an Il2cpp game
- [x] Make a file type specific class that holds information about the file and which can convert virtual addresses into offsets.
- [x] Add basic global-metadata.dat related structs into the project 
- [x] Create a function to get the Il2cppMetadataRegistration pointer (necessary for generating C++ classes from Il2cpp data)
- [x] Parse metadata information (such as classes, their fields, methods etc.) into our own organized structs/vectors
- [ ] Put each class into their own folder
- [ ] Generate boilerplate code that will initialize some of the Il2cpp backend functions
- [ ] Generate code to initialize all of the game classes
- [ ] Generate each class into their own file
- [ ] ???
- [ ] Make everything in this list work across multiple platforms

