// Snakebot2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <string>
#include <fstream>
using namespace std;
int generations;
float multiplier[200][4][32][518], parentmultiplier[200][4][32][518];
float bias[200][4][32], parentbias[200][4][32];
float value[5][518];
int x = 9, y = 7, att = 0, d = 0, foodxp[260], foodyp[260], foodx, foody, aid = 0, score = 0, 
timer = 0, dmin = 15, dstart, gen = 0, tries = 5, highscore = 0, mutationrate = 988;
float totalfit, top;
const int gensize = 200;
int snakex[255];
int snakey[255];
float fitness[gensize];
int exitpoint;
string file_path1 = "weights.txt";
string file_path2 = "biases.txt";
double timer2 = 0.0;

void randomize(int a) {
    fitness[a] = 0;
    for (int i = 0; i < 32; i++){
        bias[a][0][i] = (rand() / double(RAND_MAX)) - 0.5;
        for (int ii = 0; ii < 518; ii++) {
            multiplier[a][0][i][ii] = (rand() / double(RAND_MAX)) - 0.5;
        }
    }
    for (int i = 0; i < 16; i++) {
        bias[a][1][i] = (rand() / double(RAND_MAX)) - 0.5;
        for (int ii = 0; ii < 32; ii++) {
            multiplier[a][1][i][ii] = (rand() / double(RAND_MAX)) - 0.5;
        }
    }
    for (int i = 0; i < 8; i++) {
        bias[a][2][i] = (rand() / double(RAND_MAX)) - 0.5;
        for (int ii = 0; ii < 16; ii++) {
            multiplier[a][2][i][ii] = (rand() / double(RAND_MAX)) - 0.5;
        }
    }
    for (int i = 0; i < 4; i++) {
        bias[a][3][i] = (rand() / double(RAND_MAX)) - 0.5;
        for (int ii = 0; ii < 8; ii++) {
            multiplier[a][3][i][ii] = (rand() / double(RAND_MAX)) - 0.5;
        }
    }
}

void food() {
    for (int i = 0; i < 260; i++) {
        foodxp[i] = (rand() / (double(RAND_MAX) + 1)) * 17 + 1;
        foodyp[i] = (rand() / (double(RAND_MAX) + 1)) * 15 + 1;
    }
}

void sort(float a, float b, int c) {
    for (int ii = 0; ii < gensize; ii++) {
        totalfit -= fitness[ii];
        if (totalfit / a < b) {
            copy(&multiplier[ii][0][0][0], &multiplier[ii][0][0][0] + 51290, &parentmultiplier[c][0][0][0]);
            copy(&bias[ii][0][0], &bias[ii][0][0] + 100, &parentbias[c][0][0]);
            return;
        }
    }
    copy(&multiplier[gensize-1][0][0][0], &multiplier[gensize-1][0][0][0] + 51290, &parentmultiplier[c][0][0][0]);
    copy(&bias[gensize-1][0][0], &bias[gensize-1][0][0] + 100, &parentbias[c][0][0]);
}

