mod mutex;
mod cond;
mod rwlock;
mod barrier;
mod semaphore;
mod channels;

use crate::mutex::run_mutex;
use crate::cond::run_cond;
use crate::rwlock::run_rwlock;
use crate::barrier::run_barrier;
use crate::channels::run_channels;

fn main() {
    // run_mutex();
    // run_cond();
    // run_rwlock();
    // run_barrier();
    run_channels();
}
