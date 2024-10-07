'''import os
print('changing tiles...')
for i in range(1,64):
	os.system('cp tile-0 tile-'+str(i))
	print(i)
print('done...')'''
import os

# Full path to the directory containing tile-0
tile_directory = 'config/traffic/'  # Replace this with the actual path

# Full path to tile-0
source_file = os.path.join(tile_directory, 'tile-0')

# Check if tile-0 exists
if not os.path.exists(source_file):
    print(f"Error: '{source_file}' does not exist.")
else:
    print('Changing tiles...')
    for i in range(1, 64):
        destination_file = os.path.join(tile_directory, f'tile-{i}')
        os.system(f'cp {source_file} {destination_file}')
        
    print('Done...')
