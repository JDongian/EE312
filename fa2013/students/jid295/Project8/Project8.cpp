#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <algorithm>
#include <vector>
#include <map>

struct car_data {
    unsigned int laps;
    int best_lap;
    unsigned int last_time;
};

FILE* input_stream = fopen("input.txt", "r");

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
    int number_in = 0;
    int curr_id = 0;
    unsigned int curr_lap = 0;
    car_data* temp_car;
    std::map<unsigned int, car_data*> car_map;
    /* Vector of number of laps and latest time. */
    std::vector< std::pair<unsigned int, car_data*> > scores;
    unsigned int best_time_ever;

    //car_map.insert(std::pair<int, const car_data*>(1, 0));
    //car_map.insert(std::pair<int, const car_data*>(1, &temp_car));
    //std::pair<std::map<int, const car_data*>::iterator, bool> ret;
    //ret = car_map.insert(std::pair<int, const car_data*>(1, 0));


    //for (it=car_map.begin(); it!=car_map.end(); ++it) {
    //   printf("%d->%d\n", it->first, it->second);
    //}

    while (readNum(curr_id)) {
        printf("first in: %d\n", curr_id);
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
            printf("UPDATE: %d->{%d, %d, %d}\n", curr_id,
                    car_map.find(curr_id)->second->laps,
                    car_map.find(curr_id)->second->best_lap,
                    car_map.find(curr_id)->second->last_time);
        } else {
            temp_car = (car_data*) malloc (sizeof(car_data));
            temp_car->laps = 0; //New cars have completed at least 1 lap.
            readNum(number_in);
            printf("in: %d\n", number_in);
            temp_car->best_lap = -1;
            temp_car->last_time = number_in; //Latest data of the car.
            car_map[curr_id] = temp_car;
            printf("INSERT: %d,%d @ %d\n",
                    curr_id, temp_car->last_time, temp_car);
        }
    }
    printf("Input finished.\n");
    std::map<unsigned int, car_data*>::iterator it;
    /* Initialize the value of the very best time. */
    best_time_ever = car_map.begin()->second->best_lap;
    for(it = car_map.begin(); it != car_map.end(); ++it) {
        std::pair< unsigned int, car_data* > curr_score = std::make_pair(it->first, it->second);
        scores.push_back(curr_score);
        if (it->second->best_lap < best_time_ever) {
            best_time_ever = it->second->best_lap;
        }
    }
    std::sort(scores.begin(), scores.end(), comp_score);
    printf("There are %d cars in the race\n", scores.size());
    int k = 1;
    for(std::vector< std::pair<unsigned int, car_data*> >::iterator itr = scores.begin();
        itr != scores.end(); itr++) {
        printf("Position %d was car %d, completing %d laps with %s best lap"
               " of %d.%03d seconds.\n",
               k++, itr->first, itr->second->laps,
               (itr->second->best_lap == best_time_ever) ? "the" : "a",
               itr->second->best_lap/1000, itr->second->best_lap%1000);
    }
    return 0;
}
