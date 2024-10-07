def print_file_lines(file_path):
    try:
        with open(file_path, 'r') as file:
            for line in file:
                print(line.strip())  # Use strip() to remove any extra newline characters
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage:
if __name__ == "__main__":
    file_path = "config/traffic/tile-0"  # Replace with the path to your text file
    print_file_lines(file_path)

