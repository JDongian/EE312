#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <algorithm>
#include <vector>
#include <map>

#define INPUT_FILE "input.txt"

struct car_data {
    unsigned int laps;
    int best_lap;
    unsigned int last_time;
};

/* Input file */
FILE* input_stream = fopen(INPUT_FILE, "r");

bool readNum(int& x) {
    return EOF != fscanf(input_stream, "%d", &x);
}

bool comp_score(std::pair<unsigned int, car_data*> a,
                std::pair<unsigned int, car_data*> b) {
    if (a.second->laps != b.second->laps) {
        return a.second->laps < b.second->laps;
    } else {
        return a.second->last_time < b.second->last_time;
    }
}

int main () {
    /* Variable to hold input data. */
    int number_in = 0;
    /* Throwaway variable to remeber the key of the map (input). */
    int curr_id = 0;
    /* Throwaway variable to compute the current lap time. */
    int curr_lap = 0;
    /* Throwaway variable to check if a car is duplicate. */
    car_data* temp_car;
    /* Keep a map of carid->timedata.
     * Update best time, current time, and lap count values as
     * new input comes in. Usually implemented as BST, so:
     * Searching for element    O(log n)
     * Inserting new element    O(log n)
     * The algorithm for the associative container can be swapped out
     * later, probably using a hashmap to improve runtime, but not time
     * complexity. This is due to the sort at the end, which is O(n*log n).
     */
    std::map<unsigned int, car_data*> car_map;
    /* Vector of number of laps and latest time. */
    std::vector< std::pair<unsigned int, car_data*> > scores;
    /* Vector iterator. */
    std::map<unsigned int, car_data*>::iterator it;
    /* Remeber the best time ever. */
    int best_time_ever;
    while (readNum(curr_id)) {
        /* If there is alredy an entry, update. */
        if(car_map.count(curr_id)) {
            ++(car_map.find(curr_id)->second->laps);
            readNum(number_in);
            /* Calculate the time it took for the last lap. */
            curr_lap = number_in - car_map.find(curr_id)->second->last_time;
            /* Assign the current car's best lap to be the minimum. */
            if (car_map.find(curr_id)->second->best_lap == -1) {
                car_map.find(curr_id)->second->best_lap = curr_lap;
            } else {
                car_map.find(curr_id)->second->best_lap =
                    curr_lap < (car_map.find(curr_id)->second->best_lap) ?
                    curr_lap : (car_map.find(curr_id)->second->best_lap);
            }
            car_map.find(curr_id)->second->last_time = number_in;
        /* Insert new car contestant. */
        } else {
            temp_car = (car_data*) malloc (sizeof(car_data));
            /* Cars start out at 0 laps. */
            temp_car->laps = 0; 
            readNum(number_in);
            temp_car->best_lap = -1;
            /* Latest data of the car. */
            temp_car->last_time = number_in; 
            car_map[curr_id] = temp_car;
        }
    }
    /* Initialize the value of the very best time. */
    best_time_ever = car_map.begin()->second->best_lap;
    /* Copy the map into a vector. */
    for(it = car_map.begin(); it != car_map.end(); ++it) {
        std::pair<unsigned int,
                  car_data*> curr_score = std::make_pair(it->first,
                                                         it->second);
        scores.push_back(curr_score);
        /* Update the value of the best time if needed. */
        if (it->second->best_lap < best_time_ever) {
            best_time_ever = it->second->best_lap;
        }
    }
    /* Sort the vector.
     * Approxamately N*log(N) comparisons and up to that many swaps.
     */
    std::sort(scores.begin(), scores.end(), comp_score);
    printf("There are %d cars in the race\n", int(scores.size()));
    /* Car index (placement) */
    int k = 1;
    for(std::vector< std::pair<unsigned int,
                               car_data*> >::iterator itr = scores.begin();
        itr != scores.end(); itr++) {
        /* Make sure to print 'the' for the best score. */
        printf("Position %d was car %d, completing %d laps with %s best lap"
               " of %d.%03d seconds\n",
               k++, itr->first, itr->second->laps,
               (itr->second->best_lap == best_time_ever) ? "the" : "a",
               itr->second->best_lap/1000, itr->second->best_lap%1000);
    }
    return 0;
}
