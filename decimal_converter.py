import csv

# Input and output file names
input_file = 'output.csv'
output_file = 'data_rounded.csv'

try:
    with open(input_file, mode='r') as infile:
        with open(output_file, mode='w', newline='') as outfile:
            reader = csv.reader(infile)
            writer = csv.writer(outfile)
            
            # Initialize a counter
            row_index = 0
            
            for row in reader:
                if row:  # Skip empty rows
                    if row_index % 500 == 0:  # Take only every 25th row
                        # Process and round each non-empty cell
                        rounded_row = [
                            round(float(value), 1) for value in row if value.strip()
                        ]
                        writer.writerow(rounded_row)
                
                # Increment the row index
                row_index += 1

    print(f"Filtered and rounded data has been saved to {output_file}")
except FileNotFoundError:
    print(f"Error: The file {input_file} was not found.")
except ValueError as e:
    print(f"Error processing the data: {e}")


