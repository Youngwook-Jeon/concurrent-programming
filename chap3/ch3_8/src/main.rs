mod mutex;
mod cond;
mod rwlock;
mod barrier;

use crate::mutex::run_mutex;
use crate::cond::run_cond;
use crate::rwlock::run_rwlock;
use crate::barrier::run_barrier;

fn main() {
    // run_mutex();
    // run_cond();
    // run_rwlock();
    run_barrier();
}