void selection() {
    float bestmultiplier[4][32][518];
    float bestbias[4][32];
    copy(&multiplier[distance(fitness, max_element(begin(fitness), end(fitness)))][0][0][0], 
        &multiplier[distance(fitness, max_element(begin(fitness), end(fitness)))][0][0][0] + 51290, 
        &bestmultiplier[0][0][0]);
    copy(&bias[distance(fitness, max_element(begin(fitness), end(fitness)))][0][0],
        &bias[distance(fitness, max_element(begin(fitness), end(fitness)))][0][0] + 100,
        &bestbias[0][0]);
    totalfit = 0;
    for (int i = 0; i < gensize; i++) {
        totalfit += fitness[i];
    }
    float normalizer = totalfit;
    for (int i = 0; i < gensize; i++) {
        totalfit = normalizer;
        float randnum = (rand() / double(RAND_MAX));
        sort(normalizer, randnum, i);   
    }
    for (int i = 0; i < gensize / 10; i++) {
        for (int ii = 0; ii < 5; ii++) {
            int jj = (rand() / (double(RAND_MAX) + 1)) * 51290;
            int jjj = (rand() / (double(RAND_MAX) + 1)) * 100;
            //different layers are different sizes, must copy one by one ([0], [1], [2], [3])
            //do the same in other parts as well
            copy(&parentmultiplier[10*i+2*ii][0][0][0], &parentmultiplier[10*i+2*ii][0][0][0] + jj, 
                &multiplier[10*i+2*ii][0][0][0]);
            copy(&parentmultiplier[10 * i + 2 * ii + 1][0][0][0] + jj + 1, &parentmultiplier[10*i+2*ii+1][0][0][0] + 51290,
                &multiplier[10 * i + 2 * ii][0][0][0] + jj + 1);
            copy(&parentmultiplier[10 * i + 2 * ii + 1][0][0][0], &parentmultiplier[10 * i + 2 * ii + 1][0][0][0] + jj,
                &multiplier[10 * i + 2 * ii + 1][0][0][0]);
            copy(&parentmultiplier[10 * i + 2 * ii][0][0][0] + jj + 1, &parentmultiplier[10 * i + 2 * ii][0][0][0] + 51290,
                &multiplier[10 * i + 2 * ii + 1][0][0][0] + jj + 1);
            copy(&parentbias[10 * i + 2 * ii][0][0], &parentbias[10 * i + 2 * ii][0][0] + jjj,
                &bias[10 * i + 2 * ii][0][0]);
            copy(&parentbias[10 * i + 2 * ii + 1][0][0] + jjj + 1, &parentbias[10 * i + 2 * ii + 1][0][0] + 100,
                &bias[10 * i + 2 * ii][0][0] + jjj + 1);
            copy(&parentbias[10 * i + 2 * ii + 1][0][0], &parentbias[10 * i + 2 * ii + 1][0][0] + jjj,
                &bias[10 * i + 2 * ii + 1][0][0]);
            copy(&parentbias[10 * i + 2 * ii][0][0] + jjj + 1, &parentbias[10 * i + 2 * ii][0][0] + 100,
                &bias[10 * i + 2 * ii + 1][0][0] + jjj + 1);
        }
    }
    for (int i = 1; i < gensize; i++) {
        //n mutations happens if chance of n mutations > (rand() / double(RAND_MAX)
        //Since RAND_MAX = 32767, if probability of n mutations is less than 1/32767, we ignore those possibilities
        double rand1 = (rand() / double(RAND_MAX));
        int mutations1 = int(rand1 < 0.0314) + int(rand1 < 0.000493);
        int previousindex1 = -1;
        for(int ii = 0; ii < mutations1; ii++){
            int index = static_cast<int>((rand() / (double(RAND_MAX)+0.1)))*(32-ii);
            if(index >= previousindex1){
                index++;
            }
            bias[i][0][index] = (rand() / double(RAND_MAX)) - 0.5;
            previousindex1 = index;
        }
        for (int ii = 0; ii < 32; ii++) {
            double rand2 = (rand() / double(RAND_MAX));
            int mutations2 = int(rand2 < 0.311) + int(rand2 < 0.0813) + int(rand2 < 0.0142) + int(rand2 < 0.00185) + int(rand2 < 0.000193);
            int previousindex2[5] = {-1, -1, -1, -1, -1};
            for(int iii = 0; iii < mutations2; iii++){
                int index = static_cast<int>((rand() / (double(RAND_MAX)+0.1)))*(518-iii);
                for(int iv = 0; iv < iii; iv++){
                    if(index >= previousindex2[iv]){
                        index++;
                    }
                }
                multiplier[i][0][ii][index] = (rand() / double(RAND_MAX)) - 0.5;
                previousindex2[iii] = index;
            }
        }
        for (int ii = 0; ii < 16; ii++) {
            if ((rand() / double(RAND_MAX)) * mutationrate < 1.0) {
                bias[i][1][ii] = (rand() / double(RAND_MAX)) - 0.5;
            }
            for (int iii = 0; iii < 32; iii++) {
                if ((rand() / double(RAND_MAX)) * mutationrate < 1.0) {
                    multiplier[i][1][ii][iii] = (rand() / double(RAND_MAX)) - 0.5;
                }
            }
        }
        for (int ii = 0; ii < 8; ii++) {
            if ((rand() / double(RAND_MAX)) * mutationrate < 1.0) {
                bias[i][2][ii] = (rand() / double(RAND_MAX)) - 0.5;
            }
            for (int iii = 0; iii < 16; iii++) {
                if ((rand() / double(RAND_MAX)) * mutationrate < 1.0) {
                    multiplier[i][2][ii][iii] = (rand() / double(RAND_MAX)) - 0.5;
                }
            }
        }
        for (int ii = 0; ii < 4; ii++) {
            if ((rand() / double(RAND_MAX)) * mutationrate < 1.0) {
                bias[i][3][ii] = (rand() / double(RAND_MAX)) - 0.5;
            }
            for (int iii = 0; iii < 8; iii++) {
                if ((rand() / double(RAND_MAX)) * mutationrate < 1.0) {
                    multiplier[i][3][ii][iii] = (rand() / double(RAND_MAX)) - 0.5;
                }
            }
        }
    }
    copy(&bestmultiplier[0][0][0], &bestmultiplier[0][0][0] + 51290, &multiplier[0][0][0][0]);
    copy(&bestbias[0][0], &bestbias[0][0] + 100, &bias[0][0][0]);
}

