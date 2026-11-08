#include "flappy.h"

void advancePipes(Game *game, float dt) {
    /* TODO 1: Move every pipe left by PIPE_SPEED * dt. Then visit the slots
     * in array order; if a pipe's right edge is <= 0, call recyclePipe. */
    (void)game;
    (void)dt;
}

bool birdHitsObstacle(const Game *game) {
    /* TODO 2: Check the top/floor boundaries and both rectangles of each
     * pipe pair. Use gameBirdBox and boxesOverlap. Do not change the game. */
    (void)game;
    return false;
}

int scorePassedPipes(Game *game) {
    /* TODO 3: A pipe scores once when its right edge is strictly left of
     * BIRD_X. Mark it scored, add to m_score, and return this call's count. */
    (void)game;
    return 0;
}

unsigned int nextRandom(unsigned int seed) {
    /* TODO: Keep one static unsigned int state, initially RANDOM_INITIAL_SEED.
     * A positive seed resets it modulo RANDOM_MODULUS. Then take one
     * recurrence step and return the new state. Zero continues the sequence. */
    (void)seed;
    return 0;
}
