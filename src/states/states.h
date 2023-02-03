/**
 * @file states.c
 * @brief Defines states the program can be in.
 */
#pragma once

/**
 * @brief States the program can be in.
 */
typedef enum {
    STATE_IDLE,       /** nothing is happening */
    STATE_WAIT_INPUT, /** waiting for user input */
    STATE_GENERATING, /** generating a maze */
    STATE_GEN_WAIT,   //< wait for something to complete
} State;

typedef enum {
    LOOP_BEGIN,
    LOOP_MIDDLE,
    LOOP_SWITCH,
    LOOP_END
} LoopState;
