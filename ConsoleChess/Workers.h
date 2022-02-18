///*
// * c-chess-cli, a command line interface for UCI chess engines. Copyright 2020 lucasart.
// *
// * c-chess-cli is free software: you can redistribute it and/or modify it under the terms of the GNU
// * General Public License as published by the Free Software Foundation, either version 3 of the
// * License, or (at your option) any later version.
// *
// * c-chess-cli is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// * General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License along with this program. If
// * not, see <http://www.gnu.org/licenses/>.
// */
//#pragma once
//
//#include "Strings.h"
//#include <inttypes.h>
//#include <pthread.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include <string>
//
//// Game results
//enum { RESULT_LOSS, RESULT_DRAW, RESULT_WIN, NB_RESULT };
//
//// Per thread data
//struct Worker {
//    struct deadline {
//        pthread_mutex_t mtx;
//        int64_t timeLimit;
//        std::string engineName;
//        bool set;
//    };
//    uint64_t seed; // seed for prng()
//    int id;        // starts at 1 (0 is for main thread)
//    Worker(int i);
//    ~Worker();
//};
//
//extern Worker* vecWorkers;
//
//Worker worker_init(int id);
//void worker_destroy(Worker* w);
//
//void deadline_set(Worker* w, std::string engineName, long long now, long long timeLimit);
//void deadline_clear(Worker* w);
//bool deadline_overdue(Worker* w);
//
//void workers_busy_add(int n);
//int workers_busy_count(void);
