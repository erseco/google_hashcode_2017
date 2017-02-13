import sys
import numpy as np
from deap import base, creator, tools
from random import randint

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
            # Read the parameters of the pizza
            header = file.readline().split(' ')
            # and set up the pizza
            self.number_of_rows = int(header[0])
            self.number_of_columns = int(header[1])
            self.minimum_of_each_ingredient_per_slice = int(header[2])
            self.maximum_of_cells_per_slice = int(header[3])

            self.matrix = np.zeros((self.number_of_rows, self.number_of_columns), dtype=np.character)

            i = 0
            # Load the pizza
            for line in file:
                self.matrix[i] = np.array(list(line)[:-1])
                i+=1

    def evaluate(self, individual):
        return sum(individual),

    # generate a random slice
    def generate_rand_slice(self):
        r1 = randint(0, self.number_of_rows-1)
        r2 = randint(r1, self.number_of_rows)
        c1 = randint(0, self.number_of_columns-1)
        c2 = randint(c1, self.number_of_columns)
        return  r1, c1, r2, c2


if __name__ == '__main__':

    if len(sys.argv) < 2:
        sys.exit('Usage: %s <pizza file name>' % sys.argv[0])

    pizza = Pizza(sys.argv[1])
    print(pizza.matrix)

    # creating types
    creator.create("FitnessMax", base.Fitness, weigths=(1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMax)

    # initialize algorithm: invididuals and population
    IND_SIZE=10
    toolbox = base.Toolbox()
    toolbox.register("attribute", pizza.generate_rand_slice)
