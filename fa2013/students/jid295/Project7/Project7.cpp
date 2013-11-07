#include <stdio.h>
#include "Bugs.h"
#include "stdlib.h"

#define GENE_COUNT 8

int time_step = 0;
int average_age = 0;
int average_generation = 0;
int percent_straight;
int percent_right;
int percent_left;
int percent_back;
int total_left = 0;
int total_right = 0;
int total_straight = 0;
int total_back = 0;

/* DONE: you don't need to use this function in your solution
 * however this function is used by createWorld to generate random bugs */
Bug createBug(int x, int y) {
    Bug b;
    b.age = 0;
    b.generation = 0;
    b.x = x;
    b.y = y;
    b.dir = rand() % 8;
    b.health = START_HEALTH;
    for (int k = 0; k < 8; k += 1) {
        if (DISTRIBUTED_GENES) {
            b.genes[k] = GENE_TOTAL / 8;
        } else {
            if (k == 0) {
                b.genes[k] = GENE_TOTAL;
            } else {
                b.genes[k] = 0;
            }
        }
    }
    total_straight += b.genes[0];
    total_right += b.genes[1] + b.genes[2] + b.genes[3];
    total_left += b.genes[5] + b.genes[6] + b.genes[7];
    total_back += b.genes[4];
    return b;
}

/* DONE: */
void createWorld(void) {
    /* empty the world */
    for (int i = 0; i < WORLD_SIZE; i += 1) {
        for (int j = 0; j < WORLD_SIZE; j += 1) {
            world[i][j] = EMPTY;
        }
    }

    /* create food */
    for (int k = 0; k < INITIAL_FOOD; k += 1) {
        int x = rand() % WORLD_SIZE;
        int y = rand() % WORLD_SIZE;
        world[x][y] = FOOD;
    }

    /* create bugs */
    while (bug_list.size() < (unsigned int) INITIAL_BUGS) {
        int x = rand() % WORLD_SIZE;
        int y = rand() % WORLD_SIZE;
        if (world[x][y] == EMPTY) {
            world[x][y] = bug_list.size();
            bug_list.push_back(createBug(x, y));
        }
    }	
}

/*
 * DONE: keep in mind that bugs that "fall off" the edge of the world
 * automatically reappear on the opposite edge (i.e, the world is round,
 * well, if you really want to get topographical, our bugs live on a donut.
 */ 
int wrap(int y) {
    if (y < 0) { return WORLD_SIZE - 1; }
    else if (y == WORLD_SIZE) { return 0; }
    else { return y; }
}

/* DONE: you can use this function if you want, it's not required that you use it */
int newX(int x, int dir) {
    switch (dir) {
        case 0: // move up
        case 1:
        case 7:
            return wrap(x - 1);
            break; // not reached
        case 3: // move down
        case 4:
        case 5:
            return wrap(x + 1);
            break; // not reached
        case 2: // move left or right
        case 6:
            return x;
    }
    /* NOT REACHED */
    return 0;
}


/* DONE: you can use this function if you want, it's not required that you use it */
int newY(int y, int dir) {
    switch (dir) {
        case 1:
        case 2:
        case 3:
            return wrap(y + 1);
        case 5:
        case 6:
        case 7:
            return wrap(y - 1);
        case 0:
        case 4:
            return y;
    }
    /* NOT REACHED */
    return 0;
}


/* DONE: 
 * Add NUM_FOOD food objects in random locations
 * if you drop food on a bug, be sure to feed the bug
 */
void addFood(void) {
    for (int food = 0; food < NUM_FOOD; food += 1) {
        int x = rand() % WORLD_SIZE;
        int y = rand() % WORLD_SIZE;
        switch (world[x][y]) {
            case EMPTY: // empty, add the food
                world[x][y] = FOOD;
                break;
            case FOOD: // already has food, do nothing
                break;
            default: // bug, feed the bug
                bug_list[world[x][y]].health += EAT_HEALTH;
        }
    }	

}

int chooseDir(int genes[GENE_COUNT]) {
    /* Roulette selection */
    int choice = rand() % GENE_TOTAL;
    int cum_sum = 0;
    for (int i = 0; i < GENE_COUNT; ++i) {
        cum_sum += genes[i];
        if (choice <= cum_sum) {
            return i;
        }
    }
    return -1;
}
/* TODO... finish this function as described below
 * move bugs 
 * if they move onto food, feed them
 * if they move onto another bug, no big deal.  Two bugs
 * can be in the same square.  Let's say bug#2 and bug #6 are in the same
 * square.  Well, both bugs are still in the bug_list, so they
 * will still be able to move next time step.
 * Since the world[][] array can only hold either a 2 or a 6, we'll just
 * put one of them there.  The easiest thing to do is just put which
 * ever bug moves into the square first into the square.  When the next
 * bug moves into the square, update his x and y (in bug_list) so that
 * he is in the square, leave the other bug alone (i.e., it's also in
 * the same square) and then set world[x][y] = 6 (the bug # of the second
 * bug).  No harm done.  The graphics will draw only the second bug
 * but who cares.  The next time step they'll move into different squares
 * (probably) and no one will notice.
 * NOTE: only the first bug to move into the square gets to eat the food
 * there.
 */
