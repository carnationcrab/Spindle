### Build and Run the Application

Requires Visual Studio 2022.

1. **Run `GENERATEPROJECTS.bat` to build projects**:
   - Double-click `RunProjects.bat`. This will run the Premake script in `premake5.lua` and generate the necessary project files for Visual Studio 2022.

2. **Copy the DLL**:
   - DLL copying is now automated by Premake. You do not need to manually copy any DLLs; the build process in Visual Studio will take care of this as long as you've run the Premake script. 

3. **Run the Sandbox Application**:
   - Set **Sandbox** as the startup project (this should be done automatically by Premake).
   - Run the application by pressing **F5** in Visual Studio.
   - The application should start and execute the `Run` method from **Spindle**.

-- last updated SEPT 18 2024 --