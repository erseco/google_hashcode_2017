import sys
import numpy as np
from deap import base, creator, tools
from random import randint

from deap import algorithms
from deap import base
from deap import creator
from deap import tools

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

            i = 0
            # Load the pizza
            for line in file:
                self.matrix[i] = np.array(list(line)[:-1])
                i+=1


    """
    Removes overlaping slices in one individual
    """
    def overlap(self, individual):
        # create an occupation matrix
        occup = np.zeros((self.number_of_rows, self.number_of_columns), dtype=bool)
        to_remove = []
        for i in range(len(individual)):
            slice = individual[i]
            if (occup[slice[0]:slice[2], slice[1]:slice[3]] == False).all():
                occup[slice[0]:slice[2], slice[1]:slice[3]] = True
            else:
                to_remove.append(i)

        for i in range(len(to_remove)):
            del individual[to_remove[i] - i]

        print(individual)


    def evaluate(self, individual):

        pizza_points = 0

        self.overlap(individual)

        for item in individual:

            slice_points = 0

            number_of_mushroms = 0
            number_of_tomatos = 0

            for r in range(item[0],item[2]):
                for c in range (item[1],item[3]):
                    if self.matrix[r][c] == 'M':
                        number_of_mushroms += 1
                    elif self.matrix[r][c] == 'T':
                        number_of_tomatos += 1

            if number_of_mushroms >= self.minimum_of_each_ingredient_per_slice \
               and number_of_tomatos >= self.minimum_of_each_ingredient_per_slice:
                slice_points = sum(item)

            pizza_points += slice_points


        return pizza_points,

    def mutate(self, individual):

        # TO-DO

        return individual,


    # generate a random slice
    def generate_rand_slice(self):
        nelems = np.Infinity
        while nelems > self.maximum_of_cells_per_slice:
            r1 = randint(0, self.number_of_rows-2)
            r2 = randint(r1+1, self.number_of_rows-1)
            c1 = randint(0, self.number_of_columns-2)
            c2 = randint(c1+1, self.number_of_columns-1)
            # check the slice generated is not bigger than maximum size
            nrows = (r2 - r1)+1
            ncols = (c2 - c1)+1
            nelems = nrows*ncols

        return  r1, c1, r2, c2


if __name__ == '__main__':

    if len(sys.argv) < 2:
        sys.exit('Usage: %s <pizza file name>' % sys.argv[0])

    pizza = Pizza(sys.argv[1])
    # print(pizza.matrix)

    # creating types
    creator.create("FitnessMax", base.Fitness, weights=(1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMax)

    # initialize algorithm: invididuals and population
    IND_INIT_SIZE=pizza.number_of_rows*pizza.number_of_columns
    toolbox = base.Toolbox()
    toolbox.register("attribute", pizza.generate_rand_slice)
    toolbox.register("individual", tools.initRepeat, creator.Individual, 
                     toolbox.attribute, IND_INIT_SIZE)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", pizza.evaluate)
    toolbox.register("mate", tools.cxTwoPoint)
    toolbox.register("mutate", pizza.mutate)
    toolbox.register("select", tools.selBest) # use the pre-set operators
    #toolbox.register("select", tools.selNSGA2) # use the pre-set operators

    population = toolbox.population(n=100)
    algorithms.eaSimple(population, toolbox, cxpb=0.5, mutpb=0.2, ngen=50)