void moveBugs(void) {
    int total_age = 0;
    int total_gen = 0;

    /* update each bug in turn (but don't kill them) */
    for (int k = 0; k < bug_list.size(); ++k) {
        /* Clear the current square if we are the only one here.
         */
        if (world[bug_list[k].x][bug_list[k].y] ==  k) {
            world[bug_list[k].x][bug_list[k].y] = EMPTY;
        }
        /* Update bug position. */
        bug_list[k].x = newX(bug_list[k].x, bug_list[k].dir);
        bug_list[k].y = newY(bug_list[k].y, bug_list[k].dir);
        /* Bug eating behavior. */
        if (world[bug_list[k].x][bug_list[k].y] == FOOD) {
            bug_list[k].health += EAT_HEALTH;
        }
        /* Put the bug in the new spot. */
        world[bug_list[k].x][bug_list[k].y] = k;
        /* Bug movement health cost. */
        bug_list[k].health -= MOVE_HEALTH;
        /* Bug turn based on genes. */
        bug_list[k].dir = (bug_list[k].dir+chooseDir(bug_list[k].genes))%8;
        /* Bug statistics. */
        bug_list[k].age += 1;

        total_age += bug_list[k].age;
        total_gen += bug_list[k].generation;
    }
    average_age = total_age / bug_list.size();
    average_generation = total_gen / bug_list.size();
}

void updateStatistics(int genes[]) {
    total_straight -= genes[0];
    total_right -= genes[1]+genes[2]+genes[3];
    total_left -= genes[5]+genes[6]+genes[7];
    total_back -= genes[4];
}

void killDeadBugs(void) {
    int i = bug_list.size() - 1;
    /* Negative health kills bugs. */
    while (i >= 0) {
        /* Clear the square the bug occupied in the world if we are
         * the only one there.
         */
        if (bug_list[i].health <= 0) {
            updateStatistics(bug_list[i].genes);
            if (world[bug_list[i].x][bug_list[i].y] == i)
                world[bug_list[i].x][bug_list[i].y] = EMPTY;
            bug_list[i] = bug_list.back();
            if (i != bug_list.size() - 1) {
                world[bug_list.back().x][bug_list.back().y] = i;
            }
            bug_list.pop_back();
        }
        --i;
    }

}

void mutateGenes(int genes[GENE_COUNT]) {
    int i = 0;
    int choice = 0;
    /* Check non-zero genes. */
    for (i = 0; i < GENE_COUNT; ++i) {
        if (genes[i]) {
            ++choice;
        }
    }
    /* Decrement random gene with equal chances. */
    i = 0;
    choice = rand() % choice;
    while (choice > 0 || genes[i] == 0) {
        if (genes[i] != 0) {
            --choice;
        }
        ++i;
    }
    --genes[i];
    /* Increment random gene. */
    ++genes[rand() % GENE_COUNT];
}

/* Look at all the bugs in the bug_list.  For each bug
 * that has enough health to reproduce, reproduce a new bug.
 * Put the new bug in the same square and have it head in the
 * same direction.
 *
 * Give both the new bug and the old bug 1/2 the energy of the
 * old bug.
 *
 * Mutate the genes of the new bug as follows.  Choose two genes
 * at random.  Increment one of those genes (by one) and decrement
 * the other gene (by one).
 * YOU MUST ensure 
 * 1. that no genes ever get smaller than zero
 * 2. the total of the eight genes is always GENE_TOTAL
 *
 * Be sure to put the new bug in the bug_list (no need to update
 * the world, since you have two bugs in the same square anyway).
 */
void reproduceBugs(void) {
    int num_bugs = bug_list.size();
    for (int k = 0; k < num_bugs; k += 1) {
        if (bug_list[k].health > REPRODUCE_HEALTH) {
            /* Make the new bug. */
            Bug b = bug_list[k];
            b.health /= 2;
            b.generation++;
            b.age = 0;
            mutateGenes(b.genes);
            bug_list[k].health /= 2;

            /* Update global statistics based on the new genes in b */
            total_straight += b.genes[0];
            total_right += b.genes[1]+b.genes[2]+b.genes[3];
            total_left += b.genes[5]+b.genes[6]+b.genes[7];
            total_back += b.genes[4];

            /* Add b to the end of the global bug list */
            bug_list.push_back(b);
        }
    }	
}

void timeStep() {
    time_step += 1;
    addFood();
    moveBugs();
    killDeadBugs();
    reproduceBugs();
    percent_straight = 100 * total_straight / bug_list.size() / GENE_TOTAL;
    percent_left = 100 * total_left / bug_list.size() / GENE_TOTAL;
    percent_right = 100 * total_right / bug_list.size() / GENE_TOTAL;
    percent_back = 100 * total_back / bug_list.size() / GENE_TOTAL;
}	
