/**
 * @file states.c
 * @brief Defines states the program can be in.
 */
#pragma once

/**
 * @brief States the program can be in.
 */
typedef enum {
    STATE_IDLE,        //< nothing is happening
    STATE_WAIT_INPUT,  //< waiting for user input
    STATE_FIN_GEN,     //< just finished generating
    STATE_GENERATING,  //< generating a maze
    STATE_GEN_WAIT,    //< wait for something to complete
} State;
