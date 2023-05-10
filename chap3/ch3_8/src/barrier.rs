use std::{sync::{Arc, Barrier}, thread};

pub fn run_barrier() {
    let mut v = Vec::new();

    // 10개의 스레드 만큼의 배리어 동기를 Arc로 래핑.
    let barrier = Arc::new(Barrier::new(10));

    for _ in 0..10 {
        let b = barrier.clone();
        let th = thread::spawn(move || {
            b.wait();
            println!("finished barrier");
        });
        v.push(th);
    }

    for th in v {
        th.join().unwrap();
    }
}