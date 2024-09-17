### Build and Run the Application

Visual Studio 2022.

1. **Build the Spindle Project**:
   - Right-click on **Spindle** and select **Build**.
   - Ensure it compiles without errors.

2. **Build the Sandbox Project**:
   - Right-click on **Sandbox** and select **Build**.
   - It should link against **Spindle** successfully.

3. **Copy the DLL**: (TODO: automate this.)
   - After building, copy the `Spindle.dll` from the Spindle's output directory to the Sandbox's output directory.

4. **Run the Sandbox Application**:
   - Set **Sandbox** as the startup project.
   - Run the application (press **F5**).
   - The application should start and execute the `Run` method from the **Spindle**.