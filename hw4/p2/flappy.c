#include "flappy.h"

void advancePipes(Game *game, float dt) {
    /* TODO 1: Move every pipe left by PIPE_SPEED * dt. Then visit the slots
     * in array order; if a pipe's right edge is <= 0, call recyclePipe. */
    for (int i = 0; i < PIPE_COUNT; ++i) {
        game->m_pipes[i].m_x -= dt * PIPE_SPEED;
    }
    for (int i = 0; i < PIPE_COUNT; ++i) {
        if (game->m_pipes[i].m_x + PIPE_WIDTH <= 0) {
            recyclePipe(game, i);
        }
    }
}

bool birdHitsObstacle(const Game *game) {
    /* TODO 2: Check the top/floor boundaries and both rectangles of each
     * pipe pair. Use gameBirdBox and boxesOverlap. Do not change the game. */
    Box bird = gameBirdBox(game);
    if (bird.m_y < 0 || bird.m_y + BIRD_HEIGHT > FLOOR_Y) {
        return true;
    }
    for (int i = 0; i < PIPE_COUNT; ++i) {
        Pipe pipe = game->m_pipes[i];
        Box upperPipe = {
            pipe.m_x,
            0,
            PIPE_WIDTH,
            pipe.m_gap_y,
        };
        Box lowerPipe = {pipe.m_x, pipe.m_gap_y + PIPE_GAP_HEIGHT, PIPE_WIDTH,
                         FLOOR_Y - (pipe.m_gap_y + PIPE_GAP_HEIGHT)};
        if (boxesOverlap(bird, upperPipe) || boxesOverlap(bird, lowerPipe)) {
            return true;
        }
    }
    return false;
}

int scorePassedPipes(Game *game) {
    /* TODO 3: A pipe scores once when its right edge is strictly left of
     * BIRD_X. Mark it scored, add to m_score, and return this call's count. */
    int points = 0;
    for (int i = 0; i < PIPE_COUNT; ++i) {
        if (!game->m_pipes[i].m_scored && game->m_pipes[i].m_x + PIPE_WIDTH < BIRD_X) {
            game->m_pipes[i].m_scored = true;
            points += 1;
        }
    }
    game->m_score += points;
    return points;
}

unsigned int nextRandom(unsigned int seed) {
    /* TODO: Keep one static unsigned int state, initially RANDOM_INITIAL_SEED.
     * A positive seed resets it modulo RANDOM_MODULUS. Then take one
     * recurrence step and return the new state. Zero continues the sequence. */
    static unsigned int state = RANDOM_INITIAL_SEED;
    if (seed > 0) {
        state = seed % RANDOM_MODULUS;
    }

    state = (RANDOM_MULTIPLIER * state + RANDOM_INCREMENT) % RANDOM_MODULUS;

    return state;
}
