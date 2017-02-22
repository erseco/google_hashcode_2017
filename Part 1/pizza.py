import sys
import numpy as np
from deap import base, creator, tools, algorithms
from random import randint

from math import ceil

class Pizza:

    # Constructor
    def __init__(self, file_name):
        # Load the file and construct the matrix
        self.load_file(file_name)


    # Methods
    def print_info(self):
        print("Rows: " + str(self.number_of_rows))
        print("Columns: " + str(self.number_of_columns))

    def print_results(self):
        print("FINISH THIS")
        #3          3 slices.
        #0021       First slice between rows (0,2) and columns (0,1).
        #0222       Second slice between rows (0,2) and columns (2,2).
        #0324       Third slice between rows (0,2) and columns (3,4).

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

            # self.bool_matrix = np.full((self.number_of_rows, self.number_of_columns), False, dtype=np.bool)

            i = 0
            # Load the pizza
            for line in file:
                self.matrix[i] = np.array(list(line)[:-1])
                i+=1

    def evaluate(self, individual):
        pizza_points = 0
        nelems = 0
        slice_points = 0
        
        for item in individual:
            # Unpack the index
            r1, c1, r2, c2 = item
            print("Cachos de pizza:", item)
            # Get the size of the slice
            nelems += ((r2 - r1) + 1) * ((c2 - c1) + 1)

            n_tomatos = len(np.where(self.matrix[r1:r2+1, c1:c2+1] == b'T'))
            n_mushroms = len(np.where(self.matrix[r1:r2+1, c1:c2+1] == b'M'))

            if n_mushroms >= self.minimum_of_each_ingredient_per_slice \
                and n_tomatos >= self.minimum_of_each_ingredient_per_slice:
                slice_points += (n_tomatos+n_mushroms)

        print("Puntos de los cachos:", slice_points)
        print("Cachos en blanco",self.matrix.size - nelems)
        pizza_points += slice_points*(self.matrix.size - nelems)
        print("Puntos de la pizza:", pizza_points)
        return pizza_points,

    def generate_rand_slices(self, n_iter):
        bool_matrix = np.zeros((self.number_of_rows, self.number_of_columns),
                               dtype=bool)
        pizza_slices = []
        for i in range(n_iter):
            rand_slice = self.generate_rand_slice(bool_matrix)
            if rand_slice[0] != -1:
                pizza_slices.append(rand_slice)

        return pizza_slices

    # generate a random slice
    def generate_rand_slice(self, bool_matrix, max_attemps=50):
        not_have_enough_ingrs = True
        not_taken = True
        more_than_max_cells = True
        n_attemps = 0
        while (more_than_max_cells or not_have_enough_ingrs or not_taken):
            n_attemps += 1
            not_have_enough_ingrs = True
            not_taken = True
            more_than_max_cells = True
            r1 = randint(0, self.number_of_rows - 2)
            r2 = randint(r1 + 1, self.number_of_rows - 1)
            c1 = randint(0, self.number_of_columns - 2)
            c2 = randint(c1 + 1, self.number_of_columns - 1)
            # check the slice generated is not bigger than maximum size
            nrows = (r2 - r1) + 1
            ncols = (c2 - c1) + 1
            nelems = nrows * ncols

            n_tomatos = len(np.where(self.matrix[r1:r2+1, c1:c2+1] == b'T'))
            n_mushroms = len(np.where(self.matrix[r1:r2+1, c1:c2+1] == b'M'))

            if n_mushroms >= self.minimum_of_each_ingredient_per_slice and \
                            n_tomatos >= self.minimum_of_each_ingredient_per_slice:
                not_have_enough_ingrs = False

            if (bool_matrix[r1:r2+1, c1:c2+1] == False).all():
                not_taken = False
                bool_matrix[r1:r2+1, c1:c2+1] = True

            if nelems <= self.maximum_of_cells_per_slice:
                more_than_max_cells = False

            if n_attemps > max_attemps:
                break

        else:
            return r1, c1, r2, c2

        return -1,


    def mutate(self, individual):
        # contract = randint(0,3)
        # slice= randint(0, len(individual))
        # if contract == 1:
        #     # Eliminate the upper row of the slice
        #     self.bool_matrix[individual[slice][0], individual[slice][1]:individual[slice][3]+1] = False
        #     a, b, c, d = individual[slice]
        #     individual[slice] = (a+1, b, c, d)

        # elif contract == 2:
        #     # Eliminate the last row of the slice
        #     self.bool_matrix[individual[slice][2], individual[slice][1]:individual[slice][3] + 1] = False
        #     a, b, c, d = individual[slice]
        #     individual[slice] = (a, b, c-1, d)

        # elif contract == 3:
        #     # Eliminate the left column of the slice
        #     self.bool_matrix[individual[slice][0]:individual[slice][2] + 1, individual[slice][1]] = False
        #     a, b, c, d = individual[slice]
        #     individual[slice] = (a, b+1, c, d)

        # else:
        #     # Eliminate the right column of the slice
        #     self.bool_matrix[individual[slice][0]:individual[slice][2] + 1, individual[slice][3]] = False
        #     a, b, c, d = individual[slice]
        #     individual[slice] = (a, b, c, d-1)
        return individual,

    def mate(self, ind1, ind2):
        # TO-DO
        return ind1, ind2

if __name__ == '__main__':

    if len(sys.argv) < 2:
        sys.exit('Usage: %s <pizza file name>' % sys.argv[0])

    pizza = Pizza(sys.argv[1])
    # print(pizza.matrix)

    # creating types
    creator.create("FitnessMax", base.Fitness, weights=(1.0,))
    creator.create("Individual", set, fitness=creator.FitnessMax)

    # initialize algorithm: invididuals and population
    IND_INIT_SIZE=ceil(pizza.matrix.size/pizza.maximum_of_cells_per_slice)*100
    toolbox = base.Toolbox()
    toolbox.register("attribute", pizza.generate_rand_slices, IND_INIT_SIZE)
    toolbox.register("individual", tools.initIterate, creator.Individual, 
                     toolbox.attribute)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", pizza.evaluate)
    toolbox.register("mate", pizza.mate)
    toolbox.register("mutate", pizza.mutate)
    toolbox.register("select", tools.selBest) # use the pre-set operators
    #toolbox.register("select", tools.selNSGA2) # use the pre-set operators

    population = toolbox.population(n=5)
    print(population)
    algorithms.eaSimple(population, toolbox, cxpb=0.5, mutpb=0.2, ngen=50)

