use std::sync::{Arc, Mutex};
use std::thread;

fn some_func(lock: Arc<Mutex<u64>>) {
    loop {
        // Mutex타입 값의 참조를 위해 반드시 락킹
        // MutexGuard 변수인 val의 스코프를 벗어날 때 자동으로 락을 해제함.
        let mut val = lock.lock().unwrap();
        *val += 1;
        println!("{}", *val);
    }
}

pub fn run_mutex() {
    // Arc: 스레드 세이프한 참조 카운터 타입의 스마트 포인터
    let lock0 = Arc::new(Mutex::new(0));

    // 참조 카운터가 증가, 내용은 클론 X
    let lock1 = lock0.clone();

    let th0 = thread::spawn(move || {
        some_func(lock0);
    });

    let th1 = thread::spawn(move || {
        some_func(lock1);
    });

    th0.join().unwrap();
    th1.join().unwrap();
}
