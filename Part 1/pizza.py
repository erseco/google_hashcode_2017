import sys
import numpy as np

class Pizza:

    # Constructor
    def __init__(self, file_name):
        # Load the file and construct the matrix
        self.load_file(file_name)


    # Methods
    def print_info(self):
        print("Rows: " + str(self.number_of_rows))
        print("Columns: " + str(self.number_of_columns))


    def load_file(self, file_name):

        with open(file_name) as file:

            header = file.readline().split(' ')

            self.number_of_rows = int(header[0])
            self.number_of_columns = int(header[1])
            self.minimum_of_each_ingredient_per_slice = int(header[2])
            self.maximum_of_cells_per_slice = int(header[3])

            self.matrix = np.zeros((self.number_of_rows, self.number_of_columns), dtype=np.character)

            i = 0

            for line in file:
                a = np.array(list(line)[0:len(line)-1], dtype=np.character)
                self.matrix[i] = a
                i+=1


if __name__ == '__main__':

    if len(sys.argv) < 2:
        sys.exit('Usage: %s <pizza file name>' % sys.argv[0])

    pizza = Pizza(sys.argv[1])