void ai(int attempt){
    for (int i = 0; i < score; i++) {
        value[0][i] = snakex[i+1]/17.0;
        value[0][255 + i] = snakey[i + 1] / 15.0;
    }
    value[0][510] = x / 17.0;
    value[0][511] = y / 15.0;
    value[0][512] = foodx / 17.0;
    value[0][513] = foody / 15.0;
    value[0][514] = (d == 1);
    value[0][515] = (d == 2);
    value[0][516] = (d == 3);
    value[0][517] = (d == 4);
    for (int i = 0; i < 32; i++) {
        value[1][i] = bias[attempt][0][i];
        for (int ii = 0; ii < score; ii++) {
            value[1][i] += value[0][ii] * multiplier[attempt][0][i][ii]
                + value[0][ii+255] * multiplier[attempt][0][i][ii+255];
        }
        for (int ii = 510; ii < 518; ii++) {
            value[1][i] += value[0][ii] * multiplier[attempt][0][i][ii];
        }
        value[1][i] *= (value[1][i] > 0);
    }
    for (int i = 0; i < 16; i++) {
        value[2][i] = bias[attempt][1][i];
        for (int ii = 0; ii < 32; ii++) {
            value[2][i] += value[1][ii] * multiplier[attempt][1][i][ii];
        }
        value[2][i] *= (value[2][i] > 0);
    }
    for (int i = 0; i < 8; i++) {
        value[3][i] = bias[attempt][2][i];
        for (int ii = 0; ii < 16; ii++) {
            value[3][i] += value[2][ii] * multiplier[attempt][2][i][ii];
        }
        value[3][i] *= (value[3][i] > 0);
    }
    for (int i = 0; i < 4; i++) {
        value[4][i] = bias[attempt][3][i];
        for (int ii = 0; ii < 8; ii++) {
            value[4][i] += value[3][ii] * multiplier[attempt][3][i][ii];
        }
        value[4][i] *= (value[4][i] > 0);
    }
    int max_index = 0;
    for(int i = 0; i < 4; i++) {
        if(value[4][i] > value[4][max_index]) {
            max_index = i;
        }
    }
    aid = max_index + 1;
    if ((d - aid) % 2 != 0 || d == 0) {
        d = aid;
    }
}

int collision(int a, int b) {
    for (int i = score; i > 0; i--) {
        if (snakex[i] == a && snakey[i] == b) {
            return 1;
        }
    }
    return 0;
}

void move() {
    x += (d == 3) - (d == 1);
    y += (d == 4) - (d == 2);
    for (int i = score; i > 0; i--) {
        snakex[i] = snakex[i-1];
        snakey[i] = snakey[i-1];
    }
    snakex[0] = x;
    snakey[0] = y;
    if (x == foodx && y == foody) {
        score++;
        timer = 0;
        foodx = foodxp[score];
        foody = foodyp[score];
        for (; collision(foodx, foody) || (foodx == x && foody == y);) {
            foodx = (rand() / (double(RAND_MAX) + 1)) * 17 + 1;
            foody = (rand() / (double(RAND_MAX) + 1)) * 15 + 1;
        }
        dstart = abs(x - foodx) + abs(y - foody);
        dmin = dstart;
    }
    if (dmin > abs(x - foodx) + abs(y - foody)) {
        dmin = abs(x - foodx) + abs(y - foody);
    }
    if(x > 17 || x < 1 || y < 1 || y > 15 || collision(x, y) || timer > 500) {
        timer = 0;
        fitness[att] += score + (dstart-dmin)/static_cast<float>(2*dstart);
        att++;
        if (score > highscore) {
            highscore = score;
            mutationrate = 988 + 64 * highscore;
        }
        score = 0;
        d = 0;
        x = 9;
        y = 7;
        snakex[0] = x;
        snakey[0] = y;
        foodx = foodxp[0];
        foody = foodyp[0];
        for (; (foodx == x && foody == y);) {
            foodx = (rand() / (double(RAND_MAX) + 1)) * 17 + 1;
            foody = (rand() / (double(RAND_MAX) + 1)) * 15 + 1;
        }
        dstart = abs(x - foodx) + abs(y - foody);
        dmin = dstart;
        for (int i = 0; i < 510; i++) {
            value[0][i] = 0;
        }
    }
}

