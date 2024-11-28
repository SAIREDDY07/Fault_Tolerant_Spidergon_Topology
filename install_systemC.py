import subprocess
import os
import sys

def run_command(command):
    """
    Runs a shell command and handles exceptions.
    """
    try:
        print(f"Running: {command}")
        result = subprocess.run(command, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        print(result.stdout)
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error: Command '{command}' failed with exit code {e.returncode}.")
        print(f"Output: {e.output}")
        print(f"Error Output: {e.stderr}")
        return False

def install_systemc():
    """
    Automates the installation of SystemC.
    """
    try:
        print("Starting SystemC installation process...")
        
        # Step 1: Update package lists and install dependencies
        if not run_command("sudo apt update && sudo apt install -y build-essential cmake"):
            raise Exception("Failed to install build tools.")
        
        # Step 2: Download the SystemC source tarball
        systemc_url = "https://accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz"
        if not run_command(f"wget {systemc_url} -O systemc.tar.gz"):
            raise Exception("Failed to download SystemC.")
        
        # Step 3: Extract the tarball
        if not run_command("tar -xzf systemc.tar.gz"):
            raise Exception("Failed to extract SystemC archive.")
        
        # Step 4: Navigate to the extracted directory
        extracted_dir = "systemc-2.3.3"
        if not os.path.exists(extracted_dir):
            raise Exception(f"Extracted directory '{extracted_dir}' not found.")
        os.chdir(extracted_dir)
        
        # Step 5: Create a build directory
        os.makedirs("build", exist_ok=True)
        os.chdir("build")
        
        # Step 6: Configure and build SystemC
        if not run_command("cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/systemc"):
            raise Exception("CMake configuration failed.")
        if not run_command("make -j$(nproc)"):
            raise Exception("Make build failed.")
        
        # Step 7: Install SystemC
        if not run_command("sudo make install"):
            raise Exception("Make install failed.")
        
        print("SystemC installation completed successfully.")
    except Exception as e:
        print(f"Installation failed: {str(e)}")
        sys.exit(1)

if __name__ == "__main__":
    install_systemc()

