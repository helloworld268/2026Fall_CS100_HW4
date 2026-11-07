#include "snake.h"
#include <stdlib.h>

bool snakeInit(Snake *snake, int rows, int cols, size_t length) {
    /* TODO: Make a ring. Start in the middle, facing right.
     * On failure, free any nodes already made and leave an empty snake. */
    *snake = (Snake){0};
    if (rows < MIN_BOARD_SIZE || cols < MIN_BOARD_SIZE) {
        return false;
    }
    if (length == 0 || length > (size_t)(cols - 2 * BOARD_WALL_THICKNESS)) {
        return false;
    }
    int rowOfHead = rows / 2;
    int colOfHead = (size_t)(cols / 2) < (length) ? (int)length : (cols / 2);
    SnakeNode *head = malloc(sizeof *head);

    if (head == NULL) {
        return false;
    }
    head->m_pos = (Coord){rowOfHead, colOfHead};
    head->m_next = head;

    for (size_t i = 1; i < length; ++i) {
        SnakeNode *newNode = malloc(sizeof *newNode);
        if (newNode == NULL) {
            SnakeNode *current = head;
            for (size_t j = 0; j < i; ++j) {
                SnakeNode *next = current->m_next;
                free(current);
                current = next;
            }
            return false;
        }
        newNode->m_pos = (Coord){rowOfHead, colOfHead - (int)i};
        newNode->m_next = head->m_next;
        head->m_next = newNode;
    }
    snake->m_head = head;
    snake->m_length = length;
    snake->m_direction = DIR_RIGHT;
    snake->m_rows = rows;
    snake->m_cols = cols;
    return true;
}

Coord snakeNextPosition(const Snake *snake, Direction requested) {
    /* TODO: Return the next head position. Do not change the snake.
     * Ignore a turn straight back when the snake has more than one node. */
    Direction direction = snake->m_direction;
    if (requested >= DIR_UP && requested < DIRECTION_COUNT) {
        direction = requested;
    }
    if (snake->m_length > 1 && direction == (snake->m_direction + 2) % DIRECTION_COUNT) {
        direction = snake->m_direction;
    }

    Coord nextPos = snake->m_head->m_pos;

    switch (direction) {
    case DIR_UP:
        nextPos.m_row -= 1;
        break;
    case DIR_RIGHT:
        nextPos.m_col += 1;
        break;
    case DIR_DOWN:
        nextPos.m_row += 1;
        break;
    case DIR_LEFT:
        nextPos.m_col -= 1;
        break;
    default:
        break;
    }
    return nextPos;
}

bool snakeContains(const Snake *snake, Coord pos) {
    /* TODO: Check every node once. The ring does not end at NULL. */
    if (snake->m_length == 0 || snake->m_head == NULL) {
        return false;
    }

    const SnakeNode *current = snake->m_head;
    for (size_t i = 0; i < snake->m_length; ++i) {
        if (current->m_pos.m_row == pos.m_row && current->m_pos.m_col == pos.m_col) {
            return true;
        }
        current = current->m_next;
    }
    return false;
}

StepResult snakeStep(Snake *snake, Direction requested, bool grow) {
    /* TODO: Check the next position before changing the ring.
     * Reuse the tail on a normal move; add one node when growing.
     * A normal move may enter the old tail's cell. A growing move may not. */
    Coord nextPosition = snakeNextPosition(snake, requested);
    if (nextPosition.m_row < BOARD_WALL_THICKNESS ||
        nextPosition.m_row >= snake->m_rows - BOARD_WALL_THICKNESS ||
        nextPosition.m_col < BOARD_WALL_THICKNESS ||
        nextPosition.m_col >= snake->m_cols - BOARD_WALL_THICKNESS) {
        return STEP_WALL;
    }
    SnakeNode *tail = snake->m_head->m_next;
    bool crashTail =
        nextPosition.m_col == tail->m_pos.m_col && nextPosition.m_row == tail->m_pos.m_row;
    if (snakeContains(snake, nextPosition)) {
        if (crashTail) {
            if (grow) {
                return STEP_SELF;
            }
        } else {
            return STEP_SELF;
        }
    }

    Direction direction = snake->m_direction;
    if (requested >= DIR_UP && requested < DIRECTION_COUNT) {
        direction = requested;
    }
    if (snake->m_length > 1 && direction == (snake->m_direction + 2) % DIRECTION_COUNT) {
        direction = snake->m_direction;
    }

    if (grow) {
        SnakeNode *newHead = malloc(sizeof *newHead);

        if (newHead == NULL) {
            return STEP_NO_MEMORY;
        }

        newHead->m_pos = nextPosition;
        newHead->m_next = tail;
        snake->m_head->m_next = newHead;

        snake->m_head = newHead;
        snake->m_length += 1;
    } else {
        tail->m_pos = nextPosition;
        snake->m_head = tail;
    }
    snake->m_direction = direction;
    return STEP_OK;
}

void snakeDestroy(Snake *snake) {
    /* TODO: Free every node, then reset the snake. */
    SnakeNode *current = snake->m_head;
    for (size_t i = 0; i < snake->m_length; ++i) {
        SnakeNode *next = current->m_next;
        free(current);
        current = next;
    }
    *snake = (Snake){0};
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