int main(){
    srand(time(0));
    food();
    foodx = foodxp[0];
    foody = foodyp[0];
    for (;(foodx == x && foody == y);) {
        foodx = (rand() / (double(RAND_MAX) + 1)) * 17 + 1;
        foody = (rand() / (double(RAND_MAX) + 1)) * 15 + 1;
    }
    dstart = abs(x - foodx) + abs(y - foody);
    dmin = dstart;
    for (int i = 0; i < 510; i++) {
        value[0][i] = 0;
    }
    cin >> generations; 
    auto start = chrono::high_resolution_clock::now();
    snakex[0] = x;
    snakey[0] = y;
    for (int i = 0; i < gensize; i++) {
        randomize(i);
    }
    for (int ii = 0; ii < generations; ii++) {
        for (int j = 0; j < gensize; j++) {
            fitness[j] = 0;
        }
        for (int i = 0; i < tries; i++) {
            while(att < gensize){
                timer++;
                ai(att);
                move();
            }
            att = 0;
            food();
        }
        cout << *(max_element(begin(fitness), end(fitness)))/tries << '\n';
        selection();
    }
    //save arrays
    string weight_string = "";
    weight_string += "[[";
    for (int i = 0; i < 32; i++) {
        weight_string += '[';
        for (int ii = 0; ii < 518; ii++) {
            weight_string += to_string(multiplier[0][0][i][ii]);
            if (ii < 517) {
                weight_string += ',';
            }
        }
        weight_string += ']';
        if (i < 31) {
            weight_string += ',';
        }
    }
    weight_string += "],[";
    for (int i = 0; i < 16; i++) {
        weight_string += '[';
        for (int ii = 0; ii < 32; ii++) {
            weight_string += to_string(multiplier[0][1][i][ii]);
            if (ii < 31) {
                weight_string += ',';
            }
        }
        weight_string += ']';
        if (i < 15) {
            weight_string += ',';
        }
    }
    weight_string += "],[";
    for (int i = 0; i < 8; i++) {
        weight_string += '[';
        for (int ii = 0; ii < 16; ii++) {
            weight_string += to_string(multiplier[0][2][i][ii]);
            if (ii < 15) {
                weight_string += ',';
            }
        }
        weight_string += ']';
        if (i < 7) {
            weight_string += ',';
        }
    }
    weight_string += "],[";
    for (int i = 0; i < 4; i++) {
        weight_string += '[';
        for (int ii = 0; ii < 8; ii++) {
            weight_string += to_string(multiplier[0][3][i][ii]);
            if (ii < 7) {
                weight_string += ',';
            }
        }
        weight_string += ']';
        if (i < 3) {
            weight_string += ',';
        }
    }
    weight_string += "]]";
    std::ofstream outputFile(file_path1);
    outputFile << weight_string;
    outputFile.close();
    
    string bias_string = "";
    bias_string += "[[";
    for (int i = 0; i < 32; i++) {
        bias_string += to_string(bias[0][0][i]);
        if (i < 31) {
            bias_string += ',';
        }
    }
    bias_string += "],[";
    for (int i = 0; i < 16; i++) {
        bias_string += to_string(bias[0][1][i]);
        if (i < 15) {
            bias_string += ',';
        }
    }
    bias_string += "],[";
    for (int i = 0; i < 8; i++) {
        bias_string += to_string(bias[0][2][i]);
        if (i < 7) {
            bias_string += ',';
        }
    }
    bias_string += "],[";
    for (int i = 0; i < 4; i++) {
        bias_string += to_string(bias[0][3][i]);
        if (i < 3) {
            bias_string += ',';
        }
    }
    bias_string += "]]";
    std::ofstream outputFile2(file_path2);
    outputFile2 << bias_string;
    outputFile2.close();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << duration.count() << "s";
    cout << '\n';
    std::cout << timer2/1000000.0 << "s" << '\n';
    cout << fitness[0]/tries;
    return 0;
}