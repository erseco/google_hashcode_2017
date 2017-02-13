#!/usr/bin/env python

import sys
import numpy as np

class Pizza:

    # Constructor
    def __init__(self, rows, columns, minimum, maximum):
        self.number_of_rows = int(rows)
        self.number_of_columns = int(columns)
        self.minimum_of_each_ingredient_per_slice = int(minimum)
        self.maximum_of_cells_per_slice = int(maximum)
        # self.matrix = [[0 for i in xrange(self.number_of_rows)] for i in xrange(self.number_of_columns)]
        self.matrix = np.zeros((self.number_of_rows, self.number_of_columns))


    # Methods
    def print_info(self):
        print("Rows: " + str(self.number_of_rows))
        print("Columns: " + str(self.number_of_columns))


with open(sys.argv[1], 'r') as file:

    header = file.readline().split(' ')

    pizza = Pizza(header[0], header[1], header[2], header[3])

    print(file.read())

    pizza.print_info()